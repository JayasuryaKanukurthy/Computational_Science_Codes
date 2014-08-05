load output;
n = length(output(:,1));
phi = output(1:n/3,:);
gradx= output(n/3+1:2*n/3,:);
grady = output(2*n/3+1:end,:);
[X,Y] = meshgrid(1:length(phi(:,1)),1:length(phi(1,:)));
hold on;
quiver(X,Y,gradx,grady);
contour(X,Y,phi,180);
hold off;
saveas(gcf,'Field_Visual','jpg');

