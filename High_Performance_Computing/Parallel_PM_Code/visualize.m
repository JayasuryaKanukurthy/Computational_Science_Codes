clear all; clc;
load output
particle_info = output(:,1:2);
pathx = output(:,2:2:end-1);
pathy = output(:,3:2:end);
scrsz = get(0,'ScreenSize');
clr = output(:,1);
for i=1:length(clr)
    if(clr(i)>0)
        clr(i) = 5;
    else
        clr(i) = 1;
    end
end
for i = 1:length(pathx(1,:))
    axis([0 51 0 51]);
    axis manual
    scatter(pathx(:,i),pathy(:,i),25,clr);
    F(i)= getframe();
end
axis([0 51 0 51]);
axis manual
movie(F,1,0.5);
%movie2avi(F,'particle_sim.avi','compression','none');