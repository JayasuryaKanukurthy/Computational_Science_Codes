function [ output_args ] = AL3_Gradient(  x,y,k )
    x1 = x(1); x2 = x(2); x3 = x(3); x4  = x(4);
    y1 = y(1);y2 = y(2);
    g1 = x1^2+x2^2+x3^2+x4^2-4;
    g2 = x1+x2+2*x3+3*x4-1;
    output_args=    [9*x1^2-2*x1*y1-y2+2*k*x1*g1+k*g2;
                     6*x2^2-2*x2*y1-y2+2*k*x2*g1+k*g2;
                     3*x3^2-2*x3*y1-2*y2+2*k*x3*g1+2*k*g2;
                     3*x4^2-2*x4*y1-3*y2+2*k*x4*g1+3*k*g2];
end

