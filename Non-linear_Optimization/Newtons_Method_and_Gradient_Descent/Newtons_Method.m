clear all; clc;
x0 = [1 1]';
grad = Gradient(x0);
x_step = x0;
eta = exp(-7);
n=0;
lambda = 0.001;
while norm(grad,2)>= 0.001
    steps = (Hessian(x_step)+lambda*eye(length(x_step)))\(-Gradient(x_step))
    alpha = 1;
    while F(x_step+alpha*steps)-F(x_step)> eta*alpha*Gradient(steps)'*steps
        alpha = alpha/2;
    end
    F(x_step)
    x_step = x_step+alpha*steps;
    grad = Gradient(x_step);
    n = n+1;
end
