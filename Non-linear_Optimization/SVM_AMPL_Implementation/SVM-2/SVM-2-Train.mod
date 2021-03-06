param n := 784;
param D := 1000;
param C := 100;

set POINTS := {1..n};
set INPUT_POINTS := {1..n};
set DATASET := {1..D};

param x{DATASET,INPUT_POINTS};
param y{DATASET};

data 3and6_Train_set.dat;

#Setting the value of y: 1 if digit is 3 and -1 for 6
#for {i in DATASET} {

#	if x[i,1] = 3 then {let y[i] := 1} else{ let y[i] := -1};

#}

var w{POINTS};	
var b;
var eta{DATASET} >= 0;

minimize SVM : 0.5* sum{j in POINTS} (w[j]*w[j])+(C*sum {i in DATASET} eta[i]);

s.t. CONDITION {i in DATASET}: y[i]*( (sum {j in POINTS} (x[i,j]*w[j]))-b) >= 1-eta[i];

option solver snopt;
solve;
printf {i in POINTS} "%f\n", w[i] >> output.txt;
display b;