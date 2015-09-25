function [ output_args ] = constraint( x )
%Returns the constraint
    x1 = x(1);
    x2 = x(2);
    x3 = x(3);
    x4 = x(4);
    output_args = [-x1^2-x2^2-x3^2-x4^2+4; x1+x2+2*x3+3*x4-1];

end

