all:
	gcc -g -o main *.c -lm
	./main data/grid3 data/phi3.1
	matlab -r "asign3"	
clean:
	rm -f main output/*.dat output/velocity output/gradient output/pressure
	
