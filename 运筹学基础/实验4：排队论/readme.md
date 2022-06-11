# 实验4：排队论
PB20000156  
徐亦昶
## 实验代码
本次实验直接套公式即可。
```matlab
%Compute.m
function f=Compute(lambda,mu,c,N)
P0=0;
rho=lambda/(c*mu);
for k=0:c
    P0=P0+(c*rho)^k/factorial(k);
end
P0=P0+c^c/factorial(c)*rho*(rho^c-rho^N)/(1-rho);
P0=1/P0;
fprintf("系统空闲的概率P0=%f\n",P0);
for i=1:N
    if i<=c
        P(i)=(c*rho)^i/factorial(i)*P0;
    else
        P(i)=c^c/factorial(c)*rho^i*P0;
    end
end
fprintf("系统中有n个顾客的概率依次为（n从1开始）：");
disp(P);
Lq=P0*c*(c*rho)^c/factorial(c)/(1-rho)^2*(1-rho^(N-c)-(N-c)*rho^(N-c)*(1-rho));
Ls=Lq+c*rho*(1-P(N));
Wq=Lq/(lambda*(1-P(N)));
Ws=Wq+1/mu;
fprintf("系统中平均顾客数Ls=%f\n",Ls);
fprintf("系统中平均等待接受服务的顾客数Lq=%f\n",Lq);
fprintf("平均逗留时间Ws=%f\n",Ws);
fprintf("平均等待时间Wq=%f\n",Wq);
return;
```
## 问题解决
问题：某风景区准备建造旅馆，根据事先的调查知道，顾客到达该景区的规律服从泊松分布，平均有6人/d。在已知的小旅馆等处调查的结果显示顾客平均逗留2d。试讨论该拟建造的旅馆在有8个单间的条件下，每天客房的平均占用数以及满员的概率。  
分析：属于M/M/8/8/inf模型，其中平均逗留时间即为平均服务时间。可知lambda=6,mu=0.5。  
运行Compute(6,0.5,8,8)，得到如下结果：
```
系统空闲的概率P0=0.000040
系统中有n个顾客的概率依次为（n从1开始）：    0.0005    0.0029    0.0114    0.0342    0.0822    0.1644    0.2818    0.4227

系统中平均顾客数Ls=6.928139
系统中平均等待接受服务的顾客数Lq=0.000000
平均逗留时间Ws=2.000000
平均等待时间Wq=0.000000
```
平均占用数为6.927139人，满员概率为P8=0.4227。
