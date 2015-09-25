function  [output_args] = PHI( unknown,lambda)
%UNTITLED2 Summary of this function goes here
%   Detailed explanation goes here
    global k;
    global r;

    f = fx(unknown,lambda);
    psikc = psik(unknown);
    
    output_args = f - 1.0/k*((lambda'*psikc(:,1)));
    
end

