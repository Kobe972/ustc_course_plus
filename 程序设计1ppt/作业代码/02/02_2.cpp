#include<iostream>
using namespace std;
int main()
{
	unsigned long int a;
	int b;
	a=(1<<31);
	b=a;
	cout<<b<<endl;
	system("pause");
	return 0;
 } 
 
 //b增加到 2147483647后溢出，返回到 -2147483648 
