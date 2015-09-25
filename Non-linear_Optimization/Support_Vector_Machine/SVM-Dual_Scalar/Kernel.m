function [ Ker ] = Kernel( data )
%Scalar Kernel
    n = length(data);
    Ker = zeros(n,n);
    for i = 1:n
        for j = 1:n
            Ker(i,j) = data(i,:)*data(j,:)';
        end
    end

end

