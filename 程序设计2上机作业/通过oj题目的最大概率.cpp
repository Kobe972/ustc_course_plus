#include<iostream>
#include<stdlib.h>
using namespace std;
int lines[100];
int max_lens[100]={0};
int n_lines=0; //一共有几行 
int getelem(char* s) //得到被空格split后的结果，如果separate token是空格，返回1，否则返回0 
{
	int i=0;
	char c;
	while((c=getchar())!=' '&&c!='\n'&&c!=EOF)
		s[i++]=c;
	s[i]=0;
	return c==' '?1:0;
}
int max_line(int index) //使用动态规划求答案 
{
	if(index==n_lines-2) max_lens[index]=max(lines[index],lines[index+1]);
	else if(index==n_lines-1) max_lens[index]=lines[index];
	else max_lens[index]=max(lines[index]+(max_lens[index+2]==0?max_line(index+2):max_lens[index+2]),max_lens[index+1]==0?max_line(index+1):max_lens[index+1]);
    return max_lens[index];
}
int main()
{
	char tmp[5];
	while(getelem(tmp))
		lines[n_lines++]=atoi(tmp);
	lines[n_lines++]=atoi(tmp);
	cout<<max_line(0)<<endl;
	return 0;
}
