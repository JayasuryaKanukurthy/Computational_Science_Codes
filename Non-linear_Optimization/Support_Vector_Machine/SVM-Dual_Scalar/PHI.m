function  [output_args] = PHI(alpha,y,z)
    global r;
    global K;
    global C;
    global k;
    
    psikc = psik([alpha;C-alpha]);
    fx = -sum(alpha) + 0.5*sum(sum((alpha*alpha').*(r*r').*K));
    eq_g1 = alpha'*r;
    output_args = fx - 1.0/k *(sum(y .* psikc(:,1)))- z*eq_g1 + k/2.0 * norm(eq_g1)^2;
    
end
