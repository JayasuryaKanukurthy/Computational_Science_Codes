function r = fgradient(x1)
    x = x1(1,1);
    y = x1(2,1);
    r = [-2*(1-x)-400*x*(y-x^2);200*(-x^2+y)];
end
