function [y1 y2 y3  t] = ForwardEuler(y,params,timestep)
%Implements Forward Euler Method for the given IVP of the ODEs and the
%parameters set
y01 = y(1);
y02 = y(2);
y03 = y(3);
yk1 = y01;
yk2 = y02;
yk3 = y03;
eps = params(1);
t_limit = params(2);
a = params(3);
h = timestep;
y1 = [y01];
y2 = [y02];
y3 = [y03];
t = [0];
dy1_dt = @(y_1,y_2) (y_2/eps)+a*y_1;
dy2_dt = @(y_1,y_2) (y_1/-eps)+a*y_2;
dy3_dt = @(y_1,y_2) (y_1*y_1 + y_2*y_2);
    for time = 0:h:t_limit
        yk1 = y01+h*dy1_dt(y01,y02);
        yk2 = y02+h*dy2_dt(y01,y02);
        yk3 = y03+h*dy3_dt(y01,y02);
        y1 = [y1,yk1];
        y2 = [y2,yk2];
        y3 = [y3,yk3];
        y01 = yk1;
        y02 = yk2;
        y03 = yk3;
        t = [t,time+h];
    end
end
