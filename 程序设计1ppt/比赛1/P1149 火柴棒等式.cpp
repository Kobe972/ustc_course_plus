#include<iostream>
using namespace std;
int cost[10]={6,2,5,5,4,5,6,3,7,6},ans[2],cnt=0,glob;
void find(int n);
int fee(int n);
int main()
{
	cin>>glob;
	glob-=4;
	find(0);
	cout<<cnt<<endl;
	return 0;
}
int fee(int n)
{
	int a=0;
	if(n==0) return 6;
	while(n)
	{
		a+=cost[n%10];
		n/=10;
	}
	return a;
}
void find(int n)
{
	if(n==2)
	{
		if(glob==fee(ans[0])+fee(ans[1])+fee(ans[0]+ans[1])) cnt++;
	}
	else
	{
		for(int i=0;i<1000;i++)
		{
			ans[n]=i;
			find(n+1);
		}
	}
}
