function [ output_args ] = AL2( x,y,k )
    x1 = x(1);
    x2 = x(2);
    y1 = y(1);
    output_args = x1^2+2*x1*x2+x2^2-y1*(3*x1^2+x2^2-9)+k/2*norm(3*x1^2+x2^2-9)^2;

end

