#include<iostream>
#include<cstring>
using namespace std;
void input(char* s);
int main()
{
	char str1[1000],str2[1000];
	char tmp;
	int ok=0;
	cout<<"str1=";
	input(str1);
	cout<<"str2=";
	input(str2);
	system("cls");
	for(int i=0;i<=strlen(str1)-strlen(str2);i++)
	{
		tmp=str1[i+strlen(str2)];
		str1[i+strlen(str2)]=0;
		if(strcmp(str1+i,str2)==0)
		{
			cout<<"The position of str2 in str1 is "<<i<<endl;
			system("pause");
			return 0;
		}
		str1[i+strlen(str2)]=tmp;
	}
	cout<<"Not Found"<<endl;
	system("pause");
	return 0;
}
void input(char* s)
{
	int i;
	for(i=0;(s[i]=getchar())!='\n';i++);
	s[i]=0;
}
