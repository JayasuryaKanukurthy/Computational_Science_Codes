function r = hessian(x1)
    x = x1(1,1);
    y = x1(2,1);
    r = [2+800*x^2-400*(-x^2+y), -400*x;-400*x,200];
end
