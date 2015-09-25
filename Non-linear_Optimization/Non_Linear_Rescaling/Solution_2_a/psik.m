function [ output_args ] = psik( x,y,k )
%Return the function value, first differential
%second differential of glued transformation function
    psi = 0;
    psi_diff = 0;
    psi_2_diff = 0;
    
    a = -2;
    b =0;
    c = log(0.5)+0.5;
    
    t = k*constraint(x);
    
    if t > -0.5
        psi = log(1+t);
        psi_diff = 1.0/(1+t);
        psi_2_diff = -1.0/(1+t)^2;
    else
        psi = a*(t^2)+b*t+c;
        psi_diff = 2*a*t+b;
        psi_2_diff = 2*a;
    end
    
    output_args = [psi;psi_diff;psi_2_diff];

end
