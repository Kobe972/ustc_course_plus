function [f,g]=chain(chart,startpos,curpos,direction,vis) %f=[chain,succeed]
if(~all(direction==[0,0])&&all(startpos==curpos)) %Completed
    f=[curpos];
    g=1;
    return;
end
if(~all(direction==[0,0])&&chart(curpos(1),curpos(2))==inf) %Wrong way
    f=[curpos];
    g=0;
    return;
end
if(all(direction==[0,0])) %On start
    direction=[0,1];
    while(direction(2)+curpos(2)<=length(chart(1,:)))
        newpos=curpos+direction;
        if(vis(newpos(1),newpos(2))==1) %already in path
            direction=direction+[0,1];
            continue;
        end
        vis(newpos(1),newpos(2))=0;
        [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
        vis(newpos(1),newpos(2))=1;
        if(tmp2==0) %Not found
            direction=direction+[0,1];
            continue;
        else
            f=[curpos;tmp1];
            g=1;
            return;
        end
    end
    direction=[1,0];
    while(direction(1)+curpos(1)<=length(chart(:,1)))
        newpos=curpos+direction;
        if(vis(newpos(1),newpos(2))==1) %already in path
            direction=direction+[1,0];
            continue;
        end
        vis(newpos(1),newpos(2))=0;
        [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
        vis(newpos(1),newpos(2))=1;
        if(tmp2==0) %Not found
            direction=direction+[1,0];
            continue;
        else
            f=[curpos;tmp1];
            g=1;
            return;
        end
    end
    direction=[0,-1];
    while(direction(2)+curpos(2)>=1)
        newpos=curpos+direction;
        if(vis(newpos(1),newpos(2)==1)) %already in path
            direction=direction+[0,-1];
            continue;
        end
        vis(newpos(1),newpos(2))=0;
        [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
        vis(newpos(1),newpos(2))=1;
        if(tmp2==0) %Not found
            direction=direction+[0,-1];
            continue;
        else
            f=[curpos;tmp1];
            g=1;
            return;
        end
    end
    direction=[-1,0];
    while(direction(1)+curpos(1)>=1)
        newpos=curpos+direction;
        if(vis(newpos(1),newpos(2))==1) %already in path
            direction=direction+[-1,0];
            continue;
        end
        vis(newpos(1),newpos(2))=0;
        [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
        vis(newpos(1),newpos(2))=1;
        if(tmp2==0) %Not found
            direction=direction+[-1,0];
            continue;
        else
            f=[curpos;tmp1];
            g=1;
            return;
        end
    end
else
    if(direction(1)==0)
        direction=[1,0];
        while(direction(1)+curpos(1)<=length(chart(:,1)))
            newpos=curpos+direction;
            if(vis(newpos(1),newpos(2))==1) %already in path
                direction=direction+[1,0];
                continue;
            end
            vis(newpos(1),newpos(2))=0;
            [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
            vis(newpos(1),newpos(2))=1;
            if(tmp2==0) %Not found
                direction=direction+[1,0];
                continue;
            else
                f=[curpos;tmp1];
                g=1;
                return;
            end
        end
        direction=[-1,0];
        while(direction(1)+curpos(1)>=1)
            newpos=curpos+direction;
            if(vis(newpos(1),newpos(2))==1) %already in path
                direction=direction+[-1,0];
                continue;
            end
            vis(newpos(1),newpos(2))=0;
            [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
            vis(newpos(1),newpos(2))=1;
            if(tmp2==0) %Not found
                direction=direction+[-1,0];
                continue;
            else
                f=[curpos;tmp1];
                g=1;
                return;
            end
        end
    else
        direction=[0,1];
        while(direction(2)+curpos(2)<=length(chart(1,:)))
            newpos=curpos+direction;
            if(vis(newpos(1),newpos(2))==1) %already in path
                direction=direction+[0,1];
                continue;
            end
            vis(newpos(1),newpos(2))=0;
            [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
            vis(newpos(1),newpos(2))=1;
            if(tmp2==0) %Not found
                direction=direction+[0,1];
                continue;
            else
                f=[curpos;tmp1];
                g=1;
                return;
            end
        end
        direction=[0,-1];
        while(direction(2)+curpos(2)>=1)
            newpos=curpos+direction;
            if(vis(newpos(1),newpos(2))==1) %already in path
                direction=direction+[0,-1];
                continue;
            end
            vis(newpos(1),newpos(2))=0;
            [tmp1,tmp2]=chain(chart,startpos,newpos,direction,vis);
            vis(newpos(1),newpos(2))=1;
            if(tmp2==0) %Not found
                direction=direction+[0,-1];
                continue;
            else
                f=[curpos;tmp1];
                g=1;
                return;
            end
        end
    end
end
f=[curpos];
g=0;
return;