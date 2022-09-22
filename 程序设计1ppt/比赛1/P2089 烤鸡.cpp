#include<iostream>
using namespace std;
void find(int m);
int n,method[10],out[10000][10],cnt=0;
int main()
{
	cin>>n;
	find(0);
	cout<<cnt<<endl;
	for(int i=0;i<cnt;i++)
	{
		for(int j=0;j<10;j++)
			cout<<out[i][j]<<" ";
		cout<<endl;
	}
	return 0;
}
void find(int m)
{
	if(m==10)
	{
		if(method[0]+method[1]+method[2]+method[3]+method[4]+method[5]+method[6]+method[7]+method[8]+method[9]==n)
		{
			for(int i=0;i<10;i++)
				out[cnt][i]=method[i];
			cnt++;
			return;
		}
	}
	else
	{
		for(int i=1;i<=3;i++)
		{
			method[m]=i;
			find(m+1);
		}
		return;
	}
}
