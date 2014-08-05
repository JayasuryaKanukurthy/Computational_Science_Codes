load output/mesh_x
load output/mesh_y
load output/gradient_x
load output/gradient_y
load output/grad

%grad = sqrt((gradient_x.^2)+(gradient_y.^2));

hold on;
quiver(mesh_x,mesh_y,gradient_x,gradient_y);
contour(mesh_x,mesh_y,grad);
hold off;
saveas(gcf, 'Field_Visual','jpg');
quit;

