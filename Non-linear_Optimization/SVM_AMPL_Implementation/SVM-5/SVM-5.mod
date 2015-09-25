reset;
param n := 784;
param D := 1000;
param C := 100;
param gama := 0.05;


set POINTS := {1..n};
set INPUT_POINTS := {1..n};
set DATASET := {1..D};

param x{DATASET,INPUT_POINTS};
param y{DATASET};

data Train_Set_Full.dat;


#Initializing KERNEL Function: Radial-basis function machine with K[i,j] = e^(-g*(x[j]-x[i])^2)
param K{i in DATASET, j in DATASET} := exp (-gama*(sum{t in POINTS} (x[i,t]-x[j,t])^2));

var alpha{DATASET} >= 0, <= C;

maximize DUALSVM : sum {i in DATASET} alpha[i] - 0.5*sum {i in DATASET,j in DATASET} alpha[i]*alpha[j]*y[i]*y[j]*K[i,j];

s.t. CONDITION :  sum {i in DATASET} alpha[i]*y[i] = 0;

option solver snopt;
solve;
