#include<stdio.h>
#include<windows.h> 
#include<time.h>
#include<stdlib.h>
int array[10000],n;
void produce_random_array();
void insert_sort();
void output();
void swap(int *x,int *y);
int main()
{
	produce_random_array();
	insert_sort();
	output();
	system("pause");
	return 0;
}
void produce_random_array()
{
	srand((unsigned)time(NULL));
	n=rand()%10000;
	for(int i=0;i<n;i++)
		array[i]=rand();
}
void insert_sort()
{
	int sorted=0;
	while(sorted<n-1)
	{
		for(int i=sorted;i>=0;i--)
			if(array[i]>array[i+1]) swap(&array[i],&array[i+1]);
		sorted++;
	}
}
void output()
{
	for(int i=0;i<n;i++)
	{
		printf("%d ",array[i]);
	}
}
void swap(int *x,int *y)
{
	int temp;
	temp=*x;
	*x=*y;
	*y=temp;
}

