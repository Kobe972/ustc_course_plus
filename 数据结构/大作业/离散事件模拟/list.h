#include<stdlib.h>
#include<stdio.h> 

#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR 0
#define INFEASIBLE -1
#define OVERFLOW -2

typedef int Status;

typedef struct
{
	int Occurtime;
	int NType;
}Event,ElemType;

typedef struct LNode
{
	ElemType data;
	struct LNode *next;
}*Link,*Position;

typedef struct
{
	Link head,tail;
	int len;
}LinkList;

Status MakeNode(Link &p,ElemType e);
void FreeNode(Link &p);
Status InitList (LinkList &L);
Status DestroyList(LinkList &L);
Status ClearList(LinkList &L);
Status InsFirst(Link h,Link s);
Status DelFirst(Link h,Link &q);
Status Append(LinkList &L,Link s);
Status Remove(LinkList &L,Link &q);
Status InsBefore(LinkList &L,Link &p,Link s);
Status InsAfter(LinkList &L,Link &p,Link s);
Status SetCurElem(Link &p,ElemType e);
ElemType GetCurElem(Link p);
Status ListEmpty(LinkList L);
int ListLength(LinkList L);
Position GetHead(LinkList L);
Position GetLast(LinkList L);
Position PriorPos(LinkList L,Link p);
Position NextPos(LinkList L,Link p);
Status LocatePos(LinkList L,int i,Link &p);
Position LocateElem(LinkList L,ElemType e,Status (*compare)(ElemType,ElemType));
Status ListTraverse(LinkList L,Status (*visit)(Link));
Status OrderInsert(LinkList &L,ElemType e,int (*cmp)(ElemType,ElemType));
