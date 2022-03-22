function f=Display(cj,CB,XB,b,mat,cjzj,theta)
fprintf("\tcj\t\t");
for i = 1:length(cj(:))
    fprintf("%.2f\t",cj(i));
end
fprintf("theta\nCB\tXB\tb\t");
for i = 1:length(cj(:))
    fprintf("x%d\t",i);
end
fprintf("\n");
for i = 1:length(CB(:))
    fprintf("%.2f\tx%d\t%.2f\t",CB(i),XB(i),b(i));
    for j = 1:length(cj(:))
        fprintf("%.2f\t",mat(i,j));
    end
    if theta(i)==inf
        fprintf("-\n");
    else
        fprintf("%.2f\n",theta(i));
    end
end
fprintf("\tcj-zj\t\t");
for i = 1:length(cjzj(:))
    fprintf("%.2f\t",cjzj(i));
end
fprintf("\n\n");
return;