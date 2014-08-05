#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <omp.h>

#define GRID_SIZE 100
#define ABS(X) ((X)<0? -(X):(X))

void input();
double grad_x[GRID_SIZE][GRID_SIZE];
double grad_y[GRID_SIZE][GRID_SIZE];
double gradient[GRID_SIZE][GRID_SIZE];
double grid_x[GRID_SIZE][GRID_SIZE];
double grid_y[GRID_SIZE][GRID_SIZE];
double x,y,phi1,phi2,flagx,flagy;
double phi(double x,double y);
int a = 10, b = 10;
double boundary_value = 0;

double xmin = 0, xmax = 0;		
double ymin = 0, ymax = 0;	
int Grid_Size = 100;
double nx = 0,ny = 0;
double  grad_max=0,grad=0;
int i=0,j=0;
char* out_grad_x = "output/gradient_x";
char* out_grad_y = "output/gradient_y";
char* out_mesh_x = "output/mesh_x";
char* out_mesh_y = "output/mesh_y";
char* out_grad = "output/grad";
char* inputs = "inputs";

FILE *fp_gradx, *fp_grady, *fp_meshx, *fp_meshy, *fp_inputs, *fp_grad;
