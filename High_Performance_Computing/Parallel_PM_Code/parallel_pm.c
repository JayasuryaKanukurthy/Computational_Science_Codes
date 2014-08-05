/*------------------------------------Particle - Mesh Interactions -------------
All particles are locally refered by the indices of the arrays. Hence the ith 
index in Particle_paths[][] variable and particle_info[][] variable refer to the
same particle.
- Only works if the number of MPI tasks is at most 20
------------------------------------------------------------------------------*/


#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#define NX 		50
#define NY 		50
#define PHI0		-10
#define PHI1		10
#define ERROR	 	1E-7
#define MASTER 		0
#define ITR_DATA 	1
#define ITR_DATA2 	2
#define PAR_MIGRATION 	10
#define FINAL_MIGRATION 45

#define TIME_STEPS 		50
#define DT			1
#define PARTICLES 		80
#define LOCAL_PARTICLES		5			//Particles in each grid. Should not exceed 10

#define STAT_ACTIVE 1
#define STAT_DEAD 	0

#define STATUS	0
#define ID 1
#define CHARGE 	0
#define MASS 	1
#define POS_X 	2
#define POS_Y 	3
#define FORCE_X	4
#define FORCE_Y	5


#define X 	0
#define Y 	1


int main(int argc, char *argv[])
{

	/*Grid Parameters*/
	float phi[NX][NY];
	float rho[NX][NY];
	float gradient[2][NX][NY];
	float phi0=PHI0, phi1=PHI1;
	float dx=1,dy=1,dt=DT;				//Space steps

	/*Variables for the particles*/
	float particle_paths[2][PARTICLES][TIME_STEPS+1];
	float particle_info[PARTICLES][6];		//Contains Mass, charge, position_x, position_y, force_x and force_y of the particle during iterations
	float global_particle_list[PARTICLES][2*TIME_STEPS+4]; //Array with all the particle details  <-- Is it Correct !?!
	
	int local_particles[PARTICLES][2],gpl_count = 0;				//Contains ID and STATUS of particles being computed in the grid

	float particle_send_buffer[2*TIME_STEPS+3];				//Sending particle_paths(2*Time_steps), charge, mass and status ( +3)
	float final_particle_send[2*TIME_STEPS+3];
	float particle_recv_buffer[2*TIME_STEPS+3];
	float final_particle_recv[2*TIME_STEPS+3];	
	
	
	/*Variables used in solving poisson equation*/
	float phi_left,phi_right ,
		phi_above  , phi_below ,
		phi_x_y, eta, Rij, sum_R, max_sum_R,tem_err;

	float a = 1/(dx*dx), b = 1/(dy*dy), c=-2/(dx*dx)-2/(dy*dy);
	float rij;
	float omega=0.8;/*Relaxation Factor*/
	float send_buf[NY], recv_buf[NY];
	float *localarray, *master_array;
	
	float error,new_error, cummulative_error=1;

	// MPI Variables & Loop Variables 
	int i,j,t,k,l,m,p, gridx, gridy,time;
	int taskid, numprocs,
		prev,next,
		source,dest,
		start, end,
		work_grid_size,endt,startt;
	int ierr, resultlen;
	char err_buffer[MPI_MAX_ERROR_STRING];
	MPI_Status status;
	MPI_Request request;
	double t0,t1,elapsed_time;
	int flag=0,migrate=0,dead=0,id, total_particles = 0;
	int lp_count = 0;	
	//Temporary Variables*/
	float charge, mass, position_x, position_y, x, x1, y, y1;

	//Initializing arrays to 0
	srand(1);
	for(i=0;i<NX;i++)
	{
		for(j=0;j<NY;j++)
		{
			phi[i][j] = 0;
			rho[i][j] = 0;
			gradient[X][i][j]=0;
			gradient[Y][i][j]=0;
		}
	}

	//Todo: Initialize Particle Arrays to 0
	memset(&local_particles[0][0],0,2*PARTICLES*sizeof(int));
	memset(&global_particle_list[0][0],0,(PARTICLES*(2*TIME_STEPS+3))*sizeof(float));

	// Initializing MPI

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN); //return info about errors
	
	//Defining Grid Domain Neighbours

	if(taskid == MASTER)
	{
		prev = -1;
	} else {
		prev = taskid -1;
	}
	if(taskid == numprocs-1)
	{
		next = -1;
	} else {
		next = taskid+1;
	}

	//Dividing Tasks Among Processes

	work_grid_size = NX/numprocs;
	master_array = (float *)malloc(NX*NY*sizeof(float));
	localarray = (float *)malloc(work_grid_size*NY*sizeof(float));

	if(taskid == numprocs-1)
	{
		start = taskid*work_grid_size;
		end = NY;
		
	} else {
		start = taskid*work_grid_size;
		end = start+work_grid_size;
	}
	
	//Initialize Particle information locally
	for(i=0;i<LOCAL_PARTICLES;i++)
	{
		id = taskid*LOCAL_PARTICLES+i;
		local_particles[id][STATUS] = STAT_ACTIVE;
		local_particles[id][ID] = id;
		position_x = rand()%work_grid_size+start+1;
		position_y = rand()%NY;
		particle_paths[X][id][0] = position_x;
		particle_paths[Y][id][0] = position_y;
		particle_info[id][POS_X] = position_x;
		particle_info[id][POS_Y] = position_y;
		particle_info[id][CHARGE]=(rand()%5+1)*pow(-1,(int)rand());			//Charge
		particle_info[id][MASS]=rand()%10+1;				//Mass
	}
	lp_count = i;
	total_particles = numprocs*LOCAL_PARTICLES;
	
	//---------------- Particle-Mesh interaction code begins here-----------------
	for(time=1;time<TIME_STEPS-1;time++)					//Check the looping with do-while loop
	{
		//Calculating the charge density on grid using "Nearest Grid Point" 
		memset(&rho[0][0],0,NX*NY*sizeof(float));
		for(p = 0;p<total_particles;p++)
		{
			if(local_particles[p][STATUS] != STAT_DEAD)
			{
				//Finding X and Y positions
				position_x = particle_paths[X][p][time];
				position_y = particle_paths[Y][p][time];
				gridx = (int) position_x;
				gridy = (int) position_y;
				rho[gridx][gridy] += particle_info[p][CHARGE];
			}
		}
		//Solving Poisson Equation 
	t = 0;

	do
	{
			for(i=start; i<end; i++)		
			{
				for(j=0;j<NY;j++)		
				{
					if(i == 0 || i == NX-1)					//Dirchlet Boundary Condition: TOP & BOTTOM
					{
						eta = ((float) j)/NY;
						phi[i][j] = (1-eta)*phi0 + eta*phi1;
					} else {
						phi_above = phi[i-1][j];
						phi_below = phi[i+1][j];
						
						switch(j)			//Dirchlet Boundary Conditions: left, right or default Jacobian Iteration
						{
						case 0:
							phi_left = phi0;
							phi_right = phi[i][j+1];
							break;
						case NY-1 :
							phi_left = phi[i][j-1];
							phi_right = phi1;
							break;
						default:
							phi_left = phi[i][j-1];
							phi_right = phi[i][j+1];
							break;
						}
						
						Rij = (a*(phi_left+phi_right)+b*(phi_above+phi_below)+c*phi[i][j] - rho[i][j])/c; 

						phi[i][j] = phi[i][j]-omega*Rij;


						if(phi[i][j] != 0.0)
						{
							sum_R += Rij*Rij;
						}
						phi[i][j] = phi[i][j];
					}

				}
			}
			for(i=0;i<work_grid_size;i++)
			{
				for(j=0;j<NY;j++)
				{
					localarray[i*NY+j] = phi[start+i][j];
				}
			}


		//Pass messages for the overlapping grid points

		for(i =0; i<work_grid_size;i++)
		{
			for(j=0;j<NY;j++)
			{
				localarray[i*NY+j] = phi[start+i][j];
			}
		}
		MPI_Allgather(localarray, (work_grid_size)*NY,MPI_FLOAT, &phi[0][0], (work_grid_size)*NY,MPI_FLOAT, MPI_COMM_WORLD);
		//Use a reduction operation to check for error
			
		MPI_Allreduce(&sum_R, &max_sum_R, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

		cummulative_error = sqrt(max_sum_R)/(NX*NY);
		error=0;

		sum_R = 0;
	t++;	
	}while(t<3||cummulative_error>ERROR);

		//Calculating Gradient using central difference formula
		for(i = 0; i<NX;i++)
		{
			if(i!=0 && i!= NX-1)
			{
				for(j=1;j<NY-1;j++)
				{
					gradient[0][i][j] = (phi[i+1][j]-phi[i-1][j])/(2*dx);
					gradient[1][i][j] = (phi[i][j+1]-phi[i][j-1])/(2*dy);
				}
			}else continue;
		}
		//Advancing particles & Migrating
		for(p = 0;p < PARTICLES;p++)  //local_particles[p][STATUS] != STAT_DEAD
		{
			if(local_particles[p][STATUS] != STAT_DEAD)
			{	//Calculate the Force on Particles
				x = particle_paths[X][p][time-1];		//Check if this is correct!!
				x1 = (time-1 <= 1)?particle_paths[X][p][0]:particle_paths[X][p][time-2];
				y = particle_paths[Y][p][time-1];
				y1 = (time-1 <= 1)?particle_paths[Y][p][0]:particle_paths[Y][p][time-2];
				mass = particle_info[p][MASS];
				charge = particle_info[p][CHARGE];

				gridx = (int) x;
				gridy = (int) y;
				particle_info[p][FORCE_X] = - charge * gradient[X][gridx][gridy];
				particle_info[p][FORCE_Y] = - charge *gradient[Y][gridx][gridy];
				
				//Advancing particles using Verlet's algorithm
				
				position_x = x+(x-x1)+(dt*dt*particle_info[p][FORCE_X]/mass);
				position_y = y+(y-y1)+(dt*dt*particle_info[p][FORCE_Y]/mass);

				particle_paths[X][p][time] = position_x;
				particle_paths[Y][p][time] = position_y;
				particle_info[p][POS_X] = position_x;
				particle_info[p][POS_Y] = position_y;

				//Checking if particles need migration
				migrate = 0; dead = 0;
				if(position_x<start || position_x >end)
				{
					if((position_x>1 && position_x <NX-1) && (position_y > 1 && position_y<NY-1))
					{
						migrate = 1;
					} else if(position_x<=1 || position_x >= NX-1 || position_y <=1 || position_y >= NY-1)
					{
						migrate = 0;
						dead = 1;
					}
				}else
				{
					if(position_x <= 1 || position_x >= NX-1 || position_y <= 1 || position_y >= NY-1)
						dead = 1;
				}

				//Check status of particles and update those out of scope to inactive
				if(migrate)		//--> particle needs to migrate
				{
					//Finding destination grid
					local_particles[p][STATUS] = STAT_DEAD;
					dest = (int) position_x/work_grid_size;
					//Prepare sending message buffer
					particle_send_buffer[0] = local_particles[p][ID];
					particle_send_buffer[1] = particle_info[p][CHARGE];
					particle_send_buffer[2] = particle_info[p][MASS];
					for(m=3,k=0;k < time+2; m+=2,k++)							//Replaced TIME_STEPS by t
					{
						particle_send_buffer[m] = particle_paths[X][p][k];
						particle_send_buffer[m+1] = particle_paths[Y][p][k];
					}
					
					//Make the Non-Blocking MPI Send
					MPI_Isend(particle_send_buffer, (2*TIME_STEPS+3), MPI_FLOAT, dest, PAR_MIGRATION, MPI_COMM_WORLD, &request);

				}
				else if (dead)
				{
					local_particles[p][STATUS] = STAT_DEAD;
					//When a particle dies send its information to the MASTERS GLOBAL PARTICLES ARRAY
					MPI_Comm_rank(MPI_COMM_WORLD, &taskid);	
					//Sending Message to Master from other tasks
					if(taskid != MASTER)
					{	
						final_particle_send[0] = local_particles[p][ID];
						final_particle_send[1] = particle_info[p][CHARGE];
						final_particle_send[2] = particle_info[p][MASS];
						for(m=3,k=0;k<time+2;m+=2,k++)
						{
							final_particle_send[m] = particle_paths[X][p][k];
							final_particle_send[m+1] = particle_paths[Y][p][k];
						}

						MPI_Isend(final_particle_send,(2*TIME_STEPS+3),MPI_FLOAT, MASTER, FINAL_MIGRATION, MPI_COMM_WORLD, &request);
					} else				//Code for MASTER
					{
						
						global_particle_list[gpl_count][0] = gpl_count;
						global_particle_list[gpl_count][1] = particle_info[p][CHARGE];
						global_particle_list[gpl_count][2] = particle_info[p][MASS];
						for(m=3,k=0;k<time+2;m+=2,k++)
						{
							global_particle_list[gpl_count][m] = particle_paths[X][p][k];
							global_particle_list[gpl_count][m+1] = particle_paths[Y][p][k];
						}
						gpl_count++;
					}
				}
				dead = 0;
				migrate = 0;
			}
			else;
			MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
			//Probing for Incoming Particles
			MPI_Iprobe(MPI_ANY_SOURCE, PAR_MIGRATION, MPI_COMM_WORLD, &flag, &status);
			while(flag !=0 )
			{ //Receiving & Probe again for more incoming particles

				for(l = 0; l<lp_count && local_particles[l][STATUS] != STAT_DEAD;l++);	
				MPI_Recv(particle_recv_buffer,(2*TIME_STEPS+3),MPI_FLOAT,status.MPI_SOURCE,PAR_MIGRATION, MPI_COMM_WORLD, &status);
				id = l;			//Get Location to copy

				//Update Particle Information
				local_particles[id][STATUS] = STAT_ACTIVE;
				particle_info[id][CHARGE] = particle_recv_buffer[1];
				particle_info[id][MASS] = particle_recv_buffer[2];
				for(m=3,k=0;k < time+2; m+=2,k++)
				{
					particle_paths[X][id][k] = particle_recv_buffer[m];
					particle_paths[Y][id][k] = particle_recv_buffer[m+1];
				}
				if(l>=lp_count)
					lp_count++;	
				MPI_Iprobe(MPI_ANY_SOURCE, PAR_MIGRATION, MPI_COMM_WORLD,&flag,&status);
			}
			flag = 0;
			MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
			if(taskid == MASTER)
			{
				//Receive Final Particle Migrations and place in global_particle_array
				MPI_Iprobe(MPI_ANY_SOURCE,FINAL_MIGRATION, MPI_COMM_WORLD, &flag, &status);
				while(flag !=0)
				{	
					MPI_Irecv(final_particle_recv, (2*TIME_STEPS+3),MPI_FLOAT,status.MPI_SOURCE,FINAL_MIGRATION,MPI_COMM_WORLD,&request);
					global_particle_list[gpl_count][0] = gpl_count;
					global_particle_list[gpl_count][1] = final_particle_recv[1];		//CHARGE
					global_particle_list[gpl_count][2] = final_particle_recv[2];		//MASS
					for(m=3,k=0;k<time+2;m+=2,k++)
					{
						global_particle_list[gpl_count][m] = final_particle_recv[m];
						global_particle_list[gpl_count][m+1]= final_particle_recv[m+1];

					}
					gpl_count++;
					MPI_Iprobe(MPI_ANY_SOURCE, FINAL_MIGRATION, MPI_COMM_WORLD, &flag, &status);
				}
			}
			flag = 0;
			MPI_Barrier(MPI_COMM_WORLD);
		}
	}	
		
	//Copy any remaining Active particles back to Master's Global Array
	for(p = 0; p<PARTICLES;p++)
	{
		if(taskid != MASTER)
		{
			//Send local active particles
			if(local_particles[p][STATUS] == STAT_ACTIVE)
			{
				local_particles[p][STATUS] = STAT_DEAD;
				final_particle_send[0] = local_particles[p][ID];
				final_particle_send[1] = particle_info[p][CHARGE];
				final_particle_send[2] = particle_info[p][MASS];
				for(m=3,k=0;k<TIME_STEPS;m+=2,k++)
				{
					final_particle_send[m]= particle_paths[X][p][k];
					final_particle_send[m+1] = particle_paths[Y][p][k];
				
				}	
				MPI_Send(final_particle_send,(2*TIME_STEPS+3), MPI_FLOAT,MASTER, FINAL_MIGRATION, MPI_COMM_WORLD);
			}
		}
		
	
			else if(taskid == MASTER)
			{
			//Copy local active particles to global array
			if(local_particles[p][STATUS] == STAT_ACTIVE)
			{
				global_particle_list[gpl_count][0] = local_particles[p][ID];
				global_particle_list[gpl_count][1]= particle_info[p][CHARGE];
				global_particle_list[gpl_count][2] = particle_info[p][MASS];
				for(m=3,k=0;k<TIME_STEPS;m+=2,k++)
				{
					global_particle_list[gpl_count][m] = (particle_paths[X][p][k]>0&&particle_paths[X][p][k]<NX)?particle_paths[X][p][k]:0;
					global_particle_list[gpl_count][m+1] = (particle_paths[Y][p][k]>0&&particle_paths[Y][p][k]<NY)?particle_paths[Y][p][k]:0;
				}
				gpl_count++;
			}
		}
		MPI_Barrier(MPI_COMM_WORLD);
		//Probe for in coming final particles & copy to global array
		MPI_Iprobe(MPI_ANY_SOURCE, FINAL_MIGRATION, MPI_COMM_WORLD, &flag, &status);
		while(flag != 0)
		{
			MPI_Recv(final_particle_recv,(2*TIME_STEPS+3), MPI_FLOAT, status.MPI_SOURCE, FINAL_MIGRATION, MPI_COMM_WORLD,&status);
			global_particle_list[gpl_count][0] = final_particle_recv[0];
			global_particle_list[gpl_count][1] = final_particle_recv[1];
			global_particle_list[gpl_count][2] = final_particle_recv[2];
			for(i=3;i<2*TIME_STEPS+3;i++)
			{
				global_particle_list[gpl_count][i] = (final_particle_recv[i]>0 && final_particle_recv[i]<NX)?final_particle_recv[i]:0;
			}
			gpl_count++;
			MPI_Iprobe(MPI_ANY_SOURCE, FINAL_MIGRATION, MPI_COMM_WORLD, &flag, &status);
		}
	}

	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);
	MPI_Barrier(MPI_COMM_WORLD);
	if(taskid == MASTER)
	{
		for(i = 0; i< total_particles;i++)
		{
			printf("%f %f ",global_particle_list[i][1],global_particle_list[i][2]);
			for(j=3;j<2*TIME_STEPS;j+=2)
			{	
				printf("%f %f ",global_particle_list[i][j],global_particle_list[i][j+1]);
			}
			printf("\n");

		}
	}


	MPI_Finalize();	

	return 0;
}

