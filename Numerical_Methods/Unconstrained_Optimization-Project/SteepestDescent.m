function xx = SteepestDescent(x)
    xk = x;
    xx = [x'];
    alpha = 20;
    sk = -fgradient(xk);
        for i = 1:2000
        sk = -fgradient(xk);
        f = @(a) (100*((xk(2,1)+a*sk(2,1))-(xk(1,1)+a*sk(1,1))^2)^2+(1-(xk(1,1)+a*sk(1,1))));
        alpha = fminsearch(f,alpha);     %linesearch - Finding alpha to minimise f(xk+alpha*sk);
        xk1 = xk+alpha*sk;
        xk = xk1;
        xx = [xx;xk'];
        end
end