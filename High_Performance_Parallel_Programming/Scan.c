// MP 5 Scan
// Given a list (lst) of length n
// Output its prefix sum = {lst[0], lst[0] + lst[1], lst[0] + lst[1] + ... + lst[n-1]}
// Due Tuesday, January 22, 2013 at 11:59 p.m. PST

#include    <wb.h>

#define BLOCK_SIZE 512 //@@ You can change this

#define wbCheck(stmt) do {                                 \
        cudaError_t err = stmt;                            \
        if (err != cudaSuccess) {                          \
            wbLog(ERROR, "Failed to run stmt ", #stmt);    \
            return -1;                                     \
        }                                                  \
    } while(0)

__global__ void scan(float * input, float * output, int len, float * aux) {
    //@@ Modify the body of this function to complete the functionality of
    //@@ the scan on the device
    //@@ You may need multiple kernel calls; write your kernels before this
    //@@ function and call them from here
      
	__shared__ float scan_array[BLOCK_SIZE];
  
  	int start = blockIdx.x*blockDim.x;
  	int threadId = threadIdx.x;
	
	if(start+threadId < len)
	{
		scan_array[threadId] = input[start+threadId];
	} else {
		scan_array[threadId]= 0;
	}
	
	//Reduction Step Kernel Code
	
	int stride = 1;
	while(stride < BLOCK_SIZE)
	{
		int index = (threadId + 1)*stride*2 -1;
		if(index < BLOCK_SIZE)
		{
			scan_array[index] += scan_array[index-stride];
		}
		stride *= 2;
		
		__syncthreads();
	}
	
	//Post Reduction Step
	
	for(int stride = BLOCK_SIZE/4; stride; stride /=2)
	{
		__syncthreads();
		
		int index = (threadId+1)*stride*2 -1;
		
		if(index+stride < BLOCK_SIZE)
		{
			scan_array[index+stride] += scan_array[index];
		}
	}
	
	__syncthreads();
  
	if(start+threadId < len)	output[start+threadId] = scan_array[threadId];
  	if(aux && threadId == blockDim.x-1)	aux[blockIdx.x] = scan_array[threadId];
}

__global__ void add(float *array, float *aux, int len){
  
  int id = blockIdx.x*blockDim.x+threadIdx.x;
  
  if(blockIdx.x && id < len)
  {
    array[id] += aux[blockIdx.x-1];
  }
  
  __syncthreads();
}

int main(int argc, char ** argv) {
    wbArg_t args;
    float * hostInput; // The input 1D list
    float * hostOutput; // The output list
    float * deviceInput;
    float * deviceOutput;
    int numElements; // number of elements in the list
  	float * deviceAux;
  	float * deviceScannedAux;
  	int numBlocks;

    args = wbArg_read(argc, argv);
	
    wbTime_start(Generic, "Importing data and creating memory on host");
    hostInput = (float *) wbImport(wbArg_getInputFile(args, 0), &numElements);
    hostOutput = (float*) malloc(numElements * sizeof(float));
  	numBlocks = ceil(float(numElements)/BLOCK_SIZE);
    wbTime_stop(Generic, "Importing data and creating memory on host");

    wbLog(TRACE, "The number of input elements in the input is ", numElements);

    wbTime_start(GPU, "Allocating GPU memory.");
    wbCheck(cudaMalloc((void**)&deviceInput, numElements*sizeof(float)));
    wbCheck(cudaMalloc((void**)&deviceOutput, numElements*sizeof(float)));
  	wbCheck(cudaMalloc((void**)&deviceAux, numBlocks*sizeof(float)));
  	wbCheck(cudaMalloc((void **) &deviceScannedAux, numBlocks*sizeof(float)));
    wbTime_stop(GPU, "Allocating GPU memory.");

    wbTime_start(GPU, "Clearing output memory.");
    wbCheck(cudaMemset(deviceOutput, 0, numElements*sizeof(float)));
  	wbCheck(cudaMemset(deviceAux, 0, numBlocks*sizeof(float)));
  	wbCheck(cudaMemset(deviceScannedAux, 0, numBlocks*sizeof(float)));
    wbTime_stop(GPU, "Clearing output memory.");

    wbTime_start(GPU, "Copying input memory to the GPU.");
    wbCheck(cudaMemcpy(deviceInput, hostInput, numElements*sizeof(float), cudaMemcpyHostToDevice));
    wbTime_stop(GPU, "Copying input memory to the GPU.");

    //@@ Initialize the grid and block dimensions here
    dim3 GridDim(numBlocks,1,1);
    dim3 BlockDim(BLOCK_SIZE,1,1);

    wbTime_start(Compute, "Performing CUDA computation");
    //@@ Modify this to complete the functionality of the scan
    //@@ on the deivce
    
    scan<<<GridDim,BlockDim>>> (deviceInput,deviceOutput,numElements,deviceAux);
  	cudaDeviceSynchronize();
  	scan<<<GridDim, BlockDim>>>(deviceAux, deviceScannedAux, numBlocks, NULL);
  
	add <<<GridDim, BlockDim>>>(deviceOutput, deviceScannedAux, numElements);

    cudaDeviceSynchronize();
    wbTime_stop(Compute, "Performing CUDA computation");

    wbTime_start(Copy, "Copying output memory to the CPU");
    wbCheck(cudaMemcpy(hostOutput, deviceOutput, numElements*sizeof(float), cudaMemcpyDeviceToHost));
    wbTime_stop(Copy, "Copying output memory to the CPU");

    wbTime_start(GPU, "Freeing GPU Memory");
    cudaFree(deviceInput);
    cudaFree(deviceOutput);
    wbTime_stop(GPU, "Freeing GPU Memory");

    wbSolution(args, hostOutput, numElements);

    free(hostInput);
    free(hostOutput);

    return 0;
}
