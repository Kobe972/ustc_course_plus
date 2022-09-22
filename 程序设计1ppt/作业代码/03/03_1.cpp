#include<iostream>
#include<windows.h>
using namespace std;
int main()
{
	int height;
	cin>>height;
	system("cls");
	if(height>9)
	{
		MessageBox(NULL,"Invalid input!","Error",MB_OK);
		return 0;
	}
	for(int j=1;j<=height;j++)
	{
		for(int i=1;i<=height-j;i++) cout<<" ";
		for(int i=1;i<=j;i++) cout<<j<<" ";
		cout<<endl;
	}
	for(int j=height-1;j>=1;j--)
	{
		for(int i=1;i<=height-j;i++) cout<<" ";
		for(int i=1;i<=j;i++) cout<<j<<" ";
		cout<<endl;
	}
	system("pause");
	return 0;
}
