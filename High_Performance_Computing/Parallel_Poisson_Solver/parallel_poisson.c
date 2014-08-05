/*------------------ Parallel Poisson Solver Using MPI---------------------------------------

The program tries to solve Poisson equation in 2D with dirchlet boundary conditions using
Jacobi Method. 

-

-

- 

---------------------------------------------------------------------------------------------*/

#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define NX 100
#define NY 100
#define PHI0 0
#define PHI1 100
#define RHO 0
#define ERROR 1E-7
#define MASTER 0
#define ITR_DATA 1
#define ITR_DATA2 2
#define FINAL_DATA 5
#define FINAL_PHI 10
#define FINAL_GRADX 100
#define FINAL_GRADY 1000

int main(int argc, char *argv[])
{

	/*Grid Parameters*/
	float phi[NX][NY];
	float phi_old[NX][NY];
	float rho[NX][NY];
	float gradient[2][NX][NY];
	float phi0=PHI0, phi1=PHI1;
	float dx=1,dy=1;				//Space steps

	/*Variables used in calculation*/
	float phi_left,phi_right ,
		phi_above  , phi_below ,
		phi_x_y, eta, Rij, sum_R, max_sum_R,tem_err;

	float a = 1/(dx*dx), b = 1/(dy*dy), c=-2/(dx*dx)-2/(dy*dy);
	float rij;
	float omega=0.8;/*Relaxation Factor*/
	float send_buf[NY], recv_buf[NY];
	float *localarray, *master_array;
	
	float error,new_error, cummulative_error=1;

	/* MPI Variables & Loop Variables */
	int i,j,t,k,l;
	int taskid, numprocs,
		prev,next,
		source,dest,
		start, end,
		work_grid_size,endt,startt;
	int ierr, resultlen;
	char err_buffer[MPI_MAX_ERROR_STRING];
	MPI_Status status;
	double t0,t1,elapsed_time;
	MPI_File *result_phi, *result_grad_x, *result_grad_y;

	/*Initializing arrays to 0*/

	for(i=0;i<NX;i++)
	{
		for(j=0;j<NY;j++)
		{
			phi[i][j] = 1;
			phi_old[i][j] = 0;
			rho[i][j] = RHO;
			gradient[0][i][j]=0;
			gradient[1][i][j]=0;
		}
	}

	/* Initializing MPI*/

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);
	MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

	MPI_Errhandler_set(MPI_COMM_WORLD,MPI_ERRORS_RETURN); //return info about errors
	
	/*Defining Processes' Neighbours*/

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

	/*Dividing Tasks Among Processes*/

	work_grid_size = NX/numprocs;
	master_array = (float *)malloc(NX*NY*sizeof(float));
	localarray = (float *)malloc((work_grid_size)*NY*sizeof(float));

	if(taskid == numprocs-1)
	{
		start = taskid*work_grid_size;
		end = NY;
		work_grid_size = end-start;
	} else {
		start = taskid*work_grid_size;
		end = start+work_grid_size;
	}
	/*Running Jacobi Iterations*/ 

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
					//		error = fabs(phi[i][j-1] - phi[i][j]);
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


		/*Pass messages for the overlapping grid points*/

		for(i =0; i<work_grid_size;i++)
		{
			for(j=0;j<NY;j++)
			{
				localarray[i*NY+j] = phi[start+i][j];
			}
		}
		MPI_Allgather(localarray, (work_grid_size)*NY,MPI_FLOAT, &phi[0][0], (work_grid_size)*NY,MPI_FLOAT, MPI_COMM_WORLD);
		/*Use a reduction operation to check for error*/
			
		MPI_Allreduce(&sum_R, &max_sum_R, 1, MPI_FLOAT, MPI_SUM, MPI_COMM_WORLD);

		cummulative_error = sqrt(max_sum_R)/(NX*NY);
	//	if(taskid == 0)
	//		printf("Error: %f,Sum_R: %f, Itr: %d\n",cummulative_error,sum_R,t);
		error=0;

		sum_R = 0;
	t++;	
	}while(t<3||cummulative_error>ERROR);

	/*Gathering phi data from all nodes*/
	for(i = 0; i < work_grid_size; i++)
	{
		for(j = 0; j<NY;j++)
		{
			localarray[i*NY+j] = phi[start+i][j];
			if(taskid == MASTER)
				master_array[i*NY+j] = phi[i][j];
		}
		
	}

	ierr = MPI_Gather(localarray, (work_grid_size)*NY, MPI_FLOAT, master_array, (work_grid_size)*NY, MPI_FLOAT, 0, MPI_COMM_WORLD);

	if(taskid == 0)
	{
		for(i = 0; i < NX; i++)
		{
			for(j = 0; j < NY; j++)
			{
				phi[i][j] = master_array[i*NY+j];
			}
		}
		/*Calculating Gradient using central difference formula*/
	
		for(i = 0; i<=NX;i++)
		{
			for(j=0;j<NY;j++)
			{
				if(i == 0 || i == NX-1 || j == 0 || j == NY-1)
				{
					gradient[0][i][j] = 0;
					gradient[1][i][j] = 0;
				}
				else
				{
					gradient[0][i][j] = (phi[i+1][j]-phi[i-1][j])/(2*dx);
					gradient[1][i][j] = (phi[i][j+1]-phi[i][j-1])/(2*dy);
				}
			}
		}

			/*Writing phi data to screen*/
		for(i = 0; i<NY;i++)
		{
			for(j = 0; j<NY; j++)
			{
				printf("%f ",phi[i][j]);

			}
			printf("\n");

		}
		printf("\n\n");
			/*Writing X-Gradient*/		
		for(i = 0; i<NY;i++)
		{
			for(j = 0; j<NY; j++)
			{
				printf("%f ",gradient[0][i][j]);

			}
			printf("\n");

		}
		printf("\n\n");
			/*Writing Y-Gradient*/
		for(i = 0; i<NY;i++)
		{
			for(j = 0; j<NY; j++)
			{
				printf("%f ",gradient[1][i][j]);

			}
			printf("\n");
		}

	}

	MPI_Finalize();	

	return 0;
}

