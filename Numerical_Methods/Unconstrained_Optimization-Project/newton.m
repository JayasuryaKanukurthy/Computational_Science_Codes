function xx = newton(x)
    x0=x;
    xk = x;
    xx = [x'];
    for k = 1:5
        sk = inv(hessian(xk))*-fgradient(xk);
        x0=xk;
        xk =xk+sk;
        xx = [xx;xk'];
    end
end