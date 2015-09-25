function [ output_args ] = constraint( unknowns )
%Returns the constraints
    global vecLen;
    global dataPts;
    global data;
    global r;

    w = unknowns(1:vecLen,1);
    eta = unknowns(vecLen+1:dataPts+vecLen,1);
    b = unknowns(end,1);
    
    output_args = [r.*((data*w)-b)+eta-1;eta];

end

