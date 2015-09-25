function [ output_args ] = AL1_Hessian( x,y,k )
        x1 = x(1);
        x2 = x(2);
        y1 = y(1);
        output_args =[ -2*y1 + k*(x1^2 + x2^2 - 25)*2 + k*4*x1^2, k*2*(x2)*2*x1;
                        k*2*(x1)*2*x2, -2*y1 + k*(x1^2 + x2^2 - 25)*2 + k*4*x2^2];
end
