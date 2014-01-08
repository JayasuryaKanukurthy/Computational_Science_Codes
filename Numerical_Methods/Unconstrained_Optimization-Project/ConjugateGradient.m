function xx = ConjugateGradient(x)
    xk = x;
    gk = fgradient(xk);
    sk = -gk;
    alpha = 20;
    xx = [x'];
%    while((xk1(1,1) ~= 1) && (xk1(2,1) ~= 1))
    for i = 1:2000
        f = @(a) (100*((xk(2,1)+a*sk(2,1))-(xk(1,1)+a*sk(1,1))^2)^2+(1-(xk(1,1)+a*sk(1,1))));
        alpha = fminsearch(f,alpha);     %linesearch - Finding alpha to minimise f(xk+alpha*sk);
        xk1 = xk+alpha*sk;
        gk1 = fgradient(xk1);
        betak = (gk1'*gk1)/(gk'*gk);
        sk = -gk1 + betak*sk;
        gk = gk1;
        xk = xk1;
        xx = [xx;xk'];
    end
end