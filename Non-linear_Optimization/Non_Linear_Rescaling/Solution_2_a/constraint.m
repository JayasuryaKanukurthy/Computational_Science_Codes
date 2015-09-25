function [ output_args ] = constraint( x )
%Returns the constraint

    output_args = -x(1)^2-x(2)^2+25;

end

