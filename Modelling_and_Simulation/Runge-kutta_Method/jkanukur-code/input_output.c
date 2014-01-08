#include "headers/input_output.h"

//FILE *ofp,*gnuplot_command_file;
//int data_output_to_file(double xn1, double yn1, double h);
//int create_gnuplot_command_file();

FILE *opf;
char filename[50] = "outputs/model_plot_data";

int initialize_values(double *x, double *y, double *h, double *a, double *b, int *range, int *model)
{
    printf("Enter:\n");
    printf("1. Conventional Model.\n");
    printf("2. Guerilla Model\n");
    printf("Enter Choice: \n");
    scanf("%d",model);
	printf("Initial value of x: \n");
	scanf("%lf", x);
	printf("Initial value of y: \n");
	scanf("%lf", y);
	printf("Combat effectiveness of x: \n");
	scanf("%lf", b);
	printf("Combat effectiveness of y: \n");
	scanf("%lf", a);
	printf("The time step size: \n");
	scanf("%lf", h);
	printf("Range of simulation (multiple of h): \n");
	scanf("%d", range);
	return(0);
}

int create_file(char *filename)
{
    opf = fopen(filename, "w");
    return(0);

}

int output_to_file(double time, double x, double y)
{
    fprintf(opf, "%lf %lf %lf\n",time,x,y);
    return(0);
}
