clear;
c=[0.9;0.45;-0.05;1.4;0.95;0.45;1.9;1.45;0.95;0;0;0;0;0;0;0;0];
a=[];
b=[];
deq=[2000,2500,1200,0,0,0,0,0];
aeq(1,:)=[1,1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0];
aeq(2,:)=[0,0,0,1,1,1,0,0,0,0,1,0,0,0,0,0,0];
aeq(3,:)=[0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,0,0];
aeq(4,:)=[-0.4,0,0,0.6,0,0,0.6,0,0,0,0,0,1,0,0,0,0];
aeq(5,:)=[0,-0.85,0,0,0.15,0,0,0.15,0,0,0,0,0,1,0,0,0];
aeq(6,:)=[-0.2,0,0,-0.2,0,0,0.8,0,0,0,0,0,0,0,1,0,0];
aeq(7,:)=[0,-0.6,0,0,-0.6,0,0,0.4,0,0,0,0,0,0,0,1,0];
aeq(8,:)=[0,0,-0.5,0,0,-0.5,0,0,0.9,0,0,0,0,0,0,0,1];
x=linprog(-c,a,b,aeq,deq,zeros(17,1),[]);
disp(c'*x);