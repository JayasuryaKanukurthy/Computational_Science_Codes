load output/mesh.dat
load output/nodes.dat
status = [];
scrsz = get(0,'ScreenSize');

for k = 0:40
    matFilename = sprintf('output/status%d.dat',k);
    data = load(matFilename);
    status = [status,data];
end

for i=1:k
    trisurf(mesh,nodes(:,1),nodes(:,2),nodes(:,3),status(:,i+1));
    F(i) = getframe();
end
movie(F,1,2)

