#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define GRID_SIZE 100

double get_temperature(int, int), vel_x(int, int), vel_y(int,int);
int check_convergence(double);

/* Defining Grid Dimensions and Information*/

double T[GRID_SIZE][GRID_SIZE];                             //Stores Temperature at grid points
double T_new[GRID_SIZE][GRID_SIZE];
double residue[GRID_SIZE][GRID_SIZE];
int lx,ly;                                                                  //Stores the lengths of the tissue
int n_x=GRID_SIZE/3,n_y=GRID_SIZE/2,n_width_x=2, n_width_y=2;                               //Size of the needle
int blood_vessel_x,blood_vessel_y, b,a;                             //Start of the blood vessel in y co-ordinate and its width

//Defining Temperature constants
double k = 0.5, t0, t_needle, t_tresh, percent_tissue_burnt=0.0, residue_norm=0.0,max_residue=0.0;

//Defining time steps for time, x and y
double x_step = 1, y_step = 1, t_step = 0.05,time=0,range=30;

double v =0.15, vmax = 0.40;                              //Maximum Blood Velocity in the vein (m/s !?!)
int i,j,x,y,max_diff, checking_convergence=0;
