#include "headers/main.h"
#include "headers/input_output.h"


int main(int argc, char **argv)
{

    double T_x_min_1 = t0, T_x_pl_1 = t0, T_y_min_1= t0, T_y_pl_1=t0, T_x_y=t0;
    double convective = 0.0, diffusive = 0.0;
    int count=0,converged_flag=0;

    input_data();
    calculate_timesteps();                         //Set correct time step and mapping the grid to the inputs

   printf("\nChecking for convergence..");

    //Initializing grid to initial temperatures
    for(i = 0; i<GRID_SIZE; i++)
    {
        for(j = 0; j<GRID_SIZE;j++)
        {
            if((i>=n_x && i < n_x+n_width_x) && (j>=n_y && j< n_y+n_width_y))       //TODO: CHANGE the conditions
            {
                T[i][j] = t_needle;
                T_new[i][j] = t_needle;
            }
            else
            {
                T[i][j] = t0;
                T_new[i][j] = t0;
            }
        }
    }
    output_temperature(time);

   // for(time = 0,count=0;(converged_flag==0 && time <= range);time += t_step,count++)                                              //Apply a convergence criteria
   // {
    time =0;count=0;
    while((converged_flag==0 ))                                         //Changed from : (converged_flag==0 && time <= range)
    {
        for(i =0;i<GRID_SIZE;i++)
        {
            for(j=0;j<GRID_SIZE;j++)
            {

                    T_x_min_1 = get_temperature(i-1,j);
                    T_x_pl_1 = get_temperature(i+1,j);
                    T_x_y = get_temperature(i,j);
                    T_y_min_1 = get_temperature(i,j-1);
                    T_y_pl_1 = get_temperature(i,j+1);

                    convective = (vel_x(i,j)*(T_x_pl_1 - T_x_y)/(x_step)) +
                                (vel_y(i,j)*(T_y_pl_1 - T_x_y)/(y_step));                  //Upwind  for convective term

                    diffusive = ((T_x_min_1-2*T_x_y+T_x_pl_1)/(x_step*x_step)) +            //Central Difference 2nd order accurate
                                ((T_y_min_1-2*T_x_y+T_y_pl_1)/(y_step*y_step));

                    T_new[i][j] = T[i][j] + t_step*(-convective + k*diffusive);              //Upwind for Time


                    if((i >= n_x && i < n_x+n_width_x) && (j>= n_y && j< n_y+n_width_y))
                    { //Keeping the needle temperature constant

                        T_new[i][j] = t_needle;
                    }


            }
            residue_norm = T_new[1][1]-T[1][1];
            for(x=0,percent_tissue_burnt=0; x<GRID_SIZE;x++)
            {                                                                   //Copying new values into array
                for(y=0;y<GRID_SIZE;y++)
                {
                    residue[x][y] = T_new[x][y]-T[x][y];
                    if(residue_norm<residue[x][y])
                        residue_norm = residue[x][y];
                    T[x][y] = T_new[x][y];
                    if(T[x][y]>=t_tresh)
                        percent_tissue_burnt++;

                }
            }
        }
        if(count >=500 && count%500 == 0)
        {
            if(max_residue<residue_norm)
                max_residue = residue_norm;
            else
                checking_convergence=1;
            converged_flag = check_convergence(residue_norm);                     //Returns 1 when the solution goes towards convergence
            percent_tissue_burnt = (percent_tissue_burnt*100)/(GRID_SIZE*GRID_SIZE);
            output_temperature(count);
            printf("..");
        }
        if(count >= 7500)
        {
            printf("\n\n\nThe given scenario is converging VERY slowly.\nPlease see the outputs in the \"output\" folder to verify convergence.\n\n\n");
            converged_flag=1;
        }
        time += t_step;
        count++;
    }
//    output_temperature(time);

    printf("\n\nPlease check for outputs in the outputs folder.\n\n");
    return (0);
}

double get_temperature(int x, int y)
/*Checks for boundary conditions before returning the value from array*/
{
    if(x>=GRID_SIZE-1 && (y>=blood_vessel_y && y<blood_vessel_y+a))         //TODO: change the condition
    /*No Boundary conditions, At the right end of blood vessel.
      Returns the temperature at the right end of blood vessel
      (does this create a similar effect as letting the blood flow !?!)*/
    {
        //return(T[19][y]);
        return(get_temperature(x-1,y));
    }
    else
    {
        if(x<0 || y<0 || x>=GRID_SIZE||y>=GRID_SIZE)      //At boundaries
            return(t0);
        else
            return(T[x][y]);
    }

}

double vel_x(x,y)
{
    if(y>=blood_vessel_y && y<blood_vessel_y+a)
    {
        if(y*y*v<vmax)
            return(y*y*v);
        else
            return(vmax);
//        return(y*y*v);
    }
    else
        return(0.0);
}

double vel_y(x,y)
{
    return(0);
}


int check_convergence(double residue_inf_norm)
{
    if(checking_convergence == 1 && (residue_inf_norm <= max_residue/10))
        return (1);                                              //Stops the for loop in main() if convergence is reached
    else
        return(0);                                              //continues the for loop in main() as convergence is yet to be reached

    return(0);
}
