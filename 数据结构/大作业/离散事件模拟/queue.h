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
	int ArrivalTime;
	int Duration;
	int Type; //0表示存款，1表示取钱
	int Amount; //数额
}QElemType;

typedef struct QNode
{
	QElemType data;
	struct QNode *next;
}QNode,*QueuePtr;

typedef struct
{
	QueuePtr front;
	QueuePtr rear;
}LinkQueue;

Status InitQueue(LinkQueue &Q);
Status DestroyQueue(LinkQueue &Q);
Status ClearQueue(LinkQueue &Q);
Status QueueEmpty(LinkQueue Q);
int QueueLength(LinkQueue Q);
Status GetHead(LinkQueue Q,QElemType &e);
Status EnQueue(LinkQueue &Q,QElemType e);
Status DeQueue(LinkQueue &Q,QElemType &e);
Status QueueTraverse(LinkQueue Q,void (*visit)(QNode *node));

