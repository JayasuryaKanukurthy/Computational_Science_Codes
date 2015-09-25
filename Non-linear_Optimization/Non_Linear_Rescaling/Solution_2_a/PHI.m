function  [output_args] = PHI( x,y,k)
    x1 = x(1);
    x2 = x(2);
    psik_values = psik(x,y,k);
    output_args = 2*x1-3*x2-(1.0/k)*y*psik_values(1);
end

