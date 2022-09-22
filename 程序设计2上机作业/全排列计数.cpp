#include<stdio.h>
int visited[10]={0},factor[10]={1,1,2,6,24,120,720,5040,40320,362880},n; //visited表示每个数字是否已经输出，factor存储0到9的阶乘 
void output(int k,int round) //round表示这次要输出第几个数字 
{
	if(round>n) return;
	int th=(k-1)/factor[n-round],over=0,i; //th表示待填充的最前面一位要偏移多少，over表示已经偏移了多少 
	for(i=1;i<=n;i++)
	{
		if(visited[i]==0&&over==th) break;
		if(visited[i]==0) over++;
	}
	visited[i]=1;
	printf("%d",i);
	output(k-th*factor[n-round],round+1);
}
int main()
{
	int k;
	scanf("%d %d",&n,&k);
	output(k,1);
	return 0;
}
