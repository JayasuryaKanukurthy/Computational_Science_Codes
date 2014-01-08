/*---------------- File contains declarations of: -----------------------------
	* Functions to obtain initial values.
	* Functions to interact with the user.
	* Functions to manage output.

Function definitions are implemented in initialize.c file in the main folder.
-------------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>

int initialize_values(double *x, double *y, double *h, double *a, double *b,int *range, int *model);
int create_file();
int output_to_file(double time, double x, double y);
