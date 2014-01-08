#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>
#include <math.h>

extern FILE *gf, *vf, *pf ,*p,*p1,*p2 ;
extern fpos_t pos;
extern long int ip_ordinates, ip_gridpts, nodes_num,mesh_num;
extern double *node, *phi_1, *phi_2, **vel, *pressure, **rhs, *element_area;
extern long int *mesh;
extern long int ipmax, ipmin;
extern double pmax,pmin;
void input_meshinfo(FILE *);
void input_phi(FILE *, double *phi);
void output();
