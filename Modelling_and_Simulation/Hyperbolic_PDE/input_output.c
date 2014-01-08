#include "headers/input_output.h"

void input_data()
{
    printf("Enter:\n\n");
    printf("Length (Lx) and Width (Ly) of the tissue:\n");
    scanf("%d %d", &lx,&ly);
    printf("Needle Position (xn) and (yn): \n");
    scanf("%d %d",&n_x,&n_y);
    printf("Needle Size (sx) and (sy): \n");
    scanf("%d %d",&n_width_x,&n_width_y);
    printf("Blood Vessel position (vx) and (vy): \n");
    scanf("%d %d",&blood_vessel_x,&blood_vessel_y);
    printf("Blood Vessel width in x and y direction: \n");
    scanf("%d %d",&b,&a);
    printf("Minimum and Maximum blood velocity in the vein: \n");
    scanf("%lf %lf",&v, &vmax);
    printf("Tissue, Needle and Burn Temperature:\n");
    scanf("%lf %lf %lf",&t0,&t_needle,&t_tresh);
}

void calculate_timesteps()
{
    int flag = 1;
    double t_x,t_y;
    int needle_start_x, needle_start_y;

    x_step = ((float)lx)/(GRID_SIZE-1);
    y_step = ((float)ly)/(GRID_SIZE-1);
    t_x = x_step*x_step/(2*k);
    t_y = y_step*y_step/(2*k);

    while(flag)
    {
        if((k*t_x/(x_step*x_step) <0.5) && (k*t_y/(y_step*y_step)<0.5))
        {
            if(t_x<=(x_step/vmax) && t_y<=(y_step/vmax))
                flag = 0;
        }
        else
        {
            t_x /=2;             //Reducing time step by half
            t_y /=2;
        }
    }
    if(t_x<=t_y)
        t_step = t_x;
    else
        t_step = t_y;

 //   range = 4000*t_step;
    //Calculating the grid co-ordinates of needle and blood vessel
    needle_start_x = roundf((float)n_x/x_step)-(n_width_x/2);
    needle_start_y = roundf((float)n_y/y_step)-(n_width_y/2);

    n_x = needle_start_x;
    n_y = needle_start_y;
    n_width_x /= x_step;
    n_width_y /= y_step;
    blood_vessel_x = roundf((float)blood_vessel_x/x_step);
    blood_vessel_y = roundf((float)blood_vessel_y/y_step);
    a = roundf((float) a /y_step);
    b = roundf((float) b /x_step);

}

void output_temperature(int t)
{
    char ch[20],filename[20] = "time-step-";
    int x,y;
    FILE *fp,*pipe_gp;

    sprintf(ch, "%d",t);
    strcat(filename,ch);
    fp = fopen(filename,"w+");
    pipe_gp = popen("gnuplot","w");

    for(x = 0; x < GRID_SIZE; x++)
    {
        for(y = 0; y < GRID_SIZE; y++)
        {
            if(T[x][y]<t0)
                fprintf(fp,"%d %d %lf\n",x,y,t0);
            else
                fprintf(fp,"%d %d %lf\n",x,y,T[x][y]);
        }
        fprintf(fp,"\n");
    }

    fprintf(pipe_gp, "set terminal png butt crop\n");
    fprintf(pipe_gp, "set output \"%s%s\" \n",filename,".png");
//    fprintf(pipe_gp, "set dgrid3d\n");
    fprintf(pipe_gp, "set title \"Percent Tissue Burnt: %4.2f \"\n",percent_tissue_burnt);
//    fprintf(pipe_gp, "set xrange [0:%d]\n",GRID_SIZE);
//    fprintf(pipe_gp, "set yrange [0:%d]\n",GRID_SIZE);
    fprintf(pipe_gp, "set zrange [%lf:%lf]\n",t0,t_needle);
//    fprintf(pipe_gp,"set pm3d at s\n");
    fprintf(pipe_gp, "set palette defined (30 \"#00008b\", 40 \"#00ffff\", 60 \"yellow\", 100 \"red\", 130 \"#990000\")\n");
    //fprintf(pipe_gp, "set palette rgbformulae 33,13,10\n");
 //   fprintf(pipe_gp, "unset clabel\n");
//    fprintf(pipe_gp, "set contour\n");
//    fprintf(pipe_gp,"set border 32\n");       //26,
    fprintf(pipe_gp,"set view 60,65\n");
//    fprintf(pipe_gp, "set xyplane 4\n");
    fprintf(pipe_gp, "set xlabel \"X-Axis\" \n");
    fprintf(pipe_gp, "set ylabel \"Y-Axis\" \n");
    fprintf(pipe_gp, "set zlabel \"Temp\" \n");
//    fprintf(pipe_gp, "set cntrparam cubicspline\n");
//    fprintf(pipe_gp, "set cntrparam levels incremental %lf, %d, %lf\n",t0,10,t_needle);
    fprintf(pipe_gp, "sp \"%s\" u 1:2:3 w pm3d \n",filename);       //ls 7 palette
//    fprintf(pipe_gp,"e\n");*/
    pclose(pipe_gp);
    fclose(fp);
}
