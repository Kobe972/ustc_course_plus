#include<stdlib.h>
#include<iostream>
using namespace std;
int main()
{
	double pi=3.141592653589793,ans=2.00,prec,n=1.00;
	char precise[20];
	cin>>precise;
	prec=atof(precise);
	while(pi-ans>=prec)
	{
		ans=ans*4.00*n*n/(2.00*n-1.00)/(2.00*n+1.00);
		n=n+1.00;
	}
	printf("%.15f\n",ans);
	system("pause");
	return 0;
}
