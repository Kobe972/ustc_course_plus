function f=Fill(cost,a,b)
chart=inf*ones(length(a(:)),length(b(:)));
for cnt=1:length(a(:))+length(b(:))-1
    minimum=min(min(cost));
    if(minimum==inf)
        continue;
    end
    [u,v]=find(cost==minimum);
    chart(u,v)=min(a(u),b(v));
    if(a(u)>b(v)) %mask the column
        for row=1:length(a(:))
            cost(row,v)=inf;
        end
        a(u)=a(u)-b(v);
    else %mask the row
        for col=1:length(b(:))
            cost(u,col)=inf;
        end
        b(v)=b(v)-a(u);
    end
end
f=chart;
fprintf("初始基可行解为：\n");
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