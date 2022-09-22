#include<iostream>
using namespace std;
int main()
{
	char mat[16][16];
	for(int i=0;i<16;i++) cin>>mat[i];
	system("cls");
	for(int i=0;i<16;i++)
	{
		for(int j=0;j<16;j++)
			cout<<mat[j][i];
		cout<<endl;
	}
	return 0;
 } 
