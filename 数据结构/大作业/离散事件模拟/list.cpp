#include"list.h"

Status MakeNode(Link &p,ElemType e)
{
	p=(Link)malloc(sizeof(LNode));
	if(!p) return ERROR;
	p->data=e;
	p->next=NULL;
	return OK;
}
void FreeNode(Link &p)
{
	free(p);
}
Status InitList(LinkList &L)
{
	L.head=(Link)malloc(sizeof(LNode));
	if(!L.head) return ERROR;
	L.tail=L.head;
	L.head->next=NULL;
	L.len=0;
	return OK;
}
Status DestroyList(LinkList &L)
{
	Link p=L.head,q;
	if(!p) return ERROR;
	q=p->next;
	while(p)
	{
		FreeNode(p);
		p=q;
		q=p->next;
	}
	return OK;
}
Status ClearList(LinkList &L)
{
	if(L.len==0) return OK;
	Link p=L.head,q;
	if(!p) return ERROR;
	p=p->next;
	q=p->next;
	while(p)
	{
		FreeNode(p);
		p=q;
		if(p) q=p->next;
	}
	L.tail=L.head;
	L.len=0;
	return OK;
}
Status InsFirst(Link h,Link s)
{
	if(h==NULL) return ERROR; //头结点不存在 
	s->next=h->next;
	h->next=s;
	return OK;
}
Status DelFirst(Link h,Link &q)
{
	if(h->next==NULL) return ERROR; //第一个结点不存在
	q=h->next;
	h->next=q->next;
	return OK;
}
Status Append(LinkList &L,Link s)
{
	L.tail->next=s;
	for(;L.tail->next!=NULL;L.tail=L.tail->next) L.len++;
	return OK;
}
Status Remove(LinkList &L,Link &q)
{
	if(L.len==0) return ERROR;
	Link p;
	for(p=L.head;p->next!=L.tail;p=p->next);
	p->next=NULL;
	q=L.tail;
	L.tail=p;
	L.len--;
	return OK;
}
Status InsBefore(LinkList &L,Link &p,Link s)
{
	if(!p) return ERROR;
	Link prev=L.head;
	if(p==L.head) return ERROR;
	for(;prev->next!=p;prev=prev->next);
	s->next=prev->next;
	prev->next=s;
	p=s;
	L.len++;
	return OK; 
}
Status InsAfter(LinkList &L,Link &p,Link s)
{
	if(!p) return ERROR;
	s->next=p->next;
	p->next=s;
	if(p==L.tail) L.tail=s;
	L.len++;
	return OK;
}
Status SetCurElem(Link &p,ElemType e)
{
	if(!p) return ERROR;
	p->data=e;
	return OK;
}
ElemType GetCurElem(Link p)
{
	return p->data;
}
Status ListEmpty(LinkList L)
{
	if(L.head==L.tail) return TRUE;
	return FALSE;
}
int ListLength(LinkList L)
{
	int len=0;
	for(Link p=L.head;p!=L.tail;p=p->next) len++;
	return len;
}
Position GetHead(LinkList L)
{
	return L.head;
}
Position GetLast(LinkList L)
{
	Link ret;
	for (ret = L.head; ret->next; ret = ret->next);
	return ret;
}
Position PriorPos(LinkList L,Link p)
{
	if(p==L.head||p==NULL||L.head==NULL) return NULL;
	Link ret;
	for(ret=L.head;ret&&ret->next!=p;ret=ret->next);
	return ret;
}
Position NextPos(LinkList L,Link p)
{
	return p->next;
}
Status LocatePos(LinkList L,int i,Link &p)
{
	if(i>L.len) return ERROR;
	p=L.head;
	for(int j=0;j<i;j++) p=p->next;
	return OK;
}
Position LocateElem(LinkList L,ElemType e,Status (*compare)(ElemType,ElemType))
{
	if(ListEmpty(L)) return NULL;
	Link p;
	for(p=L.head->next;p&&compare(p->data,e)!=0;p=p->next);
	return p;
}
Status ListTraverse(LinkList L,Status (*visit)(Link))
{
	if(ListEmpty(L)) return ERROR;
	for(Link p=L.head->next;p;p=p->next) visit(p);
}
Status OrderInsert(LinkList &L,ElemType e,int (*cmp)(ElemType,ElemType))
{
	Link p;
	MakeNode(p,e);
	if(ListEmpty(L)||cmp(e,L.tail->data)>=0) return InsAfter(L,L.tail,p);
	if(cmp(e,L.head->next->data)<=0) return InsAfter(L,L.head,p);
	for(Link pointer=GetHead(L)->next;pointer!=L.tail;pointer=pointer->next)
	{
		if(cmp(pointer->next->data,e)<0) continue;
		return InsAfter(L,pointer,p);
		break;
	}
}
