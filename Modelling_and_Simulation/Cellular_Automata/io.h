#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <string.h>

extern FILE *gf, *vf, *pf ,*p,*p1,*p2 ;
extern long int ip_ordinates, ip_gridpts, nodes_num,mesh_num;
extern double *node, *element_area;
extern long int *mesh, *element_status;
void input_meshinfo(FILE *);
void output(int);
