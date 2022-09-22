#include<iostream>
using namespace std;
int main()
{
	int n,x,cnt=0,tmp;
	cin>>n>>x;
	for(int i=1;i<=n;i++)
	{
		tmp=i;
		while(tmp)
		{
			if(tmp%10==x) cnt++;
			tmp/=10;
		}
	}
	cout<<cnt<<endl;
	return 0;
}

