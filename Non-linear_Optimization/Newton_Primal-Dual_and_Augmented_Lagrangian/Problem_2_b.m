clear all; clc;
epsilon = 0.001;

y = 0;
x =[-2.5;2.5];

k = 1;
g = @(x) 3*x(1)^2+x(2)^2-9;

while norm(g(x)) > epsilon
    newton_steps = 0;
    eta = 0.1;
    %Implementing Unconstrained Newton's Method
    while norm(AL2_Gradient(x,y,k)) >= max(epsilon,0.2*g(x))
        %Gradient of Augmented Lagrangian
        Gradient = AL2_Gradient(x,y,k);
            
        %Hessian of Augmented Lagrangian
        Hessian = AL2_Hessian(x,y,k);
        
        lambda = 0.00001;  
        %Regularization: Checking for positive definiteness
        while min(eig( Hessian + lambda*eye(length(Hessian)))) <= 0
            lambda = 10*lambda;
        end
        %Hessian = Hessian + lambda*eye(length(Hessian));
        steps = (Hessian + lambda*eye(length(Hessian)))\-(Gradient);
        alpha = 1;
        %Armijo rule
        while (AL2(x+alpha*steps,y,k)-AL1(x,y,k)) >= eta*alpha*AL2_Gradient(x,y,k)'*steps
            alpha = alpha/2;
        end
        
        x = x+alpha*steps;
        newton_steps = newton_steps+1;
    end
    
    fprintf('x1 = %f; x2 = %f; Newton Steps = %d; Constraint Violation = %f\n', x(1), x(2), newton_steps, g(x));
    y = y - k *g(x);
end

