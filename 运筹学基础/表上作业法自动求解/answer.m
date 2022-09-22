clear;
cost=[15,15,18,22,22;21,21,25,16,16;inf,0,inf,inf,0];
chart=[inf,inf,250,140,10;290,30,inf,130,inf;inf,inf,inf,inf,30];
a=[400,450,30];
b=[290,30,250,270,40];
solve(cost,a,b,chart);