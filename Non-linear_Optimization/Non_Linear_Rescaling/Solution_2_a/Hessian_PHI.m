function [ output_args ] = Hessian_PHI( x,y,k)

    psik_values = psik(x,y,k);
    
    output_args = [ 2*y*psik_values(2)-4*k*y*psik_values(3)*x(1)^2,   -4*k*y*psik_values(3)*x(1)*x(2);
                    -4*k*y*psik_values(3)*x(1)*x(2),                   2*psik_values(2)*y-4*k*y*psik_values(3)*x(2)^2];

end

