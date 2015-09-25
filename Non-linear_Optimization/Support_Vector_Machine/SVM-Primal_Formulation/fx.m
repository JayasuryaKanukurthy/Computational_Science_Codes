function [ output_args ] = fx( unknowns,lambda )
    global vecLen;
    global dataPts;
    global epsilon;
    global k;
    global C;
    global data;
    global r;

    w = unknowns(1:vecLen,1);
    eta = unknowns(vecLen+1:vecLen+dataPts,1);
    b = unknowns(end,1);
    
    output_args= 0.5* (w'*w)+ C * sum(eta);


end

