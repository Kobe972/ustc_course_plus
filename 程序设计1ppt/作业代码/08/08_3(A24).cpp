#include<stdio.h>
#define MyLpha(c) (c>='a'&&c<='z')
int main()
{
	char c;
	while(1)
	{
		scanf("%c",&c);
		if(!((c>='a'&&c<='z')||(c>='A'&&c<='Z')))
		{
			printf("%c",c);
			continue;
		} 
		if(MyLpha(c)) printf("%c",c-'a'+'A');
		else printf("%c",c-'A'+'a');
	}
	return 0;
}
