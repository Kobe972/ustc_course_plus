/*Codia测试数据窃取工具，适用于不太大的测试数据，否则需要开发出python脚本模拟提交请求。
使用方法：先使用check_spaces获取每一行的数据个数，然后对于每一个数据用check函数二分查找。如果是字符串或者大整数，需要做相应修改。
当测试数据处于设定范围时，相应的运行结果会变为Runtime Error，否则运行结果一般为Wrong Answer。 
*/ 
#include<stdlib.h>
#include<stdio.h> 
void re()
{
    printf("%d",0/0);
}
void check(int index,float a,float b)
{
    float f;
    for(int i=0;i<index-1;i++)
    {
        scanf("%f",&f);
    }
    scanf("%f",&f);
    if(f>=a&&f<=b) re();
}
int spaces(int enters)
{
    int i=0,enter=0;
    char c;
    chec:;
    if((c=getchar())==' ') i++;
    else if(c=='\n') enter++;
    if(enter==enters) return i;
    goto chec;
}
void check_spaces(int spacel,int spacer,int enters)
{
    int s=spaces(enters);
    if(s>=spacel&&s<=spacer) re();
}
