#include<iostream>
#include<stdlib.h>
using namespace std;
int numbers[10];
int visited[10]={0};
char output[10]={0};
int n_num=0;
int getelem(char* s) //得到被空格split后的结果，如果separate token是空格，返回1，否则返回0 
{
	int i=0;
	char c;
	while((c=getchar())!=' '&&c!='\n'&&c!=EOF)
		s[i++]=c;
	s[i]=0;
	return c==' '?1:0;
}
void permu(int round)
{
	if(round>n_num)
	{
		cout<<output<<endl;
		return;
	}
	for(int i=0;i<n_num;i++)
	{
		if(!visited[i])
		{
			output[round-1]='0'+numbers[i];
			visited[i]=1;
			permu(round+1);
			visited[i]=0;
		}
	}
}
int main()
{
	char tmp[5];
	while(getelem(tmp))
		numbers[n_num++]=atoi(tmp);
	numbers[n_num++]=atoi(tmp);
	permu(1);
	return 0;
}
