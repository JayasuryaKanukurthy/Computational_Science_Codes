all: simulation_graph.png

simulation_graph.png: model_plot_data main

	gnuplot gnuplot_commands

model_plot_data: main user_inputs
	./main < user_inputs

main: input_output.c main.c main_periodic.c
	gcc -o main main.c -lm i_o
	gcc -o main_periodic main_periodic.c -lm i_o

input_output.c: headers/input_output.h
	gcc -c -o i_o input_output.c
clean:
	rm -f main* i_o
	rm -f outputs/*

