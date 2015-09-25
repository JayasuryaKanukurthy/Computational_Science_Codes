function  [output_args] = PHI( x,y,k)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    x1 = x(1);
    x2 = x(2);
    psik_values = psik(x,y,k);
    output_args = x1^2+2*x1*x2+x2^2-(1.0/k)*y*psik_values(1);
end
