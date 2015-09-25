function [ output_args ] = Hessian_PHI( x,y,k)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

    psik_values = psik(x,y,k);
    x1 = x(1);
    x2 = x(2);
    
    output_args = [ 2+6*y*psik_values(2)-k*y*psik_values(3)*36*x1*x1,       2-k*y*psik_values(2)*12*x1*x2;
                    2-k*y*psik_values(2)*12*x1*x2,                          2+2*y*psik_values(2)-k*y*psik_values(3)*4*x2*x2];

end

