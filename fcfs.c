#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#define newnode (node *)malloc(sizeof(node));
typedef struct  data{
    int hour;
    int min;
}time;//到达时间

typedef struct{//结点
    //基本
    int id;//编号
    char name[20];//进程名
    time arrive;//到达就绪队列时间
    int zx;//执行时间：预估的执行时间（min)
    //计算
    time start;//开始时间:进程开始进入cpu的时间，“走走停停”
    time finish;//完成时间：连续运行直到结束
    int zz;//周转时间=完成时间-到达时间
    float zzxs;//带权周转系数=周转时间/执行时间,下同
    struct node* next;//队列的指针域
}node;

//队列
typedef struct {
    node *front;//出队进入cpu
    node *rear;//出队进入就绪队列
}queue;
//函数功能：初始化，为之后操作做准备
queue *init(){
    queue *q;
    q=(queue *)malloc(sizeof(queue));
    q->front=NULL;
    q->rear=NULL;
    return q;
}
//函数功能：比较到达时间的早晚
bool check(time x, time y) { //函数参数：time x（当前进程的到达时间）,time y（待插入进程的到达时间）
    if((x.hour)*60+(x.min)>=(y.hour)*60+(y.min))return true;
    return false;
}
//函数功能：构建并按到达时间先后插入进程节点放入就绪队列中
queue *insert(queue *q,node *p){//新插入进程，记得输入x的基本信息（id，name...)，从队尾插入
    //算法：按照进程到达就绪队列的先后次序依次插入到就绪队列中去
    //x到达就绪队列的时间，与队列中的每一个进程的时间进行比较，插入到合适的位置当中去（前后插入都行）
    if(q->front==NULL) //如果队列为空，直接插入p
        q->front=q->rear=p;
    else{
        node *i=q->front;//队首指针
        node *pre=NULL;
        while(i!=NULL){//寻找插入位置
            if(check(i->arrive,p->arrive)==true) break;
                pre=i;
                i=i->next;
		}
        if(pre==NULL){//插在队头
                p->next=i;
                q->front=p;
        }
        else{ //队中
                p->next=i;
                pre->next=p;
        }
    }
    return q;
}
//函数功能：输入节点信息，构建并按到达时间先后依次插入进程节点放入就绪队列中
queue *input(queue *q){//先输入一个队列，结点依次插入
    int n,x;
    printf("请输入进程数：\n"); scanf("%d",&n);
    printf("请输入进程的参数：\n");
    printf("ID号   名字  到达时间  执行时间(分钟)\n");
    while(n>0){
            node *p;
            p=(node *)malloc(sizeof(node));
            scanf("%d %s %d:%d %d",&p->id,&p->name,&p->arrive.hour,&p->arrive.min,&p->zx);
            p->next=NULL;
            p=insert(q,p);
            n--;
            }
    return q;
}

queue *dele(queue *q){//删除（出队）ok
    //从队首删除1front后移
    node *p;
    if(q->front==NULL) printf("队列为空，无法删除！");
    else{
        p=q->front;
        q->front=p->next;
        free(p);
        if(q->front==NULL)
            q->rear=NULL;
    }
    return q;
}
//函数名：cal，函数参数：Time x（当前进程的开始时间）, int y（当前进程的执行时间）
time cal(time x, int y) {
    //函数功能：计算当前进程的结束时间（开始时间+执行时间）
    time t;
    t.hour=x.hour+(x.min+y)/60;
    t.min=(x.min+y)%60;
    return t;
}
//函数功能：计算进程的等待(周转)时间（即上一进程的结束时间与当前进程的到达时间的时间差）
int deline(time x, time y) {//函数参数：time x（上一进程的结束时间）,time y（当前进程的到达时间）
    return (x.hour*60+x.min)-(y.hour*60+y.min);
}

//函数功能：计算进程相关时间信息，分别确定开始时间，结束时间（开始时间+执行时间）
//周转时间（结束时间-到达时间），周转系数（周转时间/执行时间）
queue fcfs(queue *q){//重点：进程运行(先来先服务)
    //与队列出队过程一样，最先就行执行:队首指针所指的结点出队，指针右移
    node *pre,*p;
    for(pre=NULL,p=q->front; p!=NULL; pre=p,p=p->next){
        //开始时间
        if(p==q->front){//第一个进程的开始时间等于到达时间
            p->start.hour=p->arrive.hour;
            p->start.min=p->arrive.min;
        }
        else
            //如果到达时间晚于上一个进程的结束时间
            if((p->arrive.hour*60+p->arrive.min)>=(pre->finish.hour*60+pre->finish.min)){
                p->start.hour=p->arrive.hour;
                p->start.min =p->arrive.min ;
            }
            else{//如果达到时间早于上一个进程的结束时间
                p->start.hour=pre->finish.hour;
                p->start.min =pre->finish.min ;
            }
        //1. 完成时间=开始时间+执行时间
        p->finish=cal(p->start,p->zx);
        //2. 周转时间=完成时间-到达时间
        p->zz=deline(p->finish,p->arrive);
        //3.周转系数
        p->zzxs=(p->zz)*1.0/(p->zx);
    }
}
void output(queue *q){//函数功能：按进程执行顺序分别输出每个进程全部结果信息和系统平均时间
    float pjzz=0;//平均周转时间
    float dqpjzz=0;//带权平均周转时间
    int count=0;
    node *i;
    printf("\n模拟进程FCFS调度的输出结果：\n");
    printf("ID号   名字    到达时间  执行时间(分钟) 开始时间 完成时间    周转时间(分钟) 带权周转时间(系数)\n");
    for(i=q->front; i!=NULL; i=i->next){
        printf("%d    %s\t%02d:%02d      %d(分钟)\t  ",i->id,i->name,i->arrive.hour,i->arrive.min,i->zx);
        printf("%02d:%02d   %02d:%02d\t\t%d(分钟)\t  %.2f\n",i->start.hour,i->start.min,i->finish.hour,i->finish.min,i->zz,i->zzxs);
        pjzz+=i->zz;
        dqpjzz+=i->zzxs;
        count++;
    }
    printf("系统平均周转时间为：%.2f",pjzz/count);
    printf("\n系统带权平均周转时间为：%.2f\n\n",dqpjzz/count);
}

int main()
{
    int x=0;
    printf("请输入操作(1:开始进程调度；0:结束进程):");scanf("%d",&x);
    while(x==1){
	    queue *q;
	    q=init();
	    q=input(q);
	    fcfs(q);
	    output(q);
	    printf("请输入操作(1:开始进程调度；0:结束进程):");scanf("%d",&x);
    }
    return 0;
}
/*
测试用例
5个进程：
5001  p1  9:40    20
5004  p4  10:10   10
5005  p5  10:05   30
5002  p2  9:55    15
5003  p3  9:45    25

10个进程：
5001  p1    19:40  20
5002  p4    10:10  10
5003  p5    10:05  30
5004  p2    9:55   15
5005  p3    9:45   25
5006  p6    11:40  20
5007  p8    12:10  10
5008  p9    13:05  30
5009  p10   19:55  15
5010  p7    7:15   15

*/
