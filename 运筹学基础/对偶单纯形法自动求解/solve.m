function f=solve(cj,XB,b,mat,iter)
if nargin==4
    iter=1;
end
for i = 1:length(XB(:)) %Compute CB
    CB(i)=cj(XB(i));
end
for i = 1:length(cj(:)) %Compute cj-zj
    zj=0;
    for j = 1:length(CB(:))
        zj=zj+CB(j)*mat(j,i);
    end
    cjzj(i)=cj(i)-zj;
end
replace_out=find(b==min(b));
replace_out=replace_out(1);
if b(replace_out)<0 %Not optimized
    for i = 1:length(cj(:)) %Compute theta_j
        if mat(replace_out,i)>=0
            theta(i)=inf;
        else
            theta(i)=cjzj(i)/mat(replace_out,i);
        end
    end
    fprintf("Iteration %d:\n",iter);
    Display(cj,CB,XB,b,mat,cjzj);
    if min(theta)==inf %Infinite solution
        f=inf;
        return;
    else
        replace_in=find(theta==min(theta));
        replace_in=replace_in(1);
        XB(replace_out)=replace_in;
        for i = 1:length(CB(:)) %Rotate the matrix
            if i==replace_out
                continue
            end
            ratio=mat(i,replace_in)/mat(replace_out,replace_in);
            b(i)=b(i)-b(replace_out)*ratio;
            mat(i,:)=mat(i,:)-mat(replace_out,:)*ratio;
        end
        b(replace_out)=b(replace_out)/mat(replace_out,replace_in);
        mat(replace_out,:)=mat(replace_out,:)/mat(replace_out,replace_in);
        f=solve(cj,XB,b,mat,iter+1); %Process the next table
    end
else %Optimized
    f=0;
    for i = 1:length(CB(:))
        theta(i)=inf;
        f=f+CB(i)*b(i);
    end
    fprintf("Iteration %d:\n",iter);
    Display(cj,CB,XB,b,mat,cjzj);
end
return;