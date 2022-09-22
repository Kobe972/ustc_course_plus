#include<cstdio>
#include<windows.h>
int line[505][505],girl[505],k,m; //girl[i]代表第i个女孩的npy
bool found(int x) //给x找女友 
{
	if(x==m+1) return 1;
    for(int i=1; i<=m; i++)
    	if(line[x][i]&&!girl[i]) //回溯法 
		{
			girl[i]=x;
			if(found(x+1)) return 1;
			girl[i]=0;
		} 
    return 0;
}
int main()
{
    int x,y; 
	scanf("%d",&k); //输入连线数 
	scanf("%d",&m); //输入人数 
    for(int i=0; i<k; i++)
    {
        scanf("%d %d",&x,&y); //注意：输入x y表示实际上x和y可能是一对 
        line[x][y]=1;
    }
    system("cls");found(1); 
    for(int i=1; i<=m; i++) printf("%d的npy是%d\n",girl[i],i);system("pause");
    return 0;
}
