#include"queue.h"

Status InitQueue(LinkQueue &Q)
{
	Q.front=Q.rear=(QueuePtr)malloc(sizeof(QNode));
	if(!Q.front) exit(OVERFLOW);
	Q.front->next=NULL;
	return OK;
}

Status DestroyQueue(LinkQueue &Q)
{
	while(Q.front)
	{
		Q.rear=Q.front->next;
		free(Q.front);
		Q.front=Q.rear;
	}
	return OK;
}

Status ClearQueue(LinkQueue &Q)
{
	if(DestroyQueue(Q)!=OK) return ERROR;
	return InitQueue(Q);
}

Status QueueEmpty(LinkQueue Q)
{
	if(Q.front==Q.rear) return TRUE;
	return FALSE;
}

int QueueLength(LinkQueue Q)
{
	int len=0;
	for(QNode *p=Q.front;p!=Q.rear;p=p->next) len++;
	return len;
}

Status GetHead(LinkQueue Q,QElemType &e)
{
	if(QueueEmpty(Q)) return ERROR;
	e=Q.front->next->data;
	return OK; 
}

Status EnQueue(LinkQueue &Q,QElemType e)
{
	QueuePtr p=(QueuePtr)malloc(sizeof(QNode));
	if(!p) exit(OVERFLOW);
	p->data=e;
	p->next=NULL;
	Q.rear->next=p;
	Q.rear=p;
	return OK;
}

Status DeQueue(LinkQueue &Q,QElemType &e)
{
	if(Q.front==Q.rear) return ERROR;
	QueuePtr p=Q.front->next;
	e=p->data;
	Q.front->next=p->next;
	if(Q.rear==p) Q.rear=Q.front;
	free(p);
	return OK;
}

Status QueueTraverse(LinkQueue Q,void (*visit)(QNode *node))
{
	if(QueueEmpty(Q)) return ERROR;
	for(QueuePtr p=Q.front->next;p;p=p->next)
	{
		visit(p);
	}
	return OK;
}

