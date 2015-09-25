function [ output_args ] = AL3( x,y,k )
    x1 = x(1); x2 = x(2); x3 = x(3); x4  = x(4);
    y1 = y(1);y2 = y(2);

    g = [x1^2+x2^2+x3^2+x4^2-4;x1+x2+2*x3+3*x4-1];
    output_args = 3*x1^3+2*x2^3+x3^3+x4^3-[y1 y2]*g+k/2*norm(g)^2;

end

