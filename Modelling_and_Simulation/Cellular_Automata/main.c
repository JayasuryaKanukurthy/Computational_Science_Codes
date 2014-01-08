#include "main.h"
#include "io.h"

int main(int argc, char** argv)
{
    float probability_limit = atof(argv[1]);
    long int i,j;
    long int k,neigh;
    gf =  fopen("face","r");
    double affected_area=0,probability;

    input_meshinfo(gf);

    initialize();

    calculate_area();

    find_neighbours();
    printf("Running Simulation Rules\n");
    output(0);
    for(i=0; i<40;i++)
    {
        for(j=0;j<mesh_num;j++)
        {
            affected_area=0.0;
            for(neigh=0,k=0;k<3;k++)
            {
                if(element_status[neighbours[j][k]] > 0)
                {
                    affected_area += element_area[neighbours[j][k]];        //Calculating the area of affected surrounding cells
                    neigh ++;
                }
            }
                probability = 0.2 * ((affected_area)/(element_area[j]+affected_area));
//                printf("%d %lf %lf %lf\n", j, affected_area, element_area[j], probability);

                if(probability >= probability_limit)
                {
                    switch_element_status(j);
                }
                else
                {
                    if(neigh == 3)
                        switch_element_status(j);
                }
        }
        output(i+1);
    }
    fclose(gf);

    return(0);

}
void switch_element_status(long int i)
{
    switch(element_status[i])
    {
        case 0:
            element_status[i] = 1;
            break;
        case 1:
            element_status[i] = 2;
            break;
        case 2:
            element_status[i] = 3;
            break;
        case 3:
            element_status[i] = 4;
            break;
        default:
            break;
    }

}

void calculate_area()
{

    double area, prod_i,prod_j,prod_k,vec1[3],vec2[3],gm[3][3];
    long int i,j,v1,v2,v3;

    for (i=0;i<mesh_num;i++)        //Calculating area for each of the mesh element
    {
        v1 = mesh[i*3]-1;          //mesh[i]-1 because the mesh numbering starts from 1 where as the array starts from 0
        v2 = mesh[i*3+1]-1;
        v3 = mesh[i*3+2]-1;

//        printf("%d - %lf %lf %lf\n",v1+1, node[v1],node[v1+1],node[v1+2]);
//Calculating vec1 = |A-B|
        vec1[0]=node[v1]-node[v2];          //x-coord of vector1 of cross product = (v1_x - v2_x)
        vec1[1]=node[v1+1]-node[v2+1];      //Y-coord of vector1 of cross product = (v1_y - v2_y)
        vec1[2]=node[v1+2]-node[v2+2];      //similarly

//Calculating vec2 = |A-C|
        vec2[0]=node[v1]-node[v3];          //x-coord of vector2 of cross product = (v1_x - v3_x)
        vec2[1]=node[v1+1]-node[v3+1];      //Y-coord of vector1 of cross product = (v1_y - v3_y)
        vec2[2]=node[v1+2]-node[v3+2];      //similarly

//Performing cross product of vec1 and vec2

        prod_i = (vec1[1]*vec2[2]-(vec2[1]*vec1[2]));
        prod_j = (vec1[0]*vec2[2]-(vec2[0]*vec1[2]));
        prod_k = (vec1[0]*vec2[1]-(vec2[0]*vec1[1]));

        prod_i = prod_i*prod_i;                    //X^2 for finding area = 1/2 * |A X B|
        prod_j = prod_j*prod_j;
        prod_k = prod_k*prod_k;

        area = (prod_i+prod_j+prod_k);              // (X^2+Y^2+Z^2)
        area = 0.5*sqrt(area);                      //Cross product area
        element_area[i] = area;


    }
}

void initialize()
{
    int i,j;

    element_area= (double *)calloc(mesh_num,sizeof(double));
    element_status=(long int *)calloc(mesh_num,sizeof(long int));
    neighbours = (long int **)calloc(mesh_num,sizeof(long int *));

    for(i=0;i<mesh_num;i++)
    {
        neighbours[i] = (long int *)calloc(3,sizeof(long int));
    }

        element_status[1] =1;
        element_status[2] =1;
        element_status[3] =1;
    /*
    //Randomly assigning initially status for clusters of cells
    for(i=0,j=5;i<mesh_num/2;)
    {
        j=10;
        while(j>0)
        {
            rand_st = (rand()%10)+i;
            element_status[rand_st] = 1;                        //setting random elements as cancer cells
//            printf("%ld %ld\n",rand_st,element_status[rand_st]);
            j--;
        }
        i = i+100;
    }*/
}

void find_neighbours()
{
    int i,j,k,l, var1, var2;
    long int n1,n2,n3;

    printf("Finding neighbouring elements of each element by brute force search\n");
    //Finging neighbouring Elements : Brute Force Search
    for(i=0;i<mesh_num;i++)
    {

        for(j=0,k=0;j<mesh_num && k<3;j++)
        {
            if(check_sides(i,j)==1)
            {
                if(mesh[i*3] == mesh[j*3] && mesh[i*3+1] == mesh[j*3+1] && mesh[i*3+2] == mesh[j*3+2])
                    continue;
                else
                {
                    neighbours[i][k] = j;                       //neighbour is the jth element
                    k++;
                }

            }
            else
            {
                continue;
            }
        }
    }
}

int check_sides(long int m1, long int m2)
{
    int i,j,k;
    int var1=0;
    int var2=0;
    for(i=0;i<3;i++)
    {
        for(j=0;j<3;j++)
        {
            if((mesh[m1*3+i] == mesh[m2*3+j]))
            {
                if(var1 == 0)
                    var1 = 1;
                else
                    return 1;
                                                        //First matching node
            }
            else
                continue;
        }
    }
    return 0;
}
