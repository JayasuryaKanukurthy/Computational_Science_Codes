function [ output_args ] = constraint( x )
%Returns the constraint

    output_args = -3*x(1)^2-x(2)^2+9;

end

