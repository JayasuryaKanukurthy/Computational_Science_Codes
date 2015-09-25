function [ output_args ] = Grad_PHI( unknown,lambda)
    global vecLen;
    global dataPts;
    global epsilon;
    global k;
    global C;
    global data;
    global r;
    global gradc;
    
    w = unknown(1:vecLen,1);
    eta = unknown(vecLen+1:vecLen+dataPts,1);
    b = unknown(end,1);
    
    gradf = [w;C*ones(length(eta),1);0];
    psikc = psik(unknown);
    PSI_Prime = diag(psikc(:,2));
    
    output_args = gradf -  gradc' * PSI_Prime * lambda;
end

