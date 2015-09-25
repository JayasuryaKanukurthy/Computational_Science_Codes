function [ output_args ] = AL1_Gradient( x,y,k )
    
    grad_f = [2;-3];
    g = @(x) x(1)^2+x(2)^2-25;
    Grad_g = @(x) [2*x(1);2*x(2)];
    
    output_args =   grad_f - y*Grad_g(x)+k*Grad_g(x)*g(x);

end

