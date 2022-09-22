#include<iostream>
#include<cstring>
#include<conio.h>
#include<cstdio>
using namespace std;
class fucking_multiplication
{
	public:
	char num1[70]={0},num2[70]={0},ans[4900]="1";
	void perform();
};
void Reverse(char *s,int n){
    for(int i=0,j=n-1;i<j;i++,j--){
        char c=s[i];
        s[i]=s[j];
        s[j]=c;
    }
}
void fucking_multiplication::perform()
{
    int inum1[70]={0},inum2[70]={0},ians[4900]={0},len1,len2,len_ans,lenc,up;
    len1=strlen(num1);
	len2=strlen(num2);
    for (int i=0;i<=len1-1;i++) inum1[len1-i]=num1[i]-48;
    for (int i=0;i<=len2-1;i++) inum2[len2-i]=num2[i]-48;
    for (int i=1;i<=len1;i++)
    {
         up=0;                                               //用于存放进位
         for (int j=1;j<=len2;j++)                     //对乘数的每一位进行处理
         {
                     ians[i+j-1]=inum1[i]*inum2[j]+up+ians[i+j-1]; //当前乘积+上次乘积进位+原数      
                     up=ians[i+j-1]/10;
           ians[i+j-1] %= 10;
         }
         ians[i+len2]=up;                                  //进位
    }
    len_ans=len1+len2;
    while (ians[len_ans]==0&&len_ans>1)       //删除前导0
        len_ans--;
    for (int i=len_ans;i>=1;i--)
        ans[len_ans-i]=ians[i]+48;
    ans[len_ans]=0;
}
int main()
{
    char input[10];
    char ans[200]; 
    int n,dot,zeros;
    while(scanf("%s %d",input,&n)!=EOF)
    {
    	bool nonezeroed=0;
    	char inputcpy[10];
    	strcpy(inputcpy,input);
    	memset(ans,0,sizeof(ans));
    	fucking_multiplication mul;
    	for(int i=0;i<6;i++)
    		if(input[i]=='.')
    			dot=5-i;
		for(int i=5-dot;i<5;i++)
			input[i]=input[i+1];
		input[5]=0;
		strcpy(mul.num1,input);
		for(int i=0;i<n;i++)
		{
			strcpy(mul.num2,mul.ans);
			mul.perform();
		}
		int ite=0;
		bool zeroed=0;
		if(dot!=0)
		{
			ans[strlen(mul.ans)+1]=0;
			int i;
			for(i=strlen(mul.ans)-1;strlen(mul.ans)-i<=dot*n;i--)
			{
				if(i<0) ans[ite++]='0';
				else
				{
					if(mul.ans[i]!=48) zeroed=1;
					if(!zeroed&&mul.ans[i]==48)
						continue;
					nonezeroed=1;
					ans[ite++]=mul.ans[i];
				}
			}
			if(nonezeroed) ans[ite++]='.';
			for(;i>=0;i--)
				ans[ite++]=mul.ans[i];
			Reverse(ans,strlen(ans));
		}
		else
			strcpy(ans,mul.ans);
		printf("%s\n",ans);
		
	}
    return 0;
}

