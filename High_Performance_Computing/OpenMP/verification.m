load matlab_inputs
xmin = matlab_inputs(1);
xmax = matlab_inputs(2);
ymin = matlab_inputs(3);
ymax = matlab_inputs(4);
grid = matlab_inputs(5);
nx = (xmax-xmin)/grid;
ny = (ymax-ymin)/grid;
syms x y;
a = 10, b = 10;
f = a*x+b*x/(x*x+y*y);
[X,Y] = meshgrid(xmin:nx:xmax,ymin:ny:ymax);
g = gradient(f, [x,y])
G1 = subs(g(1), [x,y], {X,Y}); G2 = subs(g(2),[x,y],{X,Y});
G1 = double(G1);
G2 = double(G2);
grad_v = sqrt((G1.^2)+(G2.^2));
hold on;
quiver(X,Y,G1,G2);
contour(X,Y,grad_v);
hold off;
