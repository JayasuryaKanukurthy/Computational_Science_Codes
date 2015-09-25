clear all; clc;

Train_Set = dlmread('trainDataEvenOdd.dat');

Test_Set = dlmread('testDataEvenOdd.dat');


sv_alpha = dlmread('alpha.txt');
sv = [1:length(sv_alpha)]';
sv_y = [1*ones(500,1);-1*ones(500,1)];
support_vectors = [sv,sv_alpha,sv_y];

%Forming the kernel
Kernel = zeros(length(Train_Set));
gama = 0.05;
for(i = 1:length(Train_Set))
    xi = Train_Set(i,:);
    for (j = 1:length(Train_Set))
        xj = Train_Set(j,:);
        k = exp(-gama*norm(xi-xj,2)^2);
        Kernel(i,j) = k;
    end
end


%Calculating b for each of the vectors to check if all are equal
for(i = 1:length(support_vectors))
    summation = 0;
    for(j = 1:length(Train_Set))
        summation = summation+sv_alpha(j)*sv_y(j)*Kernel(j,support_vectors(i,1));
    end
    b = summation - support_vectors(i,3);
    B(i) = b;
end

y_result = zeros(length(support_vectors(:,1)),1);
%Testing if alpha obtained is correct
for(i = 1:length(Test_Set))
    sum = 0;
    for (j = 1:length(support_vectors))
        sum = sum + (support_vectors(j,3)*support_vectors(j,2)*(Train_Set(support_vectors(j,1),:)*Test_Set(i,:)'));
    end
    sum = sum - b;
    if(sum >= 0)
        y_result(i) = 1;
    else
        y_result(i)=-1;
    end
end
y_result=y_result';

hits = 0;
misses = 0;
for (i = 1:length(y_result))
    if(y_result(i) == sv_y(i))
        hits = hits+1;
    else
        misses = misses+1;
    end
end
display ('The Value of b for various support vectors:');
B

display ('Checking the Obtained values of Y from Test data to verify accuracy and errors')
hits
misses

Accuracy_Percent = hits/10
Error_Percent = misses/10