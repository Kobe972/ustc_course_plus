#include<stdio.h>
#include<string.h>
int main()
{
	char input[500];
	scanf("%s",input);
	for(int i=0,j=strlen(input)-1;i<j;i++,j--)
	{
		if(input[i]!=input[j])
		{
			printf("No\n");
			return 0;
		} 
	}
	printf("Yes\n");
	return 0;
 } 
