#include<iostream>
#include<string.h>
#include<time.h>
#include<stdlib.h>
#define length 300
#define M 3
using namespace std;
void change(int bit,int* num,int change_to); 
void left(int* num,int n);
void right(int* num,int n);
void input(int* big);
void add(int* num1,int* num2,int* ans);
void minu(int* num1,int* num2,int* ans);
void multi(int* num1,int* num2,int* ans);
void divide(int* num1,int* num2,int* ans,int* mod);//num1/num2 
void power(int* num1,int* num2,int* ans,int* mod);
void output(int* big);
void init(int* num);
void get_rand(int* l,int* r,int* ans);
int great(int* num1,int* num2);//num1>num2? 
int get_bit(int* num,int bit);
bool Miller(int* num);
int one[length],zero[length];
int main()
{
	srand((unsigned)time(NULL));
	init(one);init(zero);
	change(1,one,1); 
	int big1[length],big2[length],ans[length],mod[length],prime[length];
	init(prime);
	do
	{
		init(prime);
		for(int i=1;i<=256;i++) change(i,prime,rand()%2);
		change(1,prime,1);
	}while(Miller(prime));
	output(prime);
	//printf("%d",Miller(prime));
	return 0;
}
void init(int* num)
{
	memset(num,0,length*sizeof(int));
	num[1]=M;
}
int great(int* num1,int* num2)
{
	if(num1[0]>num2[0]) return 1;
	else if(num1[0]<num2[0]) return 0;
	else
	{
		for(int i=num1[0];i>=1;i--)
		{
			if(get_bit(num1,i)>get_bit(num2,i)) return 1;
			if(get_bit(num1,i)<get_bit(num2,i)) return 0;
		}
	}
	return -1;
}
int get_bit(int* num,int bit)
{
	int position=(bit-1)/M;
	int precise=bit%M,power=1;
	if(precise==0) precise=M;
	while(--precise) power*=10;
	return num[length-position-1]%(power*10)/power;
}
void output(int* big)
{
	if(big[0]==0) printf("0");
	for(int i=big[0];i>=1;i--)
		printf("%d",get_bit(big,i));
}
void change(int bit,int* num,int change_to) //原数的个位为bit=1,该函数可把原数的第bit位改为change_to 
{
	int position,precise,power=1;
	position=(bit-1)/M;
	precise=bit%M;
	if(precise==0) precise=M;
	while(--precise) power*=10;
	*(num+length-position-1)-=power*((power-1)?*(num+length-position-1)%power/(power-1):*(num+length-position-1)%10);
	*(num+length-position-1)+=change_to*power;
	if(bit==num[0]&&change_to==0)
	{
	 while(get_bit(num,num[0])!=1) num[0]--; //注意N的维护 
	}
	if(bit>num[0]&&change_to==1) num[0]=bit;
}
void left(int* num,int n)
{
	int power=1;
	for(int i=1;i<=M;i++) power*=10;
	for(int i=1;i<=n;i++)
	{
		for(int j=1;j<length-1;j++) num[j]=(num[j]*10)%power+num[j+1]/(power/10);
		num[length-1]=(num[length-1]*10)%power;
	}
	if(num[0]) num[0]++;
}
void right(int* num,int n)
{
	int power=1;
	int non_zero=0;
	for(int i=1;i<length;i++) 
	{
		if(num[i]!=0) non_zero=1;
	}
	for(int i=1;i<=M;i++) power*=10;
	for(int i=1;i<=n;i++)
	{
		for(int j=length-1;j>1;j--) num[j]=num[j]/10+num[j-1]%10*(power/10);
		num[1]=num[1]/10;
	}
	if(non_zero) num[0]--;
}  
void input(int* big)
{
	int ch,l=0,first=1;
	while((ch=getchar())!='\n')
	{
		left(big,1);
		change(1,big,ch-'0');
		if(ch!='0') first=0;
		if(first!=1) l++;
	}
	if(first==1) big[0]=0;
	else big[0]=l;
}
void add(int* num1,int* num2,int* ans)
{
	int up=0;//进位
	int bit1,bit2;
	memset(ans,0,length*sizeof(int));
	ans[1]=M;
	for(int i=1;i<=max(num1[0],num2[0]);i++)
	{
		bit1=get_bit(num1,i);
		bit2=get_bit(num2,i);
		change(i,ans,(bit1+bit2+up)%2);
		up=(bit1+bit2+up)/2;
	}
	if(num1[0]==0&&num2[0]==0)
	{
		ans[0]=0;
		ans[1]=M;
		return;
	}
	if(up!=0) change(max(num1[0],num2[0])+1,ans,1);
	if(get_bit(ans,max(num1[0],num2[0])+1)>0) ans[0]=max(num1[0],num2[0])+1;
	else ans[0]=max(num1[0],num2[0]);
}
void minu(int* num1,int* num2,int* ans) //要求num1>=num2
{
	memset(ans,0,length*sizeof(int));
	ans[1]=M;
	int up=0;//进位
	int bit1,bit2;
	for(int i=1;i<=num1[0];i++)
	{
		bit1=get_bit(num1,i);
		bit2=get_bit(num2,i);
		change(i,ans,(bit1-bit2+up)%2==0?0:1);
		up=(bit1-bit2+up)<0?-1:0;
	}
	for(int i=num1[0];i>=1;i--)
	{
		if(get_bit(ans,i)!=0)
		{
			ans[0]=i;
			break;
		}
	}
	if(num1[0]==0&&num2[0]==0)
	{
		ans[0]=0;
		ans[1]=M;
		return;
	}
}
void multi(int* num1,int* num2,int* ans)
{
	int tmp[length];
	memset(ans,0,length*sizeof(int));
	memset(tmp,0,sizeof(tmp));
	ans[1]=tmp[1]=M;
	for(int i=num1[0];i>=1;i--)
	{
		left(ans,1);
		memset(tmp,0,sizeof(tmp));
		tmp[1]=M;
		for(int j=1;j<=get_bit(num1,i);j++)
		{
			add(ans,num2,tmp);
			memset(ans,0,length*sizeof(int));
			ans[1]=M;
			add(zero,tmp,ans);
		}
	}
}
void divide(int* num1,int* num2,int* ans,int* mod)
{
	/*printf("divide():");
	output(num1);
	printf("\n");
	output(num2);
	printf("\n");
	printf("num1[0]=%d\n",num1[0]);*/
	if(ans==NULL)
	{
		int an[length];
		ans=an;
	}
	memset(ans,0,length*sizeof(int));
	ans[1]=M;
	if(mod!=NULL)
	{
		memset(mod,0,length*sizeof(int));
		mod[1]=M;
	}
	if(great(num2,num1)==1)
	{
		if(mod!=NULL) for(int i=0;i<length;i++) mod[i]=num1[i];
		//system("pause");
		return;
	}  
	int sub_ans[length]={0,M},rep_num1[length],rep_num2[length],tmp[length],ans2[length],lefted=1;//lefted=tmp左移次数+1 
	for(int i=0;i<length;i++) rep_num1[i]=num1[i];
	for(int i=0;i<length;i++) tmp[i]=num2[i];
	while(great(rep_num1,tmp)==1) 
	{
		left(tmp,1);
		lefted++;
	}
	right(tmp,1);lefted--;
	change(lefted,ans,1);
	minu(rep_num1,tmp,rep_num2);
	divide(rep_num2,num2,tmp,mod);
	add(ans,tmp,ans2);
	add(ans2,zero,ans);
}
void power(int* num1,int* num2,int* ans,int* mod)
{
	printf("power()running\n");
	int tmp1[length],tmp2[length],rep_num1[length],rep_num2[length];
	for(int i=0;i<length;i++) rep_num1[i]=num1[i];
	for(int i=0;i<length;i++) rep_num2[i]=num2[i];
	memset(tmp1,0,length*sizeof(int));
	memset(tmp2,0,length*sizeof(int));
	memset(ans,0,length*sizeof(int));
	tmp1[1]=tmp2[1]=ans[1]=M;
	if(num2[0]==0)
	{
		change(1,ans,1);
		return;
	}
	if(num2[0]==1)
	{
		divide(num2,mod,NULL,ans);
		return;
	}
	if(get_bit(num2,1)==1)
	{
		right(rep_num2,1);
		power(rep_num1,rep_num2,tmp1,mod);
		multi(tmp1,tmp1,tmp2);
		divide(tmp2,mod,NULL,tmp1);
		multi(tmp1,num1,tmp2);
		divide(tmp2,mod,NULL,ans);
	}
	else
	{
		right(rep_num2,1);
		power(rep_num1,rep_num2,tmp1,mod);
		multi(tmp1,tmp1,tmp2);
		divide(tmp2,mod,NULL,ans);
	}
	return;
}
bool Miller(int* num)
{
		printf("miller()running\n");
	int rep_num[length],q[length],k=0,tmp[length],a[length],tmp2[length],sum2[length];
	init(q);
	init(one);
	init(zero);
	init(tmp);
	init(a);
	change(1,one,1);
	for(int i=0;i<length;i++) q[i]=rep_num[i]=num[i];
	change(1,q,0);
	while(!get_bit(q,1))
	{
		right(q,1);
		k++;
	}
	minu(q,one,sum2);
	get_rand(one,sum2,a);//1<a<num-1
	power(a,q,tmp,rep_num);
	if(great(tmp,one)==-1) return 1;
	change(1,rep_num,0);
	for(int j=1;j<k-1;j++)
	{
		multi(tmp,tmp,tmp2);
		divide(tmp2,num,NULL,tmp);
		if(great(tmp,rep_num)==-1) return 1;
	}
	return 0;
}
void get_rand(int* l,int* r,int* ans)
{
	int tmp[length];
	init(tmp);
	begin:;
	init(ans);
	minu(r,l,tmp);
	for(int i=1;i<=tmp[0];i++)
	{
		change(i,ans,rand()%2);
	}
	if(great(ans,tmp)==1) goto begin;
	add(l,ans,tmp);
	for(int i=0;i<length;i++) ans[i]=tmp[i];
}
