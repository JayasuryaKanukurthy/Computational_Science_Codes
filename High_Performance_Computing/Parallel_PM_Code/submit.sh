#!/bin/csh
#PBS -j oe
#PBS -N csi702-hwk6
#PBS -l select=10:ncpus=1:mpiprocs=1
#PBS -l walltime=48:00:00

limit stacksize unlimited

setenv MACHTYPE x86-suse-linux
setenv TERM xterm
setenv MPI_BUFFER_MAX 2000000
setenv MPI_BUFS_PER_PROC 1024
setenv DAPL_MAX_CM_RESPONSE_TIME 22
setenv I_MPI_DEVICE rdssm
setenv I_MPI_DAPL_PROVIDER OpenIB-cma
setenv I_MPI_PIN_PROCS allcores
setenv I_MPI_PIN_MODE lib
setenv I_MPI_DEVICE_FALLBACK 0

echo "The nodefile is:"
cat $PBS_NODEFILE

set pth = "$HOME/hwk6/"
set exe = "$pth/main"
cd $pth

setenv PROCS `cat $PBS_NODEFILE | wc -l`
setenv NODES `cat $PBS_NODEFILE | sort -u | wc -l`
echo "Running on $PROCS processors"
echo "Working in directory: $PWD" 

mpdboot -r ssh -f $PBS_NODEFILE -n $NODES
mpiexec -np 5 $exe > output 
mpdallexit
