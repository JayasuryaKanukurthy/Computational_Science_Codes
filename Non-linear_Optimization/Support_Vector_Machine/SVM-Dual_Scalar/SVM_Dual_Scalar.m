% Nonlinear Rescaling Method for Problem 2(a)
clear all; clc;
global vecLen; global dataPts; global epsilon; global k; global C; global data; global r; global K; global gradc;

epsilon = 1e-5;
k = 100;
data = dlmread('Train_36.txt');
vecLen = length(data(1,:));
dataPts = length(data(:,1));
C = 100;
r = [ones(500,1);-1*ones(500,1)];             %vector with 1 for 3 and -1 for 6
alpha = ones(dataPts,1);          %x

K = Kernel(data);
psikc = psik([alpha;C-alpha]);
gradc = [eye(length(alpha));-1*eye(length(alpha))];
tsteps = 0;

y = ones(length(alpha)*2,1);
z = 0;
Grad = Grad_PHI(alpha,y,z);
constraint = @(x)[x;C-x];
g = @(x)x'*r;

while max([norm(Grad,2),norm(y.*constraint(alpha)),max(-constraint(alpha)),norm(g(alpha)) ]) >epsilon
    
    n_steps = 0;
    psikc = psik(constraint(alpha));
    % Implementing Newton's Method
    eta = 0.08;
    while norm(Grad) > max([epsilon,(1.0/k)*norm((y-(psikc(:,2).*y))),norm(g(alpha))])
        
        Hessian = Hessian_PHI(alpha,y);
        lambda = 0.0001;  
        %Regularization of Hessian
        
        while min(eig(Hessian + lambda*eye(length(Hessian))))<=0                  %p > 0 => not positive definite
            lambda = 10*lambda;
        end

        %Finding Direction
        
        delta_xs = linsolve(Hessian + lambda*eye(length(Hessian)),-Grad_PHI(alpha,y,z));
        
       %Finding x_step: Armijio Rule
       alph = 1;
       while PHI(alpha+alph*delta_xs,y,z)-PHI(alpha,y,z) >= (eta * alph * Grad_PHI(alpha,y,z)'*delta_xs)       
            alph = alph/2;
       end
         %Updating alpha
        alpha = alpha+alph*delta_xs;
        
        %Updating grad_PHI, psi_values
        Grad = Grad_PHI(alpha,y,z);
        n_steps = n_steps+1;
        psikc = psik(constraint(alpha));
        fprintf('\t\tNewton Step: %d\t||Grad PHI|| = %f\n',n_steps,norm(Grad));
    end
    
    % Updating y_step
    PHI_Values = PHI(alpha,y,z);
    tsteps = tsteps + 1;
    psikc = psik(constraint(alpha));
    y = y.*psikc(:,2);
    z = z-k*g(alpha);
    Grad = Grad_PHI(alpha,y,z);    
    Norm_Grad_AL = @(alpha)-1*ones(length(alpha),1)+((r*r').*K)*alpha - gradc'*y -z'*r;
    fprintf('\nNRAL = % d; ||Grad L|| = %f;\n||g(x)|| = %f;max(-c(x),0): %f; \nComplementarity: %f; Newton Steps= %d; ; \n\n\n',tsteps,norm(Norm_Grad_AL(alpha)),norm(g(alpha)),max([0,-constraint(alpha)']),max(-constraint(alpha).*y), n_steps);
end
dlmwrite('alpha.txt',alpha,'\n');

