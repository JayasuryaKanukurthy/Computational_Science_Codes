function  [output_args] = PHI( x,y,k)
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
    
    output_args = 3*x1^3+2*x2^3+x3^3+x4^3 - (1.0/k)*y1*psik1(1)-(1.0/k)*y2*psik2(1);
end

