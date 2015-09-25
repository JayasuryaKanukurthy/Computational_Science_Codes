function [ output_args ] = Grad_PHI( x,y,k )
%UNTITLED Summary of this function goes here
%   Detailed explanation goes here
    x1 = x(1);
    x2 = x(2);
    x3 = x(3);
    x4 = x(4);

    psik1 = psik(x,y,k,1);
    psik2 = psik(x,y,k,2);
    grad_phi = [9*x1^2;6*x2^2;3*x3^2;3*x4^2]-[-2*x1,1;-2*x2,1;-2*x3,2;-2*x4,3]*([psik1(2),0;0,psik2(2)]*[y(1);y(2)]);
    
    output_args = grad_phi;

end

