all:
	gcc -g -openmp -o main main.c -lm
	./main 
	matlab -nodesktop -nosplash -r visualize
	mv Field_Visual.jpg ./output/

clean:
	rm -f main matlab_inputs 
	rm -f output/*

