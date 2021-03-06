function coefficients = CubicSpline(x,y)
    t1 = x(1);
    t2 = x(2);
    t3 = x(3);
    t4 = x(4);
    t5 = x(5);
    A = zeros(16);
    A(1,1:4) = [1 t1 t1^2 t1^3];
    A(2,1:4) = [1 t2 t2^2 t2^3];
    A(3,5:8) = [1 t2 t2^2 t2^3];
    A(4,5:8) = [1 t3 t3^2 t3^3];
    A(5,9:12) = [1 t3 t3^2 t3^3];
    A(6,9:12) = [1 t4 t4^2 t4^3];
    A(7,13:16) = [1 t4 t4^2 t4^3];
    A(8,13:16) = [1 t5 t5^2 t5^3];
    A(9,1:8) = [0 1 2*t2 3*t2^2 0 -1 -2*t2 -3*(t2^2)];
    A(10,1:8) = [0 0 2 6*t2 0 0 -2 -6*t2];
    A(11, 5:12) = [0 1 2*t3 3*t3^2 0 -1 -2*t3 -3*(t3^2)];
    A(12,5:12) = [0 0 2 6*t3 0 0 -2 -6*t3];
    A(13,9:16) = [0 1 2*t4 3*t4^2 0 -1 -2*t4 -3*(t4^2)];
    A(14,9:16) = [0 0 2 6*t4 0 0 -2 -6*t4];
    A(15,1:4) = [0 0 2 6*t1];
    A(16,13:16) = [0 0 2 6*t5];
    yy = [y(1) y(2) y(2) y(3) y(3) y(4) y(4) y(5) 0 0 0 0 0 0 0 0]';
    coeff = A\yy;
    coefficients = [coeff(1:4,1)';coeff(5:8,1)';coeff(9:12,1)';coeff(13:16,1)'];
end
    
        