function [ psik] = psik( const )
%Return the function value, first differential
%second differential of glued transformation function
    global k;
    a = -2;
    c = log(0.5)+0.5;
    
    psik = zeros(length(const),3);
    
    for i = 1:length(const)
        t = k*const(i);
        if t >= -0.5
            psik(i,1) = log(1+t);
            psik(i,2) = 1/(1+t);
            psik(i,3) = -1/((1+t)^2);
        else
            psik(i,1) = a*(t^2)+c;
            psik(i,2) = 2*a*t;
            psik(i,3) = 2*a;
        end
    end
    
end