#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
using namespace std;

typedef struct data{
	int hour;
	int minute;
}time;

typedef struct node{
	
	int id;//进程编号 
	char name[20];//进程名 
	int good;//优先级 
	time arrive;//到达就绪队列的时间 
	int zx;//执行时间 
	time start;//开始执行时间 
	time finish;//执行完成时间 
	int zz;//周转时间=执行完成时间-到达就绪队列时间 
	float zzxs;//带权周转时间=周转时间/执行时间 
	struct node* next;
	
}Node;

typedef struct Queue{
	
	Node* front = NULL;
	Node* tail = NULL;
	
}Queue;

Queue* init(){
	
	Queue* p = (Queue*)malloc(sizeof(Queue));
	p->front = NULL;
	p->tail = NULL;
	return p;
	
} 
//函数名：timecompare()          参数:tt 当前时间, p 进程到达时间
bool timecompare(time tt,time p){//tt<p(时间没到) false    tt >= p true 
	//函数功能：比较进程到达时间和当前时间，若小于则返回false，否则返回true
	if(tt.hour<p.hour) {
        return false;
    }
    if(tt.hour>p.hour) {
        return true;
    }
    if(tt.minute<p.minute) {
        return false;
    }else{
        return true;
    }
}
//函数名：timecompare2()          参数:tt 当前时间, p 进程到达时间
bool timecompare2(time tt,time p){//tt<=p(时间没到) false    tt > p true 
	//函数功能：比较进程到达时间和当前时间，若小于等于则返回false，否则返回true
	if(tt.hour>p.hour) {
        return true;
    }
    if(tt.hour<p.hour) {
        return false;
    }
    if(tt.minute<=p.minute) {
        return false;
    }else{
        return true;
    }
}
//函数名：Levelcompare()          参数:p,q 进程
bool Levelcompare(Node* p,Node* q){
	//函数功能：比较p,q的优先级，p的优先级高则返回true，低则返回false，否则比较到达时间，p先或同时到达则返回true，反之则false
	if(p->good>q->good) {
        return true;
    }
    if(p->good<q->good) {
        return false;
    }
    if(timecompare2(p->arrive,q->arrive)==false){
        return true;
    }else{
        return false;
    }
}
//函数名：LevelSorted()          参数:que 进程队列指针
void LevelSorted(Queue* que,int n){
//函数功能：对进程队列按优先级排序	

	Node * p = que->front;
	Node * q = que->front->next;
	/*有几个数据就-1；比如x 个i<x-1*/
	for(int i=0;i<n;i++)
	{ 
		while((q!=NULL)&&(p!=NULL))
		{ 
			if(p->good<q->good)
			{
				/*头结点和下一节点的交换，要特殊处理，更新新的头head*/
				if (p == que->front)
				{
					p->next = q->next;
					q->next = p;
					que->front = q;
					
					/*这里切记要把p,q换回来，正常的话q应该在p的前面，进行的是p,q的比较
					*但是经过指针域交换之后就变成q,p.再次进行下一次比较时，
					*就会变成q,p的数据域比较。假如原本p->data > q->data,则进行交换。变成q->data和p->data比较，
					*不会进行交换，所以排序就会错误。有兴趣的可以调试下。
					*/	
					Node*temp=p; 
					p=q;
					q=temp;		
				}
				/*处理中间过程，其他数据的交换情况，要寻找前驱节点if (p != phead)*/
				else 
				{
					/*p，q的那个在前，那个在后。指针域的连接画图好理解一点*/
					if (p->next == q)
					{
						/*寻找p的前驱节点*/
						Node *ppre = FindPreNode(p);
						/*将p的下一个指向q的下一个*/
						p->next = q->next;
						/*此时q为头结点，让q的下一个指向p，连接起来*/
						q->next = p;
						/*将原来p的前驱节点指向现在的q，现在的q为头结点*/
						ppre->next = q;
						Node*temp=p; 
						p=q; 
						q=temp;
					}
					else if (q->next == p)
					{
						Node *qpre = FindPreNode(q);
						q->next = p->next;
						p->next = q;
						qpre->next = p;
						Node*temp=p;
						p=q; 
						q=temp;
					}									
				}		
			}
			/*地址移动*/
			p = p->next;
			q = q->next;
		}
		/*进行完一轮比较后，从头开始进行第二轮*/
		p = que->front;
		q = que->front->next;	
	}

	
}

//函数名：ComputeTime()    参数:tt 当前时间的指针,q 当前进程的指针
time ComputeTime(time* tt,Node* q){
//函数功能：更新当前时间和进程的各项时间
	int num=q->start.hour*60+q->start.minute+q->zx; 
	q->finish.hour=num/60;
	q->finish.minute=num%60;
	q->zz=num-q->arrive.hour*60-q->arrive.minute;
	q->zzxs=q->zz/q->zx;
	return q->finish;
}
// 函数名：priority()    参数:que进程队列指针,tt当前时间 n 进程数
Queue* priority(Queue *que,time tt,int n){
//函数功能：进行优先级进程调度，并同时更新当前时间。
	Node* p=que->front;
	
}
//函数名：Print()    参数:que进程队列指针, n 进程数
void Print(Queue* que,int n){
	//函数功能：打印输出进程优先进程调度结果
	Node* p;
    p=que->front;
    printf("模拟进程优先进程调度过程输出结果：\n");
    printf("id号\t名字\t优先级\t到达时间\t执行时间(分钟)\t开始时间\t完成时间\t周转时间(分钟)\t带权周转系数\n");
    while(p!=NULL) {
        printf("%4d\t%s\t%2d\t%-2d:%-2d\t%2d\t%-2d:%-2d\t%-2d:%-2d\t%2d\t%2f\n",p->id,p->name,p->good,p->arrive.hour,p->arrive.minute,p->zx,p->start.hour,p->start.minute,p->finish.hour,p->finish.minute,p->zz,p->zzxs);
        p=p->next;
    }



}
//函数名：ScanIn()    参数:wait进程队列指针, n 进程数
time ScanIn(Queue* wait,int n){
	//函数功能：输入进程信息，返回最早的进程到达时间
	printf("请输入进程的参数:\n");
	printf("id	名字	优先级	到达	执行(分钟)\n");
	Node* p;
	time quick;  //最早进程到达时间
	quick.minute=0;
	quick.hour=0;
	int id,good,zx;
	
	time arrive;
	for(int i=0;i<n;i++) {
		p=(Node*)malloc(sizeof(Node));
		scanf("%d\t%s\t%d\t%d:%d\t%d",&id,p->name,&good,&arrive.hour,&arrive.minute,&p->zx);
		
		p->next=NULL;
		p->id=id;
		
		p->good=good;
		p->arrive.hour=arrive.hour;
		p->arrive.minute=arrive.minute;
		if(wait->front==NULL){
			wait->front=p;
			wait->tail=p;
			quick=arrive;
		}else{
			wait->tail->next=p;
			wait->tail=p;
			if(timecompare2(quick,arrive)){
				quick=arrive;
			}
		}
	}
	return quick;		
}

int main(){
	
	Queue* wait;
	wait = init();
	int flag,n;
	time earlytime;
	while(1){
		printf("\n请输入操作:(1:开始进程;0:结束进程):");
		scanf("%d",&flag);
		if(flag == 0){
			printf("\n操作结束!\n");
			break; 
		} 
		else{
			printf("请输入进程数量:");
			scanf("%d",&n);
			earlytime = ScanIn(wait,n);
			// LevelSorted(wait,n);
			// wait = priority(wait,earlytime,n);
			Print(wait,n);
			wait = init();
			
		}
	}
	return 0;
}
