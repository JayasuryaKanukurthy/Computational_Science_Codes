function [ Hessian ] = Hessian_PHI( x,y,k)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here

    x1 = x(1);
    x2 = x(2);
    x3 = x(3);
    x4 = x(4);
    y1 = y(1);
    y2 = y(2);

    psik1 = psik(x,y,k,1);
    psik2 = psik(x,y,k,2);
    
    hessf = [18*x1 0 0 0;0 12*x2 0 0;0 0 6*x3 0; 0 0 0 6*x4];
    
    Hessian = hessf - psik1(2)*y(1)*-2*eye(4) - k * [-2*x1,1;-2*x2,1;-2*x3,2;-2*x4,3]*([y(1),0;0,y(2)]*[psik1(3),0;0,psik2(3)])*[-2*x1,1;-2*x2,1;-2*x3,2;-2*x4,3]';
end

