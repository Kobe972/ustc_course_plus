#include<iostream>
#include<string>
#define MAXN 10000
using namespace std;
class formula //表达式类 
{
	public:
		string expr=" "; //以字符串形式表示表达式 
		char last_op='n'; //最后一个运算的运算符，为n表示表达式是一个数 
};
class stack //手动写栈 
{
	public:
		formula top();
		void push(formula s);
		void pop();
	private:
		formula data[MAXN]; //与正常逆逆波兰式求值不同的是，采用表达式而不是结果当栈元素 
		int len=0;
};
formula stack::top()
{
	return data[len-1];
}
void stack::push(formula s)
{
	data[len++]=s;
}
void stack::pop()
{
	data[--len].expr=" ";
	data[len].last_op='n';
}
int main()
{
	stack expression;
	char input;
	while((input=getchar())!='\n')
	{
		string tmp=" ";
		tmp[0]=input;
		formula top_now; //将要压入栈顶的新表达式 
		if(tmp[0]>='0'&&tmp[0]<='9') //输入数字 
		{
			top_now.expr=tmp;
			top_now.last_op='n';
			expression.push(top_now);
		}
		else //输入运算符 
		{
			formula top1,top2;
			top1=expression.top();
			expression.pop();
			top2=expression.top();
			expression.pop();
			if(input=='+'||input=='-')
			{
				if(top1.last_op=='+'||top1.last_op=='-') top1.expr="("+top1.expr+")"; //考虑给top1两边加括号，后面类似 
				top_now.expr=top2.expr+tmp+top1.expr;
				top_now.last_op=input;
				expression.push(top_now);
			}
			else
			{
				if(top1.last_op!='n') top1.expr="("+top1.expr+")";
				if(top2.last_op=='+'||top2.last_op=='-') top2.expr="("+top2.expr+")";
				top_now.expr=top2.expr+tmp+top1.expr;
				top_now.last_op=input;
				expression.push(top_now);
			}
		}
	}
	cout<<expression.top().expr<<endl;
	return 0;
}
