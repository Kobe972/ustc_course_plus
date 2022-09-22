#include<stdio.h>
#include<string.h>
int my_strcmp(char* a,char* b);
int my_strlen(char* a);
int main()
{
	char a[3],b[4]="123";
	a[0]='1';a[1]='2',a[2]='3';
	printf("%d",my_strcmp(a,b));
	return 0;
}
int my_strlen(char* a)
{
	if(a[sizeof(a)/2-1]==0) return strlen(a);
	else return sizeof(a)/2;
}
int my_strcmp(char* a,char* b)
{
	if(my_strlen(a)>my_strlen(b)) return 1;
	else if(my_strlen(a)<my_strlen(b)) return -1;
	else
	{
		for(int i=0;i<my_strlen(a);i++)
		{
			if(a[i]>b[i]) return 1;
			if(a[i]<b[i]) return -1;
		}
	}
	return 0;
}
