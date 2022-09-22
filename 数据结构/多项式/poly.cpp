#include<stdlib.h>
#include<stdio.h>
#include<math.h>

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;
typedef struct
{
	float coef;
	int expn;
}term, ElemType;
typedef struct LNode
{
	ElemType data;
	struct LNode* next;
}*Link, * Position;
typedef struct
{
	Link head, tail;
	int len;
}LinkList;
typedef LinkList polynomial;

Status MakeNode(Link& p, ElemType e);
void FreeNode(Link& p);
Status InitList(LinkList& L);
Status InsFirst(Link h, Link s);
Status DelFirst(Link h, Link& q);
Status Append(LinkList& L, Link s);
Status ListEmpty(LinkList L);
Status SetCurElem(Link& p, ElemType e);
ElemType GetCurElem(Link p);
Position GetHead(LinkList L);
Status LocateElem(LinkList L, ElemType e, Position& q, int (*compare)(ElemType, ElemType));
Position NextPos(LinkList L, Link p);

void CreatePolyn(polynomial& P);
void DestroyPolyn(polynomial& P);
void PrintPolyn(polynomial P);
int PolyLength(polynomial P);
void AddPolyn(polynomial& Pa, polynomial& Pb);
void MultiplyPolyn(polynomial& Pa, polynomial& Pb);
void PrintPolyn(polynomial P);
double Value(polynomial P, double x);
void Derivative(polynomial& P);
int cmp(term a, term b)
{
	if (a.expn == b.expn) return 0;
	if (a.expn > b.expn) return 1;
	return -1;
}

Status InitList(LinkList& L)
{
	L.head = (Link)malloc(sizeof(LNode));
	if (!L.head) exit(OVERFLOW);
	L.head->next = NULL;
	L.tail = L.head;
	L.len = 0;
	return OK;
}
Status MakeNode(Link& p, ElemType e)
{
	p = (Link)malloc(sizeof(LNode));
	if (!p) exit(OVERFLOW);
	p->data = e;
	return OK;
}
void FreeNode(Link& p)
{
	free(p);
}
Status InsFirst(Link h, Link s)
{
	s->next = h->next;
	h->next = s;
	return OK;
}
Status DelFirst(Link h, Link& q)
{
	if (h->next == NULL) return ERROR;
	q = h->next;
	h->next = h->next->next;
	return OK;
}
Status Append(LinkList& L, Link s)
{
	Link p;
	for (p = L.head; p->next; p=p->next);
	p->next = s;
	return OK;
}
Status ListEmpty(LinkList L)
{
	if (L.head->next == NULL) return TRUE;
	else return FALSE;
}
Status SetCurElem(Link& p, ElemType e)
{
	if (!p)
	{
		p = (Link)malloc(sizeof(LNode));
		if (!p) exit(OVERFLOW);
	}
	p->data = e;
	return OK;
}
ElemType GetCurElem(Link p)
{
	return p->data;
}
Position GetHead(LinkList L)
{
	return L.head;
}
Status LocateElem(LinkList L, ElemType e, Position& q, int (*compare)(ElemType, ElemType))
{
	for (q = L.head; q; q = q->next)
	{
		if (compare(q->data, e) == 0) return TRUE;
		if (!q->next) return FALSE;
		if (compare(q->data, e) > 0 && compare(q->next->data, e) < 0) return FALSE;
	}
}
Position NextPos(LinkList L, Link p)
{
	return p->next;
}

