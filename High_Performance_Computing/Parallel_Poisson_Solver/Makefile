all:
	mpiicc -o main parallel_poisson.c
#	mpirun -np 4 ./main > output
	qsub submit.sh
	matlab -nodiplay -nosplash -r visualize.m
clean:
	rm -f ./main ./core ./output ./csi702-hwk5.*

