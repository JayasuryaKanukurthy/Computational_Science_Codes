y0 = [0 1 0];  %y0(1) = initial value of ode1, y0(2) = initial value of ode 2 and so on;
params_setA = [0.1 3 -1]; %Parameters set A: as given in the problem
params_setB = [0.1 3 1]; %Parameters set B: as given in the problem
fn1 = @(a,eps,t) exp(a.*t).*sin(t./eps);
fn2 = @(a,eps,t) exp(a.*t).*cos(t./eps);
fn3 = @(a,eps,t) (exp(2*a.*t)-1)./(2*a);
timestep = 0.001;    %Setting the time step
fprintf('Enter: \n1. Parameter Set A\n2. Parameter Set B\n\n');
set = input('Enter Choice:');
if(set == 1)
    params = params_setA;
else
    params = params_setB;
    
end
a = params(3);
eps = params(1);

[fey1 fey2 fey3 tA] = ForwardEuler(y0,params,timestep);

figure (1);
%plot(tA,fey1,tA,fn1(tA(:)));
plot(tA,fey1,tA,fn1(a,eps,tA),'-');
legend('Euler Method','Analytic Function');
title('ODE 1');

figure (2);
%plot(tA,fey1,tA,fn1(tA(:)));
plot(tA,fey2,tA,fn2(a,eps,tA),'-');
legend('Euler Method','Analytic Function');
title('ODE 2');

figure (3);
%plot(tA,fey1,tA,fn1(tA(:)));
plot(tA,fey3,tA,fn3(a,eps,tA),'-');
legend('Euler Method','Analytic Function');
title('ODE 3');