void CreatePolyn(polynomial& P)
{
	InitList(P);
	Link h = GetHead(P);
	term e;
	e.coef = 0.0;
	e.expn = 99999;
	SetCurElem(h, e);
	int first = 1;
	if (first)
	{
		scanf("%f", &e.coef);
		if (e.coef == 0.0 && getchar() == '\n')
		{
			return;
		}
		scanf("%d", &e.expn);
		Link s;
		MakeNode(s, e);
		InsFirst(P.head, s);
		if (getchar() == '\n') return;
		first = 0;
	}
	while (1)
	{
		scanf("%f%d", &e.coef, &e.expn);
		Link q, s;
		if (!LocateElem(P, e, q, cmp))
		{
			if (MakeNode(s, e)) InsFirst(q, s);
		}
		else q->data.coef += e.coef;
		if (getchar() == '\n') return;
	}
}
void DestroyPolyn(polynomial& P)
{
	if (!P.head) return;
	for (Link p = P.head; p; )
	{
		Link q = p->next;
		free(p);
		p = q;
	}
}
void AddPolyn(polynomial& Pa, polynomial& Pb)
{
	Link ha = GetHead(Pa), hb = GetHead(Pb);
	Link qa = NextPos(Pa, ha), qb = NextPos(Pb, hb);
	float sum;
	while (qa && qb)
	{
		ElemType a = GetCurElem(qa), b = GetCurElem(qb);
		switch (cmp(a, b))
		{
		case -1:
			DelFirst(hb, qb);
			InsFirst(ha, qb);
			qb = NextPos(Pb, hb);
			ha = NextPos(Pa, ha);
			break;
		case 0:
			sum = a.coef + b.coef;
			if (sum != 0.0)
			{
				SetCurElem(qa, term{ sum,a.expn });
				ha = qa;
			}
			else
			{
				DelFirst(ha, qa);
				FreeNode(qa);
			}
			DelFirst(hb, qb);
			FreeNode(qb);
			qb = NextPos(Pb, hb);
			qa = NextPos(Pa, ha);
			break;
		case 1:
			ha = qa;
			qa = NextPos(Pa, qa);
			break;
		}
	}
	if (!ListEmpty(Pb)) Append(Pa, qb);
	FreeNode(hb);
}
void SubtractPolyn(polynomial& Pa, polynomial& Pb)
{
	Link ha = GetHead(Pa), hb = GetHead(Pb);
	Link qa = NextPos(Pa, ha), qb = NextPos(Pb, hb);
	for (Link p = qb; p; p = p->next) p->data.coef *= -1;
	AddPolyn(Pa, Pb);
}
void PrintPolyn(polynomial P)
{
	if (P.head->next == NULL)
	{
		printf("0.0\n");
		return;
	}
	for (Link p = P.head->next; p; p = p->next)
	{
		if (p->data.coef == 0) continue;
		if (p->data.coef != 1&& p->data.coef != -1)
		{
			if (p != P.head->next&&p->data.coef>0) printf("+");
			printf("%.1lf", p->data.coef);
		}
		else
		{
			if (p->data.coef == 1 && p != P.head->next) printf("+");
		}
		if (p->data.expn != 0)
		{
			if(p->data.expn !=1) printf("x^%d",p->data.expn);
			else printf("x", p->data.expn);
		}
		else if (p->data.coef == 1 || p->data.coef == -1)
		{
			printf("%.1lf", p->data.coef);
		}
	}
	printf("\n");
}
double Value(polynomial P, double x)
{
	double sum = 0;
	for (Link p = P.head->next; p; p = p->next)
	{
		sum += p->data.coef * pow(x, p->data.expn);
	}
	return sum;
}
void Derivative(polynomial& P)
{
	Link q;
	for (Link p = P.head; p&&p->next; p = p->next)
	{
		if (p->next->data.expn == 0||p->next->data.coef==0) DelFirst(p, q);
		else
		{
			p->next->data.coef *= p->next->data.expn;
			p->next->data.expn--;
		}
	}
}
int PolyLength(polynomial P)
{
	int sum = 0;
	for (Link p = P.head->next; p; p = p->next)
	{
		if (p->data.coef != 0) sum++;
	}
	return sum;
}
void Copy(polynomial& Pa, polynomial Pb)
{
	InitList(Pa);
	Link h = GetHead(Pa);
	term e;
	e.coef = 0.0;
	e.expn = 99999;
	SetCurElem(h, e);
	for (Link p = Pb.head->next; p; p = p->next)
	{
		Link q, s;
		e.coef = p->data.coef;
		e.expn = p->data.expn;
		if (!LocateElem(Pa, e, q, cmp))
		{
			if (MakeNode(s, e)) InsFirst(q, s);
		}
		else q->data.coef += e.coef;
	}
}
void MultiplyPolyn(polynomial& Pa, polynomial& Pb)
{
	Link del;
	int lenb = PolyLength(Pb);
	polynomial Pc,Pd;
	Copy(Pc, Pa);
	Copy(Pd, Pa);
	InitList(Pa);
	Link h = GetHead(Pa);
	term e;
	e.coef = 0.0;
	e.expn = 99999;
	SetCurElem(h, e);
	for (Link q = Pb.head; q && q->next; q = q->next)
	{
		for (Link p = Pc.head; p->next; p = p->next)
		{
			if (q->next->data.coef != 0)
			{
				p->next->data.coef *= q->next->data.coef;
				p->next->data.expn += q->next->data.expn;
			}
		}
		AddPolyn(Pa, Pc);
		Copy(Pc, Pd);
	}
}

int main()
{
	int op;
	polynomial poly[200],tmp;
	int num = 0,tmp1,tmp2;
	while (1)
	{
		printf("1:创建多项式\n2:打印多项式\n3:多项式求和\n4:多项式相减\n5:多项式相乘\n6:求函数值\n7:求导\n8:退出\n");
		printf("请输入指令：\n");
		scanf("%d", &op);
		system("cls");
		switch (op)
		{
		case 1:
			printf("按系数1、指数1、系数2、指数2...的形式输入：\n");
			CreatePolyn(poly[num++]);
			break;
		case 2:
			printf("序号\t多项式\n");
			for (int i = 0; i < num; i++)
			{
				printf("%d\t", i);
				PrintPolyn(poly[i]);
			}
			break;
		case 3:
			printf("请依次输入两个多项式序号：\n");
			scanf("%d%d", &tmp1, &tmp2);
			Copy(tmp, poly[tmp2]);
			AddPolyn(poly[tmp1], tmp);
			PrintPolyn(poly[tmp1]);
			break;
		case 4:
			printf("请依次输入两个多项式序号：\n");
			scanf("%d%d", &tmp1, &tmp2);
			Copy(tmp, poly[tmp2]);
			SubtractPolyn(poly[tmp1], tmp);
			PrintPolyn(poly[tmp1]);
			break;
		case 5:
			printf("请依次输入两个多项式序号：\n");
			scanf("%d%d", &tmp1, &tmp2);
			Copy(tmp, poly[tmp2]);
			MultiplyPolyn(poly[tmp1], tmp);
			PrintPolyn(poly[tmp1]);
			break;
		case 6:
			printf("请输入多项式序号：\n");
			scanf("%d", &tmp1);
			printf("输入自变量值:\n");
			double x;
			scanf("%lf", &x);
			printf("%.1lf\n", Value(poly[tmp1], x));
			break;
		case 7:
			printf("请输入多项式序号：\n");
			scanf("%d", &tmp1);
			Derivative(poly[tmp1]);
			PrintPolyn(poly[tmp1]);
			break;
		case 8:
			for (int i = 0; i < num; i++)
			{
				DestroyPolyn(poly[i]);
			}
			return 0;
		default:
			printf("指令错误！\n");
			break;
		}
		system("pause");
		system("cls");
	}
	return 0;
}