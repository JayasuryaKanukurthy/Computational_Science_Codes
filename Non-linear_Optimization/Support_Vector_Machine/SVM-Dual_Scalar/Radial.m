function [ Rad ] = Radial( data )
%Scalar Kernel
    n = length(data);
    Rad = zeros(n,n);
    for i = 1:n
        for j = 1:n
            Rad(i,j) = exp(-0.05*norm(data(i,:)-data(j,:))^2);
        end
    end

end

