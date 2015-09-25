reset;
param n := 785;
param D := 1000;
param C := 100;

set POINTS := {2..n};
set INPUT_POINTS := {1..n};
set DATASET := {1..D};

param x{DATASET,INPUT_POINTS};
param y{DATASET};
#param K{DATASET,DATASET};

data 3and6_Train_set.txt;

param K{i in DATASET, j in DATASET} := sum {t in POINTS} x[i,t]*x[j,t];

#Initializing Y[i]
for {i in DATASET} {

	if x[i,1] = 3 then {let y[i] := 1} else{ let y[i] := -1};

}


var alpha{DATASET} >= 0, <= C;	# 

maximize DUALSVM : (sum {i in DATASET} alpha[i]) - 0.5*(sum {i in DATASET,j in DATASET} alpha[i]*alpha[j]*y[i]*y[j]*K[i,j]);

s.t. CONDITION :  sum {i in DATASET} alpha[i]*y[i] = 0;

option solver snopt;
solve;
