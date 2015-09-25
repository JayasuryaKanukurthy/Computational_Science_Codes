clear all; clc;
x0 = [1 1]';
grad = Gradient(x0);
x_step = x0;
eta = exp(-7);
n=0;
while norm(grad,2)>= 0.01
    step = -Gradient(x_step);
    alpha = 1;
    while F(x_step+alpha*step)-F(x_step)> eta*alpha*Gradient(step)'*x_step
        alpha = alpha/2;
    end
    F(x_step)
    x_step = x_step+alpha*step
    grad = Gradient(x_step)
    n = n+1;
end