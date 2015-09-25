function H = Hessian( x )

x1 = x(1);
x2 = x(2);

h1 = 60*x1^2-12;
h4 = 72*x2^2+10;

H = [h1,2;2,h4];

end

