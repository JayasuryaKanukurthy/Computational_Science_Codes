#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GRID_SIZE 100

void output_temperature(int), calculate_timesteps(), input_data();

/* Defining Grid Dimensions and Information*/

extern double T[GRID_SIZE][GRID_SIZE];                             //Stores Temperature at grid points
extern double T_new[GRID_SIZE][GRID_SIZE];
extern double residue[GRID_SIZE][GRID_SIZE];
extern int lx,ly;                        //Stores start of needle (x,y) on grid
extern int n_x,n_y,n_width_x, n_width_y;                               //Size of the needle
extern int blood_vessel_x,blood_vessel_y, b,a;                             //Start of the blood vessel in y co-ordinate and its width

//Defining Temperature constants
extern double k, t0, t_needle, t_tresh,percent_tissue_burnt,tissue_burnt;

//Defining time steps for time, x and y
extern double x_step, y_step, t_step,time,range;

extern double v, vmax;                              //Maximum Blood Velocity in the vein (m/s !?!)
extern int i,j,x,y;

