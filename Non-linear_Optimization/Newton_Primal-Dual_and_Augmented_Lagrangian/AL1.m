function [ output_args ] = AL1( x,y,k )
    
    f = @(x) 2*x(1)-3*x(2); 
    g = @(x) x(1)^2+x(2)^2-25;
    
    output_args = f(x)-y'*g(x)+0.5*k*(norm(g(x))^2);

end

