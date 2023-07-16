#include<stdio.h>
#include<malloc.h>

//时间结构体
typedef struct data {
	int hour;
	int min;
}Time;
typedef struct link_node {
	char name[20];//作业名称
	Time arrive;//作业入井时间
	int zx;//作业执行时间
	Time JobTime;//作业调度时间
	int JobWait;//作业调度等待时间
	Time ProcessTime;//进程调度时间
	int ProcessWait;//进程调度等待时间
	Time finish;//作业完成时间
	int zz;//作业周转时间=作业完成时间-作业入井时间
	float zzxs;//作业带权周转时间=作业周转时间/作业执行时间
	struct link_node* next;//结点指针
	int finished;//标记作业完成
	float response;//作业响应化
}node;
typedef struct {
	node* front; //队首指针
	node* rear; //队尾指针
}Queue;  //队列结构体


//函数名：init   参数：无
Queue* init() {
	//功能：初始化队列，返回队列指针
	Queue* cc = (Queue*)malloc(sizeof(Queue));
	cc->front = NULL;
	cc->rear = NULL;
	return cc;
}
//函数名：insert  参数：Queue *cc, node *x
Queue* insert(Queue* cc, node* x) {
	//功能：将结点x按照到达时间先后按序插入队列cc
	if (cc->front == NULL && cc->rear == NULL) {
		cc->front = cc->rear = x;
		return cc;
	}
	if (x->arrive.hour < cc->front->arrive.hour || (x->arrive.hour == cc->front->arrive.hour && x->arrive.min <= cc->front->arrive.min)) {
		x->next = cc->front;
		cc->front = x;
		return cc;
	}
	node* p = cc->front;
	node* pre = NULL;
	while (p != NULL&&((x->arrive.hour > p->arrive.hour) || (x->arrive.hour == p->arrive.hour && x->arrive.min > p->arrive.min))) {
		pre = p;
		p = p->next;
	}
	x->next = p;
	pre->next = x;
	if (x->next == NULL) {
		cc->rear = x;
	}
	return cc;

}
//函数名：dele  参数：Queue *cc
Queue* dele(Queue* cc) {
	//功能：结点出队
	node* p = cc->front;
	while (p != NULL) {
		node* temp = p;
		p = p->next;
		free(temp);
	}
	cc->front = NULL;
	cc->rear = NULL;
	return cc;

}
//函数名：input   参数：Queue *cc
Queue* input(Queue* cc) {
	//功能：实现作业队列的输入
	printf("请输入你需要创建的作业数:");
	int num;
	scanf("%d",&num);
	printf("请输入%d个作业的:\n",num);
	printf("作业名	入井时间 运行时间(分钟):\n");
	node* newNode;
	for (int i = 0; i < num; i++) {
		newNode = (node*)malloc(sizeof(node));
		newNode->next = NULL;
		scanf("%s %d:%d %d",newNode->name, &newNode->arrive.hour, &newNode->arrive.min, &newNode->zx);
		insert(cc, newNode);
	}
	return cc;

}


/*
	计算相关时间
*/
void computer(Queue* cc) {
	node* p, * pre;
	p = cc->front;
	pre = NULL;
	while (p != NULL) {
		if (p == cc->front) {  //首节点
			p->JobTime.hour = p->arrive.hour;
			p->JobTime.min = p->arrive.min;
			p->JobWait = 0;

		}
		else {
			p->JobTime.hour = pre->finish.hour;
			p->JobTime.min = pre->finish.min;
			p->JobWait = p->JobTime.hour * 60 + p->JobTime.min - p->arrive.hour * 60 - p->arrive.min;
		}
		p->ProcessTime.hour = p->JobTime.hour;
		p->ProcessTime.min = p->JobTime.min;
		p->ProcessWait = 0;
		p->finish.hour = ((p->ProcessTime.hour) * 60 + p->ProcessTime.min + p->zx) / 60;
		p->finish.min = ((p->ProcessTime.hour) * 60 + p->ProcessTime.min + p->zx) % 60;
		p->zz = p->finish.hour * 60 + p->finish.min - p->arrive.hour * 60 - p->arrive.min;
		p->zzxs = 1.0 * p->zz / p->zx;

		pre = p;
		p = p->next;
	}
}

//函数名：FCFS   参数：Queue *cc
void FCFS(Queue* cc) {
	//功能：实现先来先服务算法
	computer(cc);
}



int timeCompare(Time t1, Time t2) {
	if (t1.hour < t2.hour) {
		return 1;
	}
	if (t1.hour > t2.hour) {
		return 0;
	}
	if (t1.min <= t2.min) {
		return 1;
	}
	else {
		return 0;
	}
}

int zxCompare(int t1, int t2) {
	if (t1 <= t2) {
		return 1;
	}
	else {
		return 0;
	}
}

Time computerTime(Time arrive, int zx) {
	int total = arrive.hour * 60 + arrive.min + zx;
	Time t;
	t.hour = total / 60;
	t.min = total % 60;
	return t;
}


