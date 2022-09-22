#include<iostream>
using namespace std;
int main()
{
	int a,n,m;
	cin>>a;
	n=(a*a+a)/2-1;
	m=(a*a-a)/2;
	system("cls");
	cout<<a<<"的三次方可以被表示成"<<2*m+1<<"至"<<2*n+1<<"这"<<n-m+1<<"个奇数相加"<<endl;
	system("pause");
	return 0;
}
