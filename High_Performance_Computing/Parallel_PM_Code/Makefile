all:
	mpiicc -o main parallel_pm.c
	qsub submit.sh
	#mpirun -np 5 ./main
	matlab -nodesktop -nosplash -r visualize
	
clean:
	rm -f ./main ./core ./output ./csi702-hwk6.*

