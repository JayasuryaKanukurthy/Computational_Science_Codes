#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

FILE* gf, *vf, *pf, *p,*p1,*p2 ;  //grid file, velocity (output) file, pressure (output) file
fpos_t pos;
long int ip_ordinates=0, ip_gridpts=0, nodes_num=0,mesh_num=0, maxi=0, mini=0;
double *node,*phi_1, *phi_2, **vel, *pressure,p0 = 0.0,rho=12, maxp, minp,**rhs,**gradient;
double *global_matrix,*lumped_mass_matrix,*element_area;        //*global_matrix
long int *mesh;
long int ipmax=0, ipmin=0;
double pmax=0,pmin=2000000;

void calculate_area(),scatter_add(double, long int, long int, long int);                      //Calculate Area of each element and form the global matrix
void initialize(),calculate_gradient();
extern void input_meshinfo(FILE *);
extern void input_phi(FILE *, double *phi);
extern void output();
