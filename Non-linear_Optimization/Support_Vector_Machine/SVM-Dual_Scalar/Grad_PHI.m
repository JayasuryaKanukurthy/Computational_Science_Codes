function [ output_args ] = Grad_PHI( alpha,y,z)
    global r;
    global K;
    global C;
    global k;
    global gradc;
    
    gradf = -1*ones(length(alpha),1)+((r*r').*K)*alpha;
    gradg = r';
    eq_g1 = alpha'*r;
    
    psikc = psik([alpha;C-alpha]);
    
    output_args = gradf - gradc'*(psikc(:,2).*y) - gradg'*z+k*gradg' * eq_g1;
    
end
