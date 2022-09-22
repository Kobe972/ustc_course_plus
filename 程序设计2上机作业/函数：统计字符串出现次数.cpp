#include<stdio.h>
#include<string.h>
int getline(char* s)
{
	char ch;
	int i=0;
	while((ch=getchar())!='\n')
	{
		if(ch==EOF) return -1;
		s[i++]=ch;
	}
	s[i]=0;
	return 0;
}
int main()
{
	char first[55],input[55],tmp[55];
	int cnt;
	memset(first,0,sizeof(first)); 
	getline(first);
	memset(input,0,sizeof(input));
	while(getline(input)!=-1)
	{
		memset(tmp,0,sizeof(tmp));
		cnt=0;
		for(int i=0;i<=(int)(strlen(input)-strlen(first));i++)
		{
			strncpy(tmp,input+i,strlen(first));
			if(strcmp(tmp,first)==0) cnt++;
		}
		printf("%d\n",cnt);
		memset(input,0,sizeof(input));
	}
	return 0;
}
