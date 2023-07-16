/*
本实验是模拟进程调度中的时间片轮转算法，每个进程的状态可以是就绪、运行两种状态之一，
就绪队列进程按照FCFS算法思想获得CPU后都只能运行一个时间片长，
如果一个时间片后(最后一次可能小于时间片长)，进程的己占用CPU时间达到所需要的运行时间，则撤消该进程，
如果一个时间片后进程的己占用CPU时间还未达所需要的运行时间，
此时应分配时间片给就绪队列中排在该进程之后的进程，并将它重新插入到就绪队列队尾。
每进行一次调度，程序都打印一次当前正在运行的进程和就绪队列中各个进程的PCB信息，
以便进行检查。重复以上过程，直到所有进程都完成为止，最后再输出所有进程的相关信息。
*/

#include <iostream>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
using namespace std;

typedef struct data{
	int hour;
	int min;
}time;

typedef struct node{
	
	int id;//进程编号 
	char name[20];//进程名 
	time arrive;//到达就绪队列的时间 
	int zx;//执行时间 
	time start;//开始执行时间 
	time finish;//执行完成时间 
	int zz;//周转时间=执行完成时间-到达就绪队列时间 
	float zzxs;//带权周转时间=周转时间/执行时间 
	time current;//当前进程开始执行的时间 
	int ywcsj;//进程已经完成的时间 
	int sysj; //当前进程的剩余时间 
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

//函数名:timecompare		函数参数:，tt（系统计时）,p（当前进程的到达时间）
bool timecompare(time tt,time p){	
	 //函数功能：比较时间大小，tt<p返回false，否则返回true
	if(tt.hour<p.hour) {
		return false;
	}
	if(tt.hour>p.hour) {
		return true;
	}
	if(tt.min<p.min) {
		return false;
	}else{
		return true;
	}
}
//函数名:Namecompare		函数参数:，s1（进程名称）,s2（进程名称）
bool Namecompare(char* s1,char* s2){
	//函数功能：比较名字大小，s1<s2返回true，否则返回false
	if(strcmp(s1,s2)<0) {
		return true;
	}else {
		return false;
	}
}

//函数名:Choose		函数参数:，p（进程指针）,q（进程指针），flag（选择标志）
bool Choose(Node* p, Node* q,int flag){
	//函数功能：选择比较函数比较名字大小，flag==1返回时间比较结果取反，否则返回名字比较结果
	if(flag==1) {
		return !timecompare(p->current,q->arrive);
	}else {
		return Namecompare(p->name,q->name);
	}
}

//函数名:Time_or_Name_Sorted		函数参数:，que（队列指针），flag（选择标志）
void Time_or_Name_Sorted(Queue* que,int flag){
	//函数功能：选择排序函数并对进程进行排序，flag==1返回时间排序，否则返回名字排序 
	
}

//函数名:ComputeTime		函数参数:，tt（时间指针），q（结点指针），tp（时间片）
// time ComputeTime(time* tt,Node* q,int tp){
// 	//函数功能：
// //进程还未开始，首次更新进程的开始时间，并且每次重新定义进程执行的当前时间 
// 	//根据剩余时间与时间片的关系进行讨论
// 	//更新进程的完成时间和周转时间,带权周转时间 ，最后返回时间指针
// //调用timecompare()函数来寻找已到达的进程
// //调用ComputeTime()函数来计算相关时间并更新进程信息	
				
// }

//函数名:Print_Current		函数参数:，que（队列指针），tt（系统时间），i（时间轮转次数）
void Print_Current(Queue* que,time tt,int i){
	//函数功能：输出第i次时间轮转的进程信息，置前进程开始执行的时间为0
	
}

//函数名:Time_Slice_Rotation	函数参数:，que（队列指针），tp（时间片）
Queue* Time_Slice_Rotation(Queue *que,int tp){
	//函数功能：模拟时间片轮转算法，分三种情况讨论：
	//当前进程执行完
	//当前进程未执行完
	//除当前进程以外就绪队列队列为空，当前进程继续执行
	//另外还要考虑就绪队列为空，诞下一个进程还未达到的情况。 
	
	
} 

//函数名:Print 	函数参数:，que（队列指针），n（进程数）
void Print(Queue* cc,int n){
	//函数功能：输出所有进程完成后的信息以及系统平均周转时间和系统平均带权周转系数
	Node* p;
    p=cc->front;
    printf("\n模拟短进程优先调度过程输出结果:\n");
    printf("id号\t名字\t到达\t执行\t开始\t完成\t周转\t带权周转系数\n");
    int num=0;
    double zzxs=0,zz=0;
    while(p!=NULL) {
        num++;
        zz+=p->zz;
        zzxs+=p->zzxs;
        printf("%d\t%s\t%02d:%02d\t%d\t%02d:%02d\t%02d:%02d\t%d\t%.2f\n",p->id,p->name,p->arrive.hour,p->arrive.min,p->zx,p->start.hour,p->start.min,p->finish.hour,p->finish.min,p->zz,p->zzxs);
        p=p->next;
    }
    printf("系统平均周转时间为:\t\t\t\t%.2f\n",zz/num);
    printf("系统平均周转系数为:\t\t\t\t\t%.2f\n",zzxs/num);
}

//函数名：insert  参数：Queue *cc, node *x
void insert(Queue* cc,Node* s){
    //函数功能:尾插入队
    Node* p,pre;
    pre=NULL;
    p=cc->front;
    s->next=NULL;
    if(p==NULL) {
        cc->front=cc->rear=s;
        return;
    }
    while(p!=NULL&&!timeCompare(p->arrive,s->arrive)) {
        pre=p;
        p=p->next;
    }
    if(p==NULL) {
        cc->tail->next=s;
        cc->tail=s;
    }else{
        if(p==cc->front) {
        	s->next=p;
        	cc->front=s;
        	return;
		} 
		s->next=p;
        pre->next=s;
    }

}

//函数名:ScanIn	函数参数:，wait（队列指针），n（进程数）
void ScanIn(Queue* wait,int n){
	//函数功能：输入相关进程及信息
	node* q;
	printf("请输入进程的参数:\n");
	printf("id号\t名字\t到达\t执行:\n");
	for(int i=0;i<n;i++) {
		q=(Node*)malloc(sizeof(Node));
		q->next=NULL;
		scanf("%d\t%s\t%d:%d\t%d",&q->id,q->name,&q->arrive.hour,&q->arrive.min,&q->zx);
		insert(wait,q);
	}	
}

int main(){
	
	Queue* wait;
	wait = init();
	int flag,n,t;
	time earlytime;
	
	while(1){
		printf("请输入操作:(1:开始进程;0:结束进程):");
		scanf("%d",&flag);
		if(flag == 0){
			printf("\n操作结束!\n");
			break; 
		} 
		else{
			printf("请输入进程数量:\n");
			scanf("%d",&n);
			printf("请输入时间片时间:\n");
			scanf("%d",&t);
			ScanIn(wait,n);
			
			// Time_or_Name_Sorted(wait,1);//timesort 
			// wait = Time_Slice_Rotation(wait,t);
			// Time_or_Name_Sorted(wait,0);//namesort 
			Print(wait,n);
			
			wait = init();
			
		}
	}
	
	return 0;

}

/*
测试用例：
5个进程：
5001  p1  9:40    20
5004  p4  10:10   10
5005  p5  10:05   30
5002  p2  9:55    15
5003  p3  9:45    25

10个进程：
5001  p1  1  19:40  20
5002  p4  2  10:10  10
5003  p5  3  10:05  30
5004  p2  4  9:55   15
5005  p3  5  9:45   25
5006  p6  6  11:40  20
5007  p8  7  12:10  10
5008  p9  8  13:05  30
5009  p10 9  19:55  15
5010  p7  10 7:15   15
*/