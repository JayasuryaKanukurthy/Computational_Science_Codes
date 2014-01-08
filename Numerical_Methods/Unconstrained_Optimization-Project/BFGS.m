function xx = BFGS(x)
    xk = x;
    Bk = hessian(x);
    xx = [x'];
    sk = inv(Bk)*-fgradient(xk);
    xk1 = xk+sk;
    tol = 0.00000000001;
    result = [];
    while((xk1(1,1) ~= 1)||(xk(2,1) ~= 1))
      sk = inv(Bk)*-fgradient(xk);
      xk1 = xk+sk;
      yk = fgradient(xk1)-fgradient(xk);
      Bk1 = Bk+(yk*yk')./(yk'*sk)-(Bk*sk*sk'*Bk)/(sk'*Bk*sk);
      Bk = Bk1;
      xk = xk1;
      xx = [xx;xk'];
    end
end