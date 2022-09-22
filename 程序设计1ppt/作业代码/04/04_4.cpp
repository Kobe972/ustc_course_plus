#include<stdio.h>
#include<time.h>
#include<stdlib.h>
struct T
{
	int hour,minute;
 } ;
T hours_later(T t0,int minutes)
{
	T t1;
	t1.hour=t0.hour+minutes/60;
	t1.minute=t0.minute+minutes%60;
	if(t1.minute>=60)
	{
		t1.minute-=60;
		t1.hour++;
	}
	return t1;
}
void print_T(T t0)
{
	printf("%02d%02d",t0.hour,t0.minute);
}
int gap(T t0,T t1)
{
	return (t1.hour-t0.hour)*60+t1.minute-t0.minute;
}
int main()
{
	int N,classes,interval,interv0;
	T begin,end,bundary{24,0};
	srand((unsigned)time(NULL));
	N=rand()%19+2;
	printf("出发城市\t到达城市\t出发时间\t到达时间\n");
	for(int i=0;i<N;i++)
	{
		for(int j=i+1;j<N;j++)
		{
			classes=rand()%7+4;
			interv0=rand()%1350;
			while(classes--)
			{
				interval=rand()%90+interv0;
				do
				{
					begin.hour=rand()%24;
					begin.minute=rand()%60;
				}while(gap(begin,bundary)<interval);
				end=hours_later(begin,interval);
				printf("%c\t\t%c\t\t%02d%02d\t\t%02d%02d\n",'A'+i,'A'+j,begin.hour,begin.minute,end.hour,end.minute);
			}
		}
	}
}
