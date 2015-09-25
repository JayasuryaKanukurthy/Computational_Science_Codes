param x_test{DATASET,INPUT_POINTS};
param y_test{DATASET};

param test{DATASET};

param hit3; param miss3;
param hit6; param miss6;

param total_hits; param total_misses;


data 3and6_Test_set.txt;

let hit3 := 0; let miss3 := 0;
let hit6:= 0; let miss6 := 0;
let total_hits :=0; let total_misses:=0;

#Checking number of misses for digit 3
for {i in {1..500}} {

	#Finding where the point is placed on the hyperplane
	let test[i] := sum {j in POINTS} (x_test[i,j]*w[j]) - b;
	if (test[i] >= 0) then 
	{
		let y_test[i] := 1; 
		let hit3 := hit3+1
	} 
	else 
	{
		let y_test[i] := -1; 
		let miss3 := miss3+1
	};
};

#Checking number of misses for digit 6
for {i in {501..1000}} {

	#Finding where the point is placed on the hyperplane
	let test[i] := sum {j in POINTS} (x_test[i,j]*w[j]) - b;
	if (test[i] >= 0) then 
	{
		let y_test[i] := 1; 
		let miss6 := miss6+1
	} 
	else 
	{
		let y_test[i] := -1; 
		let hit6 := hit6+1
	};
};

#Calculating Accuracy and Errors
for {i in DATASET}
{
	if(test[i] >= 0 and x_test[i,1] == 3) then
	{
		let total_hits:=total_hits+1;
	}
	else if(test[i] < 0 and x_test[i,1] == 6) then
	{
		let total_hits := total_hits+1;
	}
	else
	{
		let total_misses := total_misses + 1;
	};
};

display y_test;
		
display hit3, miss3, hit6, miss6;
display total_hits, total_misses;

param Accuracy_Percent := (total_hits)/10.0;
param Error_Percent := (total_misses)/10.0;

display Accuracy_Percent;
display Error_Percent;

