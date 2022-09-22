#include<iostream>
using namespace std;
int main()
{
	int next[10000],pre[10000],n,s,m,it;
	cout<<"input n,s,and m in order:";
	cin>>n>>s>>m;
	system("cls");
	cout<<"The survivor is ";
	for(int i=1;i<=n;i++) //构造环状双向链表 
	{
		next[i]=i+1;
		pre[i]=i-1;
	}
	next[n]=1;
	pre[1]=n;
	it=s;
	while(next[it]!=it)
	{
		for(int i=0;i<m-1;i++)
		{
			it=next[it];
		}
		next[pre[it]]=next[it];//删除it 
		pre[next[it]]=pre[it];
		it=next[it];
	}
	cout<<it<<"."<<endl;
	system("pause");
	return 0;
}
