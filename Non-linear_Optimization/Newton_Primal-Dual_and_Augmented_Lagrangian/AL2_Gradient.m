function [ output_args ] = AL2_Gradient( x,y,k )
    x1 = x(1);
    x2 = x(2);
    y1 = y(1);
    output_args=    [2*x1 + 2*x2 - 6*y1*x1 + k*(3*x1^2 + x2^2 - 9)*6*x1;
                    2*x1 + 2*x2 - 2*y1*x2 + k*(3*x1^2 + x2^2 - 9)*2*x2];

end

