#include<iostream>
#include<stdlib.h>
using namespace std; 
struct Student
{
	int ID,Chinese,total;
};
int comp(const void* stu1,const void* stu2)
{
	Student *s1=(Student*)stu1;
	Student *s2=(Student*)stu2;
	s1=(Student*) stu1;
	s2=(Student*) stu2;
	if(s1->total>s2->total) return -1;
	else if(s1->total<s2->total) return 1;
	else
	{
		if(s1->Chinese>s2->Chinese) return -1;
		else if(s1->Chinese<s2->Chinese) return 1;
		else
		{
			return s1->ID>s2->ID?1:-1;
		}
	}
}
int main()
{
	int n,Ch,En,Ma;
	cin>>n;
	Student stu[305];
	for(int i=1;i<=n;i++)
	{
		cin>>Ch>>Ma>>En;
		stu[i-1].ID=i;
		stu[i-1].Chinese=Ch;
		stu[i-1].total=Ch+Ma+En;
	}
	qsort((void*)stu,n,sizeof(Student),comp);
	for(int i=0;i<5;i++)
	{
		cout<<stu[i].ID<<" "<<stu[i].total<<endl;
	}
	return 0;
}

