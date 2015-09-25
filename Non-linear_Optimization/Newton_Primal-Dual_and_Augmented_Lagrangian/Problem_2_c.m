clear all; clc;
epsilon = 0.01;
y = [0;0];
x = [1;2;3;4];
k = 100;
g = @(x) [x(1)^2+x(2)^2+x(3)^2+x(4)^2-4; x(1)+x(2)+2*x(3)+3*x(4)-1];

while norm(g(x)) > epsilon
    newton_steps = 0;
    eta = 0.1;
    %Implementing Unconstrained Newton's Method
    while norm(AL3_Gradient(x,y,k)) >= max(epsilon,0.2*g(x))
        %Gradient of Augmented Lagrangian
        Gradient = AL3_Gradient(x,y,k);
            
        %Hessian of Augmented Lagrangian
        Hessian = AL3_Hessian(x,y,k);
        
        lambda = 0.00001;  
        %Regularization: Checking for positive definiteness
        while min(eig( Hessian + lambda*eye(length(Hessian)))) <= 0
            lambda = 10*lambda;
        end
        %Hessian = Hessian + lambda*eye(length(Hessian));
        steps = (Hessian + lambda*eye(length(Hessian)))\-(Gradient);
        alpha = 1;
        %Armijo rule
        while (AL3(x+alpha*steps,y,k)-AL1(x,y,k)) >= eta*alpha*AL3_Gradient(x,y,k)'*steps
            alpha = alpha/2;
        end
        
        x = x+alpha*steps;
        newton_steps = newton_steps+1;
    end
    
    fprintf('x1 = %f; x2 = %f; x3 = %f; x4 = %f; Newton Steps = %d; Constraint1 = %f;Constraint2 = %f\n', x(1), x(2),x(3),x(4), newton_steps, g(x));
    y = y - k *g(x);
end



