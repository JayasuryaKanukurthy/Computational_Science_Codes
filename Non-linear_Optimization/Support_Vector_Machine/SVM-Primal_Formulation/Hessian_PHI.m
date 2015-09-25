function [ Hessian ] = Hessian_PHI( unknown,lambda)
    global vecLen;
    global dataPts;
    global gradc;
    global k;
    
    hessf = zeros(vecLen+dataPts+1,vecLen+dataPts+1);
    hessf(1:vecLen,1:vecLen) = eye(vecLen);
    Y = diag(lambda);
    psikc = psik(unknown);
    Hessian = hessf - k*gradc'*Y*diag(psikc(:,3))*gradc;
end

