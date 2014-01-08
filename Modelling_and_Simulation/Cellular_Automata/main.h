#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

FILE* gf, *vf, *pf, *p,*p1,*p2 ;  //grid file, velocity (output) file, pressure (output) file
long int ip_ordinates=0, ip_gridpts=0, nodes_num=0,mesh_num=0, rand_st=0;
double *node,*phi_1, *phi_2, *vel, *pressure,p0 = 0.0,rho=1, maxp, minp;
double *element_area;        //*global_matrix
long int *mesh,*element_status, **neighbours;
int check_sides(long int, long int);
void calculate_area();
void initialize();
void find_neighbours();
void switch_element_status(long int);
extern void input_meshinfo(FILE *);
extern void output(int);
