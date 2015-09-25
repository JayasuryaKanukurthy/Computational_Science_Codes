%Nonlinear Rescaling Method for Problem 2(a)
clear all; clc;

epsilon = 1e-7;
k = 200;          %Change this later and see how the execution converges

x = [-1 0.3 0.5 0.7]';
y = [1,1]';
x_step = x;
y_step = y;

tsteps = 0;
while max([norm(Grad_PHI(x_step,y_step,k),2),norm(y_step.*constraint(x_step),2),max(-constraint(x_step))]) > 10^-7
    
    n_steps = 0;
    psi_values1 = psik(x_step,y_step,k,1);
    psi_values2 = psik(x_step,y_step,k,2);
    psi_values = [psi_values1(2);psi_values2(2)];
    %Implementing Newton's Method
    eta = 0.1;
    while norm(Grad_PHI(x_step,y_step,k),2) > max([10^-7,(1.0/k)*norm(y_step-psi_values.*y_step,2)]) %Check this for matrix dimensions!!!!
        %--------------Finding Direction----------------
        %Regularization of Hessian
        Hessian = Hessian_PHI(x_step,y_step,k);
   %     norm(Grad,2)
        %Regularize Hessian Here and return regularized Hessian
        lambda = 0.0001;  
        
        while min(eig(Hessian + lambda*eye(length(Hessian)))) <= 0                  %p > 0 => not positive definite
            lambda = 10*lambda;
        end
        
        Grad = Grad_PHI(x_step,y_step,k);
        
        %Finding Direction
        
        delta_xs = (Hessian + lambda*eye(length(Hessian)))\-Grad;
        
        %Finding x_step: Armijio Rule
        
         alpha = 1;
         while PHI(x_step+alpha*delta_xs,y_step,k)-PHI(x_step,y_step,k) >= eta * alpha * Grad'*delta_xs
              alpha = alpha/2;  
%               if(alpha == 0)
%                   alpha = 1e-12;
%                   x_step = x_step+alpha*delta_xs;
%                   c = constraint(x_step);
%                   fprintf('Newton Iteration = % d; x1 = % f; x2 = % f; x3 = % f; x4 = % f; function = % f; Constraint1 = %f; Constraint2=%f\n', n_steps, x_step(1), x_step(2),x_step(3),x_step(4), 3*x_step(1)^3+2*x_step(2)^3+x_step(3)^3+x_step(4)^3, c(1),c(2));
%                   return;
%               end
         end
% %         %Updating x_step
        
        x_step = x_step+alpha*delta_xs;
        
        %Updating grad_PHI, psi_values
        n_steps = n_steps+1;
        psi_values1 = psik(x_step,y_step,k,1);
        psi_values2 = psik(x_step,y_step,k,2);
        psi_values = [psi_values1(2);psi_values2(2)];
    end
    
    %Updating y_step
    psi_values1 = psik(x_step,y_step,k,1);
    psi_values2 = psik(x_step,y_step,k,2);
    psi_values = [psi_values1(2);psi_values2(2)];
    y_step = y_step .* psi_values;
    tsteps=tsteps+1;
    c = constraint(x_step);
    fprintf('Newton Iteration = % d; x1 = % f; x2 = % f; x3 = % f; x4 = % f; function = % f; Constraint1 = %f; Constraint2=%f\n', n_steps, x_step(1), x_step(2),x_step(3),x_step(4), 3*x_step(1)^3+2*x_step(2)^3+x_step(3)^3+x_step(4)^3, c(1),c(2));
end
