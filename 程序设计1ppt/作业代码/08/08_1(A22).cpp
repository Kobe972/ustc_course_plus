#include<stdio.h>
#define LEN(a,type) sizeof(a)/sizeof(type)
int main()
{
	int a[5]={1,0,0,8,6};
	printf("%d\n",LEN(a,int));
	while(1);
	return 0;
}
