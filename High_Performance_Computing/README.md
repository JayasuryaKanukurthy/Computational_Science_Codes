CSI702-High Performance Computing
===========================================
Projects for CSI 702 - High Performance Computing

General Summary: 	

Code written to run on Parallel Linux clusters configured with OpenMP and MPI.


OpenMP: 					Simulation to find gradient and velocity vectors on a structured grid with defined potential field
							Language: C & OpenMP		OS:	Linux		Visualization:	MATLAB
					
Parallel_Poisson_Solver:	Solves Poisson solver on a 2D structured grid using simple Jacobi method. The grid is partitioned into rectangular horizontal blocks with synchronous communication schedule.
							Language: C & MPI		OS: Linux		Visualization:	MATLAB
					
Parallel_PM:				Particle-Mesh code to simulate mutual electrostatic interaction for a set of charged particles. Uses parallel poisson solver code, and uses asynchronous communications accross nodes to migrate particles accross MPI process domain boundaries
							Language: C & MPI		OS: Linux		Visualization:	MATLAB
