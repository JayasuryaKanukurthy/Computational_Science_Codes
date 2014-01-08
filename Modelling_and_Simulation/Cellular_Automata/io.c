#include "io.h"

void input_meshinfo(FILE* file)
{
    char points[] = "POINTS\n";
    char mp[] = "mesh\n";
    char end[]="END\n";
    char temp[128];

    char *str = (char*)calloc(128,sizeof(char));
    char *cords;
    int index = 0,flag=1,count = 0,i=0;
    int meshpts=0;
    double fcords=0;
    FILE *points4matlab = fopen("output/nodes.dat","w+");
    FILE *mesh4matlab = fopen("output/mesh.dat","w+");

	//Find the beginning of "POINTS"
    while(!feof(file)&&flag)
    {
		fgets(str,127,file);
		if(strcmp(str,points) == 0)
		{
			flag = 0;
		}
	}
	//initialize nodes array
	fgets(str,127,file);
	nodes_num = atoi(str);
	node = (double *)calloc(nodes_num*3,sizeof(double));

	//Read the co-ordinates
	for(count = nodes_num,index=0;(count>0&&strcmp(str,end)!=0); count--,index++)
	{
		fgets(str,127,file);
        cords = strtok(str," ");
		for(i=0;i<3;i++)
		{
			fcords = atof(cords);
			node[index*3+i] = fcords;
			cords = strtok(NULL," ");
		}
		fprintf(points4matlab,"%e %e %e\n",node[index*3],node[index*3+1],node[index*3+2]);
	}
	flag = 1;

	//Find the start of "mesh" points
	while(!feof(file)&&flag)
    {
		fgets(str,127,file);
		if(strcmp(str,mp) == 0)
		{
			flag = 0;
		}
	}

	//initialize mesh array
	fgets(str,127,file);
	mesh_num = atoi(str);
	mesh = (long int *)calloc(mesh_num*3,sizeof(long int));

	//Read the mesh points connectivity
	for(count = mesh_num,index=0;(count>0&&strcmp(str,end)!=0); count--,index++)
	{
		fgets(str,127,file);
		cords = strtok(str," ");
		for(i=0;i<3&&cords!=NULL;i++)
		{
			meshpts = atoi(cords);
			mesh[index*3+i] = meshpts;
			cords = strtok(NULL," ");
        }
        fprintf(mesh4matlab,"%ld %ld %ld\n",mesh[index*3],mesh[index*3+1],mesh[index*3+2]);
    }
    printf("Read %ld nodes and %ld mesh points\n",nodes_num,mesh_num);

}


void output(int num)
{
    int i=0;
    char filename[50];
    sprintf(filename, "output/status%d.dat",num);
 //   printf("%s\n",filename);
    FILE *status = fopen(filename,"w+");

    for(i=0;i < mesh_num;i++)
        fprintf(status,"%ld\n",element_status[i]);
}

