clear;
A=[-1,0,0,0,0,-1,0,0;-1,-1,0,0,0,-1,0,0;-1,-1,-1,0,0,-1,0,0;-1,-1,-1,-1,0,0,0,-1;0,-1,-1,-1,-1,0,-1,0;0,0,-1,-1,-1,-1,0,0;0,0,0,-1,-1,-1,0,0;0,0,0,0,-1,-1,0,0;1,1,1,1,1,0,0,0];
b=[-4,-3,-4,-6,-5,-4,-8,-8,3];
intcon=[1;2;3;4;5;6;7;8];
c=[40,40,40,40,40,100,0,0];
Aeq=[0,0,0,0,0,1,-1,-1];
beq=[0];
[x,optvalue]=intlinprog(c,intcon,A,b,Aeq,beq,[0;0;0;0;0;0;0;0],[]);
disp(x);
disp(optvalue);