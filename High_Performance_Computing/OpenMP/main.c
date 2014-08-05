#include "main.h"

int main(int argc, char **argv)
{
	char str[128];

	//Reading Inputs from file "inputs"

	fp_inputs = fopen("inputs","r+");
	fgets(str,127,fp_inputs);
	xmin = atol(str);
	fgets(str,127,fp_inputs);
	xmax = atol(str);
	fgets(str,127,fp_inputs);
	ymin = atol(str);
	fgets(str,127,fp_inputs);
	ymax = atol(str);

	printf("%lf\t%lf\t%lf\t%lf\t%d\t%d\n",xmin,xmax,ymin,ymax,a,b);
	fclose(fp_inputs);

	fp_inputs = fopen("matlab_inputs","w+");

	nx = (xmax-xmin)/GRID_SIZE;
	ny = (ymax-ymin)/GRID_SIZE;
	
	printf("nx: %lf\t ny: %lf\n",nx,ny);

	fprintf(fp_inputs, "%lf\n%lf\n%lf\n%lf\n%d\n",xmin,xmax,ymin,ymax,GRID_SIZE);
	fprintf(fp_inputs, "%d\n%d\n%lf\n%lf\n",a,b,nx,ny);
	fclose(fp_inputs);
	
	grad_max = 0;
	#pragma omp parallel for
	{
	for(j=0,y=ymin ; j<GRID_SIZE && y<= ymax ; j++,y+=ny)
	{

		for(i=0,x = xmin; i<GRID_SIZE && x <= xmax ; i++,x+=nx)
		{
			grad_x[i][j] = (phi(x+nx,y)-phi(x-nx,y))/(2*nx);
			grad_y[i][j] = (phi(x,y+ny)-phi(x,y-ny))/(2*ny);
			grid_x[i][j] = x;
			grid_y[i][j] = y;

			grad = sqrt(grad_x[i][j]*grad_x[i][j]+grad_y[i][j]*grad_y[i][j]);
			gradient[i][j] = grad;
			if(grad_max < grad)
			{
				grad_max = grad;
			}
		}
		
	}
	}

	printf("\nMaximum gradient: %lf\n\n",grad_max);
	fp_gradx = fopen(out_grad_x,"w+");
	fp_grady = fopen(out_grad_y,"w+");
	fp_meshx = fopen(out_mesh_x,"w+");
	fp_meshy = fopen(out_mesh_y,"w+");
	fp_grad = fopen(out_grad, "w+");


	printf("Writing Output to Files\n");

	for(j = 0; j < GRID_SIZE; j++)
	{
		for(i = 0; i < GRID_SIZE; i++)
		{
			fprintf(fp_gradx, "%14.11lf ",grad_x[i][j]);
			fprintf(fp_grady, "%14.11lf ",grad_y[i][j]);
			fprintf(fp_meshx, "%14.11lf ",grid_x[i][j]);
			fprintf(fp_meshy, "%14.11lf ",grid_y[i][j]);
			fprintf(fp_grad, "%14.11lf ",gradient[i][j]);
		}
		fprintf(fp_gradx, "\n");
		fprintf(fp_grady, "\n");
		fprintf(fp_meshx, "\n");
		fprintf(fp_meshy, "\n");
		fprintf(fp_grad, "\n");
	}
	printf("Completed File IO.\n");

	return(0);
}

double phi(double x, double y)
{
	return( a*x+b*(x/(x*x+y*y)));
}

