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