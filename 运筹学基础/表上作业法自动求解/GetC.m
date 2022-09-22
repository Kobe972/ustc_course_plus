function f=GetC(cost,chart)
fprintf("空格\t闭回路\t检验数\n");
f=zeros(length(chart(:,1)),length(chart));
for i=1:length(chart(:,1))
    for j=1:length(chart)
        if(chart(i,j)~=inf)
            continue;
        end
        fprintf("(%d%d)\t",i,j);
        circ=chain(chart,[i,j],[i,j],[0,0],zeros(length(chart(:,1)),length(chart)));
        c=0;
        sign=1;
        for k=1:length(circ(:,1))-1
            c=c+sign*cost(circ(k,1),circ(k,2));
            sign=-sign;
            fprintf("(%d%d)-",circ(k,1),circ(k,2));
        end
        fprintf("(%d%d)\t%d\n",circ(length(circ(:,1)),1),circ(length(circ(:,1)),2),c);
        f(i,j)=c;
    end
end
fprintf("检验数表格：\n");
for i=1:length(chart(:,1))
    for j=1:length(chart)
        fprintf("%d\t",f(i,j));
    end
    fprintf("\b\n");
end
return;