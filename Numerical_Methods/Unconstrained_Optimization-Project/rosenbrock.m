function r = rosenbrock(x1)
    x = x1(1,1);
    y = x1(2,1);
    r = 100*(y-x^2)^2+(1-x)^2;
end
