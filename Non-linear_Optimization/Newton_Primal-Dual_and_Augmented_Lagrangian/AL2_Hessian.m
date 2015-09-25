function [ output_args ] = AL2_Hessian( x,y,k )
        x1 = x(1);
        x2 = x(2);
        y1 = y(1);
        output_args = [ 2 - 6*y1 + k*(3*x1^2 + x2^2 - 9)*6 + k*6*x1*6*x1,2 + k*(2*x2)*6*x1;
                        2 + k*(6*x1)*2*x2,2 - 2*y1 + k*(3*x1^2 + x2^2 - 9)*2 + k*(2*x2)*2*x2];
end

