#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "headers/input_output.h"

//#include "headers/numerical_methods.h"
//#include "headers/functions.h"

void driver();
double runge_kutta(double x_var, double t_var, double h);
double function(double x_var, double t_var);
double reinforcement_f(double h), reinforcement_g(double h);

double x,y,h, a,b,x_init,y_init;
int range, x_or_y, model, periodic_reinforcements_flag;

int main(int argc, char **argv)
{

	if(initialize_values(&x, &y, &h,&a, &b, &range, &model))
		exit(EXIT_FAILURE);

	x_init = x;								/* Saving the initial values of x and y */
	y_init = y;

//    filename = "model_plot_data_non_periodic";

	if(create_file())
        exit(EXIT_FAILURE);

    driver();

	printf("\n\nPlease find output in the \"outputs\" folder\n\n");
	return(0);
}

void driver()
{
	double xn1 = x;
	double yn1 = y;
	double time = 0.0;

	if(output_to_file(time, x_init, y_init))				//Writing initial values to output file
		exit(EXIT_FAILURE);

	for (time = h; time <= range && x >0 && y>0; time += h)
	{
   	        x_or_y = 1;
		xn1 = runge_kutta(x,time, h);
		x_or_y = 0;
		yn1 = runge_kutta(y,time, h);

		if(output_to_file(time, xn1, yn1))
            exit(EXIT_FAILURE);

		x = xn1;
		y = yn1;
	}

}


double runge_kutta(double x_var, double t_var, double h)
{
	double k1,k2,k3,k4;
	double yn1 = 0.0;
	k1 = h*function(t_var,x_var);
	k2 = h*function((t_var+h/2), (x_var+k1/2));
	k3 = h*function((t_var+h/2), (x_var+k2/2));
	k4 = h*function(t_var+h, x_var+k3);
	yn1 = x_var + (k1)/6 + (k2)/3 + (k3)/3 + (k4)/6;

	return(yn1);
}

double function(double t_var, double x_var)
{
	switch (model)						
	{
	case 1:								//Case 1: Conventional Model

	    if(x_or_y)
	    {
	        return(-a*y+reinforcement_f(t_var));
	    }
	    else
	    {
	        return(-b*x+reinforcement_g(t_var));
	    }
	    break;
	case 2:								//Case 2: Guerilla Model
	    if(x_or_y)
	    {
	        return(-a*x*y+reinforcement_f(t_var));
	    }
	    else
	    {
	        return(-b*x+reinforcement_g(t_var));
	    }
	    break;
	 default:
	    break;
	 }
	 return(0);
}

double reinforcement_f(double time)
{
//	if(time/0.005 ==0)
//		return(100);
	return(100);
}
double reinforcement_g(double time)
{
	if((time/0.005) ==0)
		return(100);
	return (1);	
}

