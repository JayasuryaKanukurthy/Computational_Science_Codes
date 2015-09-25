function [ gradf ] = Gradient( x )
x1 = x(1);
x2 = x(2);
gradf1 = 20*x1^3-12*x1+2*x2+15;
gradf2 = 24*x2^3+2*x1+10*x2-7;
gradf = [gradf1;gradf2];
end

