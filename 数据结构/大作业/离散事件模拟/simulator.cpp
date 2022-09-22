#include"list.h"
#include"queue.h"
#include<time.h>
#include<Windows.h>

typedef LinkList EventList;

EventList ev; //事件表
Event en; //事件
LinkQueue q[102]; //100个客户队列和1个检查队列（q[101]）
QElemType customer; //客户记录
int TotalTime, CustomerNum, CloseTime, NumPorts; //累计客户逗留时间，客户数，停止一页时间，窗口数
int Assets; //银行总资产
int Check = 1; //是否值得检查

int cmp(Event a, Event b);
void Random(int& num1, int& num2);
void OpenForDay();
void CustomerArrived();
void CustomerDeparture();
void Bank_Simulation(int ClostTime, int _Assets);

int cmp(Event a, Event b)
{
	if (a.Occurtime > b.Occurtime) return 1;
	else if (a.Occurtime == b.Occurtime) return 0;
	else return -1;
}

void Random(int& num1, int& num2)
{
	num1 = rand() % 50 + 50;
	num2 = rand() % 100;
}

void OpenForDay()
{
	TotalTime = 0;
	CustomerNum = 0;
	InitList(ev);
	en.Occurtime = 3;
	en.NType = 0;
	OrderInsert(ev, en, cmp);
	for (int i = 1; i <= 101; i++) InitQueue(q[i]);
}

void CustomerArrived()
{
	int durtime, intertime;
	CustomerNum++;
	Random(durtime, intertime);
	int t = en.Occurtime + intertime;
	if (t < CloseTime) OrderInsert(ev, { t,0 }, cmp);
	int min = QueueLength(q[1]);
	int index = 1;
	for (int i = 2; i <= NumPorts; i++)
	{
		if (min > QueueLength(q[i]))
		{
			min = QueueLength(q[i]);
			index = i;
		}
	}
	EnQueue(q[index], { en.Occurtime,durtime,rand()%2,rand()%20000+1 });
	printf("时刻%d：一个客户到窗口%d，他需要%d分钟才能办理完业务。\n",en.Occurtime, index, durtime);
	if (QueueLength(q[index]) == 1) OrderInsert(ev, { en.Occurtime + durtime,index }, cmp);
}

void CustomerDeparture()
{
	int NType = en.NType,len=QueueLength(q[101]);
	DeQueue(q[NType], customer);
	if (customer.Type == 0)
	{
		Assets += customer.Amount;
		Check = 1;
		TotalTime += en.Occurtime - customer.ArrivalTime;
		printf("时刻%d：%d号窗口有客户存款%d元，现在银行有%d元\n", en.Occurtime, NType, customer.Amount, Assets);
	}
	else
	{
		if (customer.Amount <= Assets)
		{
			Assets -= customer.Amount;
			TotalTime += en.Occurtime - customer.ArrivalTime;
			printf("时刻%d：%d号窗口有客户取款%d元，现在银行有%d元\n", en.Occurtime, NType, customer.Amount, Assets);
		}
		else
		{
			EnQueue(q[101], customer);
			printf("时刻%d：%d号窗口有客户取款%d元，但钱不够，已转移到101号窗口。\n", en.Occurtime, NType, customer.Amount);
		}
		Check = 0;
	}
	if (Check)
	{
		for (int i = 0; i < len; i++)
		{
			QElemType tmp;
			DeQueue(q[101], tmp);
			if (tmp.Amount <= Assets)
			{
				Assets -= tmp.Amount;
				TotalTime += en.Occurtime - tmp.ArrivalTime;
				printf("时刻%d：101号窗口有客户取款%d元，现在银行有%d元\n", en.Occurtime, tmp.Amount, Assets);
			}
			else
			{
				EnQueue(q[101], tmp);
			}
		}
	}
	if (!QueueEmpty(q[NType]))
	{
		GetHead(q[NType], customer);
		OrderInsert(ev, { en.Occurtime + customer.Duration,NType }, cmp);
	}
}

void Bank_Simulation(int _CloseTime, int _Assets)
{
	CloseTime = _CloseTime;
	Assets = _Assets;
	while (1)
	{
		printf("请输入窗口数：");
		scanf("%d", &NumPorts);
		if (NumPorts <= 0 || NumPorts > 100) MessageBox(NULL, "窗口数必须在1到100之间！", "错误", MB_OK);
		else break;
		system("cls");
	}
	OpenForDay();
	Link p;
	while (!ListEmpty(ev))
	{
		DelFirst(GetHead(ev), p);
		ev.tail = GetLast(ev);
		ev.len--;
		en = GetCurElem(p);
		if (en.Occurtime > CloseTime)
		{
			break;
		}
		if (en.NType == 0) CustomerArrived();
		else CustomerDeparture();
	}
	for (int i = 1; i <= 101; i++)
	{
		if (!QueueEmpty(q[i])) printf("时刻%d：银行关门，但是%d号窗口还有%d位顾客，现要将他们驱离。\n", CloseTime, i, QueueLength(q[i]));
		while (!QueueEmpty(q[i]))
		{
			QElemType left;
			DeQueue(q[i], left);
			TotalTime += CloseTime - left.ArrivalTime;
		}
	}
	printf("The average Time is %f\n", (float)TotalTime / CustomerNum);
}

int main()
{
	srand((unsigned)time(NULL));
	Bank_Simulation(600, 10000);
	return 0;
}