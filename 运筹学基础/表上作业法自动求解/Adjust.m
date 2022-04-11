function f=Adjust(chart,exam)
minimum=min(min(exam));
[u,v]=find(exam==minimum);
u=u(1);
v=v(1);
startpos=[u,v];
circ=chain(chart,startpos,startpos,[0,0],zeros(length(chart(:,1)),length(chart)));
fprintf("由表得(%d,%d)为调入格，由此格出发所作闭回路为",u,v);
for i=1:length(circ(:,1))-1
    fprintf("(%d%d)-",circ(i,1),circ(i,2));
end
fprintf("(%d%d)\n",circ(length(circ(:,1)),1),circ(length(circ(:,1)),2));
even=circ(2:2:end,:);
theta=inf;
replace_out=even(1,:);
for i = 1:length(even(:,1))
    if(theta>chart(even(i,1),even(i,2)))
        theta=chart(even(i,1),even(i,2));
        replace_out=even(i,:);
    end
end
fprintf("由闭回路可得(%d,%d)为调出格，θ=%d\n",replace_out(1),replace_out(2),theta);
sign=1;
for i=1:length(circ(:,1))-1
    if(all(circ(i,:)==replace_out))
        chart(circ(i,1),circ(i,2))=inf;
        sign=-sign;
        continue;
    end
    chart(circ(i,1),circ(i,2))=chart(circ(i,1),circ(i,2))+theta*sign;
    sign=-sign;
end
chart(circ(1,1),circ(1,2))=theta;
f=chart;
fprintf("调整后的表格为：\n");
for i=1:length(chart(:,1))
    for j=1:length(chart)
        if(chart(i,j)~=inf)
            fprintf("%d\t",chart(i,j));
        else
            fprintf("-\t");
        end
    end
    fprintf("\b\n");
end
return;