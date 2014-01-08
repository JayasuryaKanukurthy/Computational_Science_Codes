#include "main.h"
#include "io.h"

int main(int argc, char** argv)
{
    char* grid_filename = argv[1];
    int i, multiple_phi=1,j, checkflag=0;
    gf =  fopen(grid_filename,"r");


    input_meshinfo(gf);

    initialize();
    p = fopen(argv[3],"r");

    if(p == NULL)
    {
        input_phi(fopen(argv[2],"r"),phi_1);
    }
    else
    {

        input_phi(fopen(argv[2],"r"),phi_1);
        input_phi(p,phi_2);
    }

    calculate_area();

    calculate_gradient();

//Calculating Inverse of lumped mass matrix
    for(i = 0; i<nodes_num;i++)
    {
        lumped_mass_matrix[i] = 1.0/lumped_mass_matrix[i];
    }

    //Finding v

    for(i=0;i<nodes_num;i++)
    {
        vel[i][0] = lumped_mass_matrix[i]*rhs[i][0];
        vel[i][1] = lumped_mass_matrix[i]*rhs[i][1];

        pressure[i] = rho*0.5*sqrt(vel[i][0]*vel[i][0]+vel[i][1]*vel[i][1]);

        if(pmax<pressure[i])
        {
            pmax = pressure[i];
            ipmax = i;
        }
        else if(pmin > pressure[i])
        {
            pmin = pressure[i];
            ipmax = i;
        }
        else
            continue;


    }
    printf("\n\nMaximum pressure is at node: %ld of magnitude: %lf\n",ipmax+1,pmax);
    printf("Minimum pressure is at node: %ld of magnitude: %lf\n\n",ipmin+1,pmin);


    output();

    fclose(gf);

    return(0);

}


void initialize()
{
    int i;

    phi_1 = (double *)calloc(nodes_num,sizeof(double));
    phi_2 = (double *)calloc(nodes_num,sizeof(double));

    lumped_mass_matrix = (double *)calloc(nodes_num,sizeof(double));
    global_matrix = (double *)calloc(nodes_num,sizeof(double));
    element_area = (double *)calloc(mesh_num,sizeof(double));

    rhs = (double **)calloc(nodes_num,sizeof(double *));
    vel = (double **)calloc(nodes_num,sizeof(double *));
    pressure = (double *)calloc(nodes_num,sizeof(double));
    gradient = (double **)calloc(nodes_num,sizeof(double *));

    for(i=0; i<nodes_num;i++)
    {
        rhs[i] = (double *)calloc(2,sizeof(double));
        vel[i] = (double *)calloc(2, sizeof(double));
        gradient[i] = (double *)calloc(2,sizeof(double));
    }

}

void calculate_area()
{

    double area, prod_i,prod_j,prod_k,vec1[3],vec2[3],gm[3][3];
    long int i,j,v1,v2,v3;

    for (i=0;i<mesh_num;i++)        //Calculating area for each of the mesh element
    {
        v1 = mesh[i*3];          //mesh[i]-1 because the mesh numbering starts from 1 where as the array starts from 0
        v2 = mesh[i*3+1];
        v3 = mesh[i*3+2];
        v1 -=1;
        v2 -=1;
        v3 -=1;
        //
        area = ((node[v2*3+0]-node[v1*3+0])*(node[v3*3+1]-node[v1*3+1])) - ((node[v3*3+0]-node[v1*3+0])*(node[v2*3+1]-node[v1*3+1]))/2;                      //Cross product area
        if(area<0)
            area = -area;
        element_area[i] = area;
//        printf("%ld %ld %ld\n%lf %lf %lf %lf\n%lf\n",i,v1,v2,node[v1*3],node[v1*3+1],node[v2*3],node[v2*3+1],area);

        lumped_mass_matrix[v1]+= area/3;
        lumped_mass_matrix[v2]+= area/3;
        lumped_mass_matrix[v3]+= area/3;

    }
}
void calculate_gradient()
{
    long int i, v1, v2, v3;
    double sigmax,sigmay;

    for (i=0;i<mesh_num;i++)        //Calculating area for each of the mesh element
    {
        v1 = mesh[i*3]-1;          //mesh[i]-1 because the mesh numbering starts from 1 where as the array starts from 0
        v2 = mesh[i*3+1]-1;
        v3 = mesh[i*3+2]-1;

    //Finding Gradient in x direction for nodes v1, v2, v3 i.e., dN/dx
        gradient[v1][0] = node[v2*3+1]-node[v3*3+1];
        gradient[v2][0] = node[v3*3+1]-node[v1*3+1];
        gradient[v3][0] = node[v1*3+1]-node[v2*3+1];

    //Finding Gradient in y direction for nodes v1,v2,v3 = dN/dy
        gradient[v1][1] = node[v3*3]-node[v2*3];
        gradient[v2][1] = node[v1*3]-node[v3*3];
        gradient[v3][1] = node[v2*3]-node[v1*3];

    //Calulating rhs in x and y directions

        /*phi[v1][0] += (gradient[v1][0]*phi_1[v1]+gradient[v1][0]*phi_2[v1])/(2*element_area[i]);
        phi[v2][0] += (gradient[v2][0]*phi_1[v2]+gradient[v2][0]*phi_2[v2])/(2*element_area[i]);
        phi[v3][0] += (gradient[v3][0]*phi_1[v3]+gradient[v3][0]*phi_2[v3])/(2*element_area[i]);
        phi[v1][1] += (gradient[v1][1]*phi_1[v1]+gradient[v1][1]*phi_2[v1])/(2*element_area[i]);
        phi[v2][1] += (gradient[v2][1]*phi_1[v2]+gradient[v2][1]*phi_2[v2])/(2*element_area[i]);
        phi[v3][1] += (gradient[v3][1]*phi_1[v3]+gradient[v3][1]*phi_2[v3])/(2*element_area[i]);*/

        sigmax = gradient[v1][0]*(phi_1[v1]+phi_2[v1])+gradient[v2][0]*(phi_1[v2]+phi_2[v2])+gradient[v3][0]*(phi_1[v3]+phi_2[v3]);
        sigmay = gradient[v1][1]*(phi_1[v1]+phi_2[v2])+gradient[v2][1]*(phi_1[v2]+phi_2[v2])+gradient[v3][1]*(phi_1[v3]+phi_2[v3]);

        rhs[v1][0] += sigmax/(element_area[i]*2);
        rhs[v2][0] += sigmax/(element_area[i]*2);
        rhs[v3][0] += sigmax/(element_area[i]*2);

        rhs[v1][1] += sigmay/(element_area[i]*2);
        rhs[v2][1] += sigmay/(element_area[i]*2);
        rhs[v3][1] += sigmay/(element_area[i]*2);

    }
}
