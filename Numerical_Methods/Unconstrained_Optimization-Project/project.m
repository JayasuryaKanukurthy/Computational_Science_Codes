clear all; clc;

x = 0; y = 0;
fprintf('Enter:\n\n1. Newtons Method\n2. BFGS Method\n3. Conjugate Gradient Method\n4. Steepest Descent\n5. Direct Search\n');
choice = input('Enter Choice: ');
clc;
fprintf('Input Variables:\n1. [x,y] = [2,1]\n2. [x,y] = [-1,1] \n3. [x,y] = [0, 1]\n');
var = input('Enter choice of variables: ');

switch var
    case 1
        x = [2 1]';
    case 2
        x = [-1 1]';
    case 3
        x = [ 0 1]';
    otherwise
        a = input('Enter value for x:');
        b = input('Enter value for y:');
        x = [a b]';
end

switch choice
    case 5
        xx = DirectSearch(x);
    case 1
        xx = SteepestDescent(x);
    case 2
        xx = newton(x);
    case 3
        xx = BFGS(x);
    case 4
        xx = ConjugateGradient(x);
    otherwise
        break;
end
xx
%Plotting Script
[x, y] = meshgrid(-8.0:0.01:8.0, -8.0:0.01:8.0); 
figure; hold on;
contour(-8.0:0.01:8.0,-8.0:0.01:8.0,100*(y-x.^2).^2+(1-x).^2,50); 

xlabel('x'); ylabel('y'); title('Contour plots of Rosenbrock’s function and the optimization path');
%hold on figure, and wait for plotting path
hold on;
%plot path
plot(xx(:,1),xx(:,2),'r-o');