//函数名：SJF   参数：Queue *cc
void SJF(Queue* cc) {
	//功能：实现短作业优先调度算法
	node* p = cc->front;
	p->finish.hour = ((p->arrive.hour) * 60 + p->arrive.min + p->zx) / 60;
	p->finish.min = ((p->arrive.hour) * 60 + p->arrive.min + p->zx) % 60;

	while (p != NULL) {
		node* tmp = p->next;
		node* pos = NULL;
		while (tmp != NULL && timeCompare(tmp->arrive, p->finish)) {
			pos = tmp; // 找到执行时，到达的最后一个队列
			tmp = tmp->next;
		}
		node* qre, *q, *tmr;
		qre = p;
		q = p->next;
		tmr = p;
		tmp = p->next;

		while (pos != NULL && q != pos) {
			if (!zxCompare(q->zx, q->next->zx)) {
				tmr = q;
				tmp = q->next;
				if (q->next == pos) {
					break;
				}

			}
			
			qre = q;
			q = q->next;
			
		}
		if (tmp != NULL) {
			tmr->next = tmp->next;
			tmp->next = p->next;
			p->next = tmp;
		}

		tmp = p;
		p = p->next;
		if (p == NULL) {
			continue;
		}
		if (timeCompare(p->arrive, tmp->finish)) {
			p->finish = computerTime(tmp->finish, p->zx);
		}
		else {
			p->finish = computerTime(p->arrive, p->zx);
		}
	}
	computer(cc);  //计算结果

}
//函数名：HRRF    参数：Queue *cc
void HRRF(Queue* cc) {
	//功能：实现最高响应比优先算法
	node* p = cc->front;
	node *q, *qre;
	node* pos, * posPre;
	double rate; //最高响应比
	while (p != NULL) {
		if (p == cc->front) {
			p->finish = computerTime(p->arrive, p->zx);
		}
		qre = p;
		q = p->next;
		posPre = p;
		pos = p->next;

		if (q != NULL) {
			rate = 1 + (1.0 * (p->finish.hour*60+p->finish.min- q->arrive.hour * 60- q->arrive.min) / q->zx);
		}
		double tmpRate;
		while (q != NULL) {
			tmpRate = 1 + (1.0 * (p->finish.hour * 60 + p->finish.min - q->arrive.hour * 60 - q->arrive.min) / q->zx);
			if(tmpRate > rate) {
				rate = tmpRate;
				pos = q;
				posPre = qre;
			}
			qre = q;
			q = q->next;
		}
		if (pos != NULL && pos != p->next) { //交换结点
			posPre->next = pos->next;
			pos->next = p->next;
			p->next = pos;
		}
		if (pos != NULL) {
			pos->finish.hour = (p->finish.hour * 60 + p->finish.min - pos->arrive.hour * 60 - pos->arrive.min + pos->zx) / 60;
			pos->finish.min = (p->finish.hour * 60 + p->finish.min - pos->arrive.hour * 60 - pos->arrive.min + pos->zx) % 60;

		}

		p = p->next;
	}

	computer(cc);


}
//函数名：output   参数：Queue *cc
void output(Queue* cc) {
	//功能：实现调度结果的输出
	int count=0;
	float zzxs = 0, zz = 0;
	printf("名字 入井时间 运行时间 作业调度时间 作业调度等待时间 进程调度时间 进程调度等待时间 完成时间 周转时间 带权周转系数:\n");
	node* p = cc->front;
	while (p != NULL) {
		printf("%s\t%d:%02d\t%d\t%d:%02d\t\t%d\t\t%d:%02d\t\t\t%d\t%d:%02d\t%d\t%0.4f\n",p->name,p->arrive.hour,p->arrive.min,p->zx,p->JobTime.hour,p->JobTime.min,p->JobWait,p->ProcessTime.hour,p->ProcessTime.min,p->ProcessWait,p->finish.hour,p->finish.min,p->zz,p->zzxs);
		zz += p->zz;
		zzxs += p->zzxs;
		p = p->next;
		count++;
	}
	zz = 1.0 * zz / count;
	zzxs = 1.0 * zzxs / count;
	printf("系统平均周转时间:\t\t\t\t\t\t\t\t\t\t%0.2f\n",zz);
	printf("系统带权平均周转系数为:\t\t\t\t\t\t\t\t\t\t\t%0.4f\n",zzxs);

}


//函数名：menu  参数：无
void menu() {
	//功能：展示菜单，选择相应算法
	printf("***************作业调度***************\n");
	printf("		*		1.FCFS		*\n");
	printf("		*		2.SJF		*\n");
	printf("		*		3.HRRF		*\n");
	printf("		*		0.退出		*\n");
	printf("请输入要运行的操作号:");

}
int main() {
	Queue* me;
	int x, y;
	printf("请输入操作(1:开始进程调度；0:结束进程)：");
	scanf("%d", &x);
	while (x) {
		me = init();
		me = input(me);

		menu();
		scanf("%d", &y);
		switch (y) {
		case 1: 
			FCFS(me); 
			printf("\n模拟作业FCFS调度过程输出结果:\n");
			output(me);
			dele(me);
			break;
		case 2: 
			SJF(me); 
			printf("\n模拟作业SJF调度过程输出结果:\n");
			output(me);
			dele(me);
			break;
		case 3: 
			HRRF(me); 
			printf("\n模拟作业HRRF调度过程输出结果:\n");
			output(me);
			dele(me);
			break;
		case 0: 
			printf("操作结束！"); 
			exit(1);
		default: 
			printf("输入操作号错误！\n"); 
			break;
		}
		
		printf("\n请输入操作(1:开始进程调度；0:结束进程)：");
		scanf("%d", &x);
	}
	printf("操作结束！");
}


/*
测试数据：

1 4
JOB2      8:50         50
JOB1      8:00         120
JOB4      9:50         20
JOB3      9:00         10
1
4
JOB2      8:50         50
JOB1      8:00         120
JOB4      9:50         20
JOB3      9:00         10
2
4
JOB2      8:50         50
JOB1      8:00         120
JOB4      9:50         20
JOB3      9:00         10
3
*/