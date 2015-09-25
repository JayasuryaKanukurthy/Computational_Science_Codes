%Nonlinear Rescaling Method for Primal SVM Implementation
clear all; clc;

global vecLen;
global dataPts;
global epsilon;
global k;
global C;
global data;
global r;
global gradc;

epsilon = 1e-7;
k = 100;
data = dlmread('Train_36.txt');
vecLen = length(data(1,:));
dataPts = length(data(:,1));
C = 100;
r = [ones(500,1);-1*ones(500,1)];             %vector with 1 for 3 and -1 for 6
unknowns = ones(vecLen+dataPts+1,1);          %x
gradc = [data .* repmat(r,1,vecLen),eye(dataPts), -r;zeros(length(r),vecLen), eye(dataPts),zeros(length(r),1)];
lambda = ones(length(gradc),1);                %y

tsteps = 0;
Grad = Grad_PHI(unknowns,lambda);
psikc = psik(unknowns);

while max([norm(Grad),norm(lambda.*constraint(unknowns)),max(-constraint(unknowns))]) > 10^-5
    %Implementing Newton's Method
    eta = 0.1;
    n_steps = 0;
    while norm(Grad,2) > max([10^-5,(1.0/k)*norm(lambda-psikc(:,2).*lambda,2)]) %Check this for matrix dimensions!!!!
        %--------------Finding Direction----------------
        %Regularization of Hessian
        PHI_Val = PHI(unknowns,lambda);
        Hessian = Hessian_PHI(unknowns,lambda);
        lambdah=0.0001;
         while min(eig(Hessian+(lambdah)*eye(length(Hessian)))) <= 0                  %p > 0 => not positive definite
             lambdah = 10*lambdah;
         end
        
%        Grad = Grad_PHI(unknowns,lambda);
        
        %Finding Direction
        
        delta_xs = linsolve(Hessian+(lambdah)*eye(length(Hessian)),-Grad);
        
        %Finding unknowns: Armijio Rule
         alpha = 1;
         while PHI(unknowns+alpha*delta_xs,lambda)-PHI(unknowns,lambda) >= eta * alpha * Grad'*delta_xs && alpha>10^-5
              alpha = alpha/2;  
         end
         
        unknowns = unknowns+alpha*delta_xs;
        
        %Updating grad_PHI, psi_values
        n_steps = n_steps+1;
        psikc = psik(unknowns);
        psi_values = diag(psikc(:,2));
        Grad = Grad_PHI(unknowns,lambda);
        fprintf('\t\t\t\tNewton: ||Grad_PHI|| = %f\n',norm(Grad));
    end
    
    %Updating lambda
    Grad = Grad_PHI(unknowns,lambda);
    psikc = psik(unknowns);
    lambda = lambda.*psikc(:,2);
    tsteps=tsteps+1;
    cond = max([norm(Grad),norm(lambda.*constraint(unknowns)),max(-constraint(unknowns))]);
    c = constraint(unknowns);
%     Grad = Grad_PHI(unknowns,lambda);
    fprintf('\nNRAL = %d;max(-c(x),0): %f; \nComplementarity: %f; Newton Steps= %d; ; \n\n\n',tsteps,max([0,-c']),max(-c.*lambda), n_steps);
end
