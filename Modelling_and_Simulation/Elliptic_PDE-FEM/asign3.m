clear all; clc;
load output/mesh.dat
load output/nodes.dat
load output/pressure
load output/velocity
load output/gradient

subplot(2,2,1);
trimesh(mesh,nodes(:,1),nodes(:,2),nodes(:,3));
view(0,90);
subplot(2,2,2);
quiver(nodes(:,1),nodes(:,2),gradient(:,1),gradient(:,2));
title('Gradient');
subplot(2,2,3);
quiver(nodes(:,1),nodes(:,2),velocity(:,1),velocity(:,2));
title('Velocity');
subplot(2,2,4);
trisurf(mesh,nodes(:,1),nodes(:,2),nodes(:,3),pressure(1:(end-2),1));
str1 = {'Max Pressure: ' num2str(pressure(pressure(end,1))) 'Min Pressure: ' num2str(pressure(pressure(end-1,1)))};
text(-0.5,-1,['\color{white}' str1],'VerticalAlignment', 'bottom','HorizontalAlignment','center');
title('Pressure');
view(0,90);
