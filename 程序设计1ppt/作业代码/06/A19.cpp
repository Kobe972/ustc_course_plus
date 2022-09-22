#include<stdio.h>
#include<string.h>
void substract(char* a,char* b); //substract b from a
int main()
{
	char a[500],b[500];
	printf("Input a and b:");
	scanf("%s%s",a,b);
	substract(a,b);
	printf("%s",a);
	return 0;
}
void substract(char* a,char* b)
{
	char ans[500];
	if(strlen(a)<strlen(b))
	{
		return;
	}
	for(int i=0;i<=strlen(a)-strlen(b);i++)
	{
		if(!strncmp(a+i,b,strlen(b)))
		{
			if(i>=1) strncpy(ans,a,i);
			if(i+strlen(b)<strlen(a)) strcat(ans,a+i+strlen(b));
			strcpy(a,ans);
			return;
		} 
	}
}
