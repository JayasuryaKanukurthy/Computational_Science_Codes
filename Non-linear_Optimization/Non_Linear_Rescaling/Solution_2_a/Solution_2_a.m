%Nonlinear Rescaling Method for Problem 2(a)
clear all; clc;

epsilon = 0.001;
k = 100;          %Change this later and see how the execution converges

x = [0.1 0.7]';
y = [1];

x_step = x;
y_step = y;

PHI_Values = PHI(x_step,y_step,k);

while max([norm(Grad_PHI(x_step,y_step,k),2),norm(y_step*constraint(x_step),2),-constraint(x_step)]) > 10^-7
    
    n_steps = 0;
    psi_values = psik(x_step,y_step,k);
    %Implementing Newton's Method
    eta = 0.3;
    while norm(Grad_PHI(x_step,y_step,k),2) > max([10^-7,(1.0/k)*norm(y_step-psi_values(2)*y_step,2)]) %Check this for matrix dimensions!!!!
        %--------------Finding Direction----------------
        %Regularization of Hessian
        Hessian = Hessian_PHI(x_step,y_step,k);
   %     norm(Grad,2)
        %Regularize Hessian Here and return regularized Hessian
        lambda = 0.0001;  
        
        [R,p] = chol(Hessian);      %Checking for positive definiteness using cholsky factorization
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
         end
% %         %Updating x_step
        
        x_step = x_step+alpha*delta_xs;
        
        %Updating grad_PHI, psi_values
        n_steps = n_steps+1;
    end
    
    %Updating y_step
    psi_values = psik(x_step,y_step,k);
    y_step = y_step * psi_values(2);
    fprintf('Newton Iteration = % d; x1 = % f; x2 = % f; function = % f; constraint=% f \n', n_steps, x_step(1), x_step(2), 2*x_step(1) - 3*x_step(2), constraint(x));

end
