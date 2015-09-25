% Primal-Dual Newton Method Implementation of Problem 1 
clear all; clc;
n=0;
x1 = 1;
x2 = 2;
y1 = 0.3;

del_x1=0;
del_x2=0;
del_y1=0;

epsilon = 0.001;
lambda = 2;
beta = 0.12;

jacobian_g = [2*x1, 2*x2];

while(norm(jacobian_g,2) >= epsilon)
    RHS = [-2+2*x1*y1; 3 + 2*x2*y1; -x1^2-x2^2+25];
    Del2_L = [-2*y1 , 0; 0, -2*y1];
    while all(eig(Del2_L)<=0)           %Regularization
        Del2_L = Del2_L + lambda * eye(length(Del2_L));
    end
    jacobian_g = [2*x1, 2*x2];
    LHS = [Del2_L, -jacobian_g'];
    [rows,cols] = size(jacobian_g);                 %Used to check if grad_g has full rank
    leng = length(LHS);
    if (rank(jacobian_g) < min(rows,cols))          %If no full rank, Secondary Regularization
        last_row = [jacobian_g, beta*eye(length(jacobian_g(1,:)))];
        LHS = [LHS;last_row];
    else
        last_row = [jacobian_g, zeros(rows,leng-cols)];
        LHS = [LHS;last_row];
    end

    result = LHS\RHS;

    del_x1 = result(1);
    del_x2 = result(2);
    del_y1 = result(3);

    x1 = x1+del_x1;
    x2 = x2+del_x2;
    y1 = y1 + del_y1;

    jacobian_g = [2*del_x1, 2*del_x2];          %Calculate new Jacobian of g for finding the norm
    n = n+1;
    fprintf('Iteration= %d; x1 = %f; x2 = %f; Function = %f; Constraint = %f\n', n, x1, x2,2*x2-3*x2, x1^2 + x2^2 - 25);
end
output_args = [x1;x2];
