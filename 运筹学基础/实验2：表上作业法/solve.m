function f=solve(cost,a,b)
chart=Fill(cost,a,b);
exam=GetC(cost,chart);
while(min(min(exam))<0)
    chart=Adjust(chart,exam);
    exam=GetC(cost,chart);
end
fprintf("检验数均不小于0，达到最优。\n");
sum=0;
fprintf("故最小值为");
for i=1:length(chart(:,1))
    for j=1:length(chart)
        if(chart(i,j)~=inf)
            fprintf("%d×%d+",chart(i,j),cost(i,j));
            sum=sum+chart(i,j)*cost(i,j);
        end
    end
end
fprintf("\b=%d。\n",sum);
return;