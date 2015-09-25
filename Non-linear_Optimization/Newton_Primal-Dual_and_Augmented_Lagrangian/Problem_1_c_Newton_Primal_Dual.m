% Primal-Dual Newton Method Implementation of Problem 1 (c)
clear all; clc;
n=0;
x1 = 0.1;
x2 = 0.2;
x3 = 0.5;
x4 = 0.8;
y1 = -0.2;
y2 = 0.1;


del_x1=0;
del_x2=0;
del_x3=0;
del_x4=0;
del_y1=0;
del_y2=0;

epsilon = 0.001;
lambda = 2;
beta = 0.12;

grad = [2*x1, 2*x2, 2*x3, 2*x4;1, 1, 2, 3];

while(norm(grad,2) >= epsilon)
    RHS = [ -9*x1^2+2*x1*y1+y2;
            -6*x2+2*x2*y1+y2;
            -3*x3^2+2*x3*y1+2*y2;
            -3*x4+2*x4*y1+3*y2;
            -x1^2-x2^2-x3^2-x4^2+4;
            -x1-x2-2*x3-3*x4+1];

    Del2_L = [  18*x1-2*y1  0   0   0;
                0   12*x1-2*y1  0   0;
                0   0   6*x3-2*y1   0;
                0   0   0   6*x4-2*y1];


    while all(eig(Del2_L)<=0)           %Regularization
        Del2_L = Del2_L + lambda * eye(length(Del2_L));
    end
    jacobian_g = [2*x1, 2*x2, 2*x3, 2*x4;1, 1, 2, 3];
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
    del_x3 = result(3);
    del_x4 = result(4);
    del_y1 = result(5);
    del_y2 = result(6);

    x1 = x1+del_x1;
    x2 = x2+del_x2;
    x3 = x3+del_x3;
    x4 = x4+del_x4;
    y1 = y1 + del_y1;
    y2 = y2 + del_y2;

    grad = [x1^2+x2^2+x3^2+x4^2-4;x1+x2+2*x3+3*x4-1];
    n = n+1;
    fprintf('Iteration= %d; x1 = %f; x2 = %f; x3 = %f; x4 = %f; Function = %f; Constraint1 = %f; Constraint2 = %f\n', n, x1, x2, x3, x4, 3*x1^3+2*x2^3+x3^3+x4^3, (x1^2+x2^2+x3^2+x4^2-4),(x1+x2+2*x3+3*x4-1));
end
output_args = [x1;x2];
