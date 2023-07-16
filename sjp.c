#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <malloc.h>
#include <string.h>
typedef struct data{
    int hour;
    int minute;
} time;

typedef struct node{
    int id;
    char name[20];
    time arrive;
    int zx;
    time start;
    time finish;
    int zz;
    float zzxs;
    time current;
    int ywcsj;
    int sysj;
    struct node* next;
} Node;

typedef struct Queue{
    Node* front;
    Node* tail;
} Queue;
Queue* init(){
	
	Queue* p = (Queue*)malloc(sizeof(Queue));
	p->front = NULL;
	p->tail = NULL;
	return p;
	
} 
bool timecompare(time tt, time p){
    if(tt.hour < p.hour){
        return false;
    }
    else if(tt.hour == p.hour && tt.minute < p.minute){
        return false;
    }
    else{
        return true;
    }
}
//函数名:Namecompare		函数参数:，s1（进程名称）,s2（进程名称）
bool Namecompare(char* s1, char* s2){
    //函数功能：比较名字大小，s1<s2返回true，否则返回false
    if(strcmp(s1, s2) < 0){
        return true;
    }
    else{
        return false;
    }
}
time ComputeTime(time tt, Node* q, int tp){
    if(q->sysj == q->zx){
        q->start = tt;
        q->current = tt;
    }
    if(q->sysj <= tp){
        tt.minute += q->sysj;
        q->finish = tt;
        q->zz = (q->finish.hour - q->arrive.hour) * 60 + q->finish.minute - q->arrive.minute;
        q->zzxs = (float)q->zz / q->zx;
        q->sysj = 0;
    }
    else{
        tt.minute += tp;
        q->current = tt;
        q->ywcsj += tp;
        q->sysj -= tp;
    }
    return tt;
}
void Print_Process(Node* p, time tt, int count) {
    printf("第%d次时间轮转：\n", count);
    printf("进程ID\t进程名\t到达时间\t执行时间\t开始时间\t完成时间\n");
    printf("%d\t%s\t%02d:%02d\t\t%d\t\t%02d:%02d\t\t%02d:%02d\n", p->id,p->name, p->arrive.hour, p->arrive.minute, p->zx, p->start.hour, p->start.minute, p->finish.hour, p->finish.minute);    
}
Queue* Time_Slice_Rotation(Queue *que, int tp) {
    //函数功能：模拟时间片轮转算法，分三种情况讨论：
	//当前进程执行完
	//当前进程未执行完
	//除当前进程以外就绪队列队列为空，当前进程继续执行
	//另外还要考虑就绪队列为空，诞下一个进程还未达到的情况。 
    Node* p = que->front;
    if (p == NULL) {
        return NULL;
    }
    time tt = p->arrive;
    int count = 1;
    Node* current = p;
    while (p != NULL) {
        if (p->sysj == 0) {
            p = p->next;
            continue;
        }
        if (p->sysj <= tp) {
            tt = ComputeTime(tt, p, tp);
            current = p->next;
            que->front = current;
            que->tail->next = p;
            que->tail = p;
            p->next = NULL;
        } else {
            tt = ComputeTime(tt, p, tp);
            current = p;
            que->front = p->next;
            que->tail->next = p;
            que->tail = p;
            p->next = NULL;
        }
        Print_Process(current, tt, count);
        count++;
        p = que->front;
    }
    return que;
}
void Print(Queue* que, int n){
    Node* p = que->front;
    int sum_zz = 0;
    float sum_zzxs = 0;
    printf("模拟进程时间片轮转进程调度过程输出结果：\n");
    printf("进程ID\t进程名\t到达时间\t执行时间\t开始时间\t完成时间\t周转时间\t带权周转时间\n");
    while(p != NULL){
        printf("%d\t%s\t%02d:%02d\t\t%d\t\t%02d:%02d\t\t%02d:%02d\t\t%d\t\t%.2f\n", p->id,p->name, p->arrive.hour, p->arrive.minute, p->zx, p->start.hour, p->start.minute, p->finish.hour, p->finish.minute, p->zz, p->zzxs);
        sum_zz += p->zz;
        sum_zzxs += p->zzxs;
        p = p->next;
    }
    printf("平均周转时间：%.2f\n", (float)sum_zz / n);
    printf("平均带权周转时间：%.2f\n", sum_zzxs / n);
}
//函数名：insert  参数：Queue *cc, node *x
void insert(Queue* cc,Node* s){
    //函数功能:尾插入队
    Node* p;
    p=cc->front;
    if(p==NULL) {
        cc->front=cc->tail=s;
        return;
    }
    cc->tail->next=s;
    cc->tail=s;
}
//函数名：input   参数：Queue *cc
void ScanIn(Queue* cc,int n){
    //功能：实现作业队列的输入
    printf("请输入进程参数：\n");
	printf("id号、名字、到达时间（时 分）、执行时间：\n");
    for(int i = 0; i < n; i++){
        Node* p = (Node*)malloc(sizeof(Node)); //为新结点分配内存空间
        scanf("%d %s %d:%d %d", &p->id, p->name, &p->arrive.hour, &p->arrive.minute, &p->zx);
        p->sysj=p->zx;
        p->next = NULL;
        if(cc->front == NULL){ //队列为空
            cc->front = cc->tail= p; //新结点为队首和队尾
        }
        else{
            insert(cc,p);
        }
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
		    printf("请输入作业个数：");
		    scanf("%d", &n);
            printf("请输入时间片时间:\n");
            scanf("%d",&t);
            ScanIn(wait,n);
			// Time_or_Name_Sorted(wait,1);//timesort 
			wait = Time_Slice_Rotation(wait,t);
			//Time_or_Name_Sorted(wait,0);//namesort
            //wait = init();
            Print(wait,n);
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
