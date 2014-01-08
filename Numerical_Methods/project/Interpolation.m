clc; clear all;
x = [8 11 15 18 22]';
y = [5 9 10 9 7]';
A = [];
for i = 1:length(x)
    m = x(:,1).^(i-1);
    A = [A,m];
end
leastsquare_sol = A(:,1:3)\y;
interpolation_sol = A\y;
leastsquare_sol = leastsquare_sol(end:-1:1);
interpolation_sol = interpolation_sol(end:-1:1);
polyfit_degree_2 = polyfit(x',y',2);
polyfit_degree_4 = polyfit(x',y',4);
linespace = [8:0.1:22];

figure(1)
hold on;
plot(linespace,polyval(leastsquare_sol,linespace),linespace,polyval(polyfit_degree_2,linespace));
title('Least Square Solution Result with Degree 2 Polynomial');
legend('Implemented Method', 'MATLAB Function');
scatter(x',y');
hold off;

figure(2)
hold on;
plot(linespace,polyval(interpolation_sol,linespace),linespace,polyval(polyfit_degree_4,linespace));
title('Interpolation Solution with Degree 4 Polynomial');
legend('Implemented Method', 'MATLAB Function');
scatter(x',y');
hold off;
