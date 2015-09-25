function [ output_args ] = Grad_PHI( x,y,k )
    x1 = x(1);
    x2 = x(2);
    grad_psik = psik(x,y,k);
    
    grad_phi1 = 2*x1 + 6*x1*y*grad_psik(2)+2*x2;
    grad_phi2 = 2*x1+2*x2+2*x2*grad_psik(2)*y;
    
    output_args = [grad_phi1;grad_phi2];

end

