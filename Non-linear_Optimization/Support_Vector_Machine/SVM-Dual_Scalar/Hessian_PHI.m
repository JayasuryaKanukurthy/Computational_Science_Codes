function [ output_args ] = Hessian_PHI(alpha,y)
    global K;
    global r;
    global k;
    global C;
    global gradc;

    hessf = (r*r').*K;
%     Y = diag(y);
%     PSI_Dprime = diag(psikc(:,3));
    gradg = r';
    psikc = psik([alpha;C-alpha]);
    output_args = hessf - k*gradc'*(diag(y.*psikc(:,3)))*gradc + k* (gradg' * gradg);
    
end
