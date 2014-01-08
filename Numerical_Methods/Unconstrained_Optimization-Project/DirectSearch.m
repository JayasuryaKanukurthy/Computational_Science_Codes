function r = DirectSearch(x)
    r = [x'];
    result = fminsearch(@rosenbrock, x)'
    r = [r;result];
end