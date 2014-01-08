all: main.c input_output.c
	gcc -g -o main *.c -lm
	./main < inputs
	mv time* ./output
clean:
	rm -f ./output/time*

