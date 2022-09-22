#include<iostream>
#include<cstring>
#include<windows.h>
using namespace std;
int stack[5],it,sign2[4],it2,sign[4],num[5],ans,ok=0;//sign2Îª¼Ó¼õ·ûºÅÕ» 
int main()
{
	for(int i=0;i<5;i++) cin>>num[i];
	cin>>ans;
	system("cls");
	stack[0]=num[0];
	it=it2=0;
	for(int i=1;i<=4;i++)
		for(int j=1;j<=4;j++)
			for(int k=1;k<=4;k++)
				for(int l=1;l<=4;l++)
				{ 
					sign[0]=i;sign[1]=j;sign[2]=k;sign[3]=l;
					for(int m=0;m<4;m++)
						if(sign[m]>=3) stack[it]=sign[m]==3?stack[it]*num[m+1]:stack[it]/num[m+1];
						else
						{
							stack[++it]=num[m+1];
							sign2[it2++]=sign[m];
						}
					if(sign2[0]) for(int m=0;m<=it-1;m++) stack[0]=sign2[m]==1?stack[0]+stack[m+1]:stack[0]-stack[m+1];
					if(stack[0]==ans)
					{
						ok=1;
						goto solved;
					 } 
					memset(stack,0,sizeof(stack));
					memset(sign2,0,sizeof(sign2));
					stack[0]=num[0];
					it=it2=0;
				}
	solved:;
	if(ok)
	{
		for(int i=0;i<4;i++)
		{
			cout<<num[i];
			switch(sign[i])
			{
				case 1:cout<<"+";break;
				case 2:cout<<"-";break;
				case 3:cout<<"*";break;
				case 4:cout<<"/";break;
			}
		}
		cout<<num[4]<<"="<<ans<<endl;
		system("pause");
	}
	else
		MessageBox(NULL,"No solution!","No Solution",MB_OK);
	return 0;
}

