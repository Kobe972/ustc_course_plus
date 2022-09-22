#include<stdio.h>
#include<string.h>
#include<windows.h>
char str[50];
int main()
{
	scanf("%s",str);
	int dot,i;
	for(dot=0;dot<strlen(str);dot++)
		if(str[dot]=='.') break;
	if(str[0]!='.') putchar(str[0]);
	else putchar(str[1]);
		putchar('.');
	if(strlen(str)!=1)
	{
		for(i=1;i<strlen(str);i++)
		if(str[i]!='.'&&(!(str[0]=='.'&&i==1))) putchar(str[i]);
	}
	printf("E%d\n",dot-1);
	system("pause");
	return 0;	
}
 
