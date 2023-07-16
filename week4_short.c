/*
短进程优先 (SPF)调度算法则是从就绪队列中选出一个估计运行时间最短的进程，
将处理机分配给它，使它立即执行并一直执行到完成
*/

#include<stdio.h>
#include<malloc.h>
typedef struct data{
    int hour;
    int min;
}time;
typedef struct node{
    int id;//编号
    char name[20];//名字
    time arrive;//到达时间
    int zx;//执行时间；
    time start;//开始时间
    time finish;//完成时间
    int zz;//周转时间=执行完成时间-到达就绪队列时间
    double zzxs;//带权周转时间=周转时间/执行时间
    struct node* next;
}linknode;

typedef linknode* linklist;

typedef struct{
    linklist front;
    linklist rear;
}queue;   //队列
//函数名：init   参数：无
queue* init(){
//函数功能：初始化队列，返回队列指针
    queue* q = (queue*)malloc(sizeof(queue));
    q->front = NULL;
    q->rear = NULL;
    return q;
}
int timeCompare(time t1,time t2) {
    if(t1.hour<t2.hour) {
        return 1;
    }
    if(t1.hour>t2.hour) {
        return 0;
    }
    if(t1.min<=t2.min) {
        return 1;
    }else{
        return 0;
    }
}
int zxCompare(int t1,int t2) {
	if(t1<=t2) {
		return 1;
	}else{
		return 0;
	}
} 
//函数名：insert  参数：Queue *cc, node *x
void insert(queue* cc,linklist s){
    //函数功能:尾插入队
    linklist p,pre;
    pre=NULL;
    p=cc->front;
    s->next=NULL;
    if(p==NULL) {
        cc->front=cc->rear=s;
        return;
    }
    // p=cc->rear;
    // p->next=s;
    while(p!=NULL&&timeCompare(p->arrive,s->arrive)) {
        pre=p;
        p=p->next;
    }
    if(p==NULL) {
        cc->rear->next=s;
        cc->rear=s;
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
//函数名：dele  参数：Queue *cc
void dele(queue* cc){ 
//功能：队首结点出队
    linklist p;
    p=cc->front;
    if(p==NULL) {
        return;
    }
    cc->front=cc->front->next;
}
//函数名：input   参数：Queue *cc
void input(queue* cc){
//功能：实现作业队列的输入
    printf("请输入进程数:\n");
    int num;
    scanf("%d",&num);
    printf("id号\t名字\t到达\t执行\n");

    for(int i=0;i<num;i++) {
        linklist p = (linklist)malloc(sizeof(linknode));
        scanf("%d\t%s\t%d:%d\t%d",&p->id,p->name,&p->arrive.hour,&p->arrive.min,&p->zx);
        insert(cc,p);
    }
}


void output(queue* cc) {
    linklist p;
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

time computer(time arrive,int zx) {
    int total=arrive.hour*60+arrive.min+zx;
    time t;
    t.hour=total/60;
    t.min=total%60;
    return t;
}

void computerArgs(queue* cc) {  //计算参数
    linklist p=cc->front,pre=NULL;

    while(p!=NULL) {
        if(p==cc->front) {
            p->start=p->arrive;
        }else{ //不是第一个
            if(timeCompare(pre->finish,p->arrive)) {
                p->start=p->arrive;
            }else{
                p->start=pre->finish;
            }
        }
        p->zz=p->finish.hour*60+p->finish.min-p->arrive.hour*60-p->arrive.min;
        p->zzxs=(double)p->zz/(double)p->zx;
        pre=p;
        p=p->next;
    }
}

//函数名：solve_SJF参数：Queue *cc
void solve_SJF(queue* cc){	
//函数功能：解决短进程优先调度算法 
    linklist p=cc->front;
    p->finish=computer(p->arrive,p->zx);
    while(p!=NULL) {
        linklist tmp=p->next;
        linklist pos=NULL;
        while(tmp!=NULL&&timeCompare(tmp->arrive,p->finish)) { 
            pos=tmp; // 找到执行进程时，到达的最后一个队列
            tmp=tmp->next;
        }
        linklist qre,q,tmr;
        qre=p;
        q=p->next;
        tmr=p;
        tmp=p->next;

        while(pos!=NULL&&q!=pos) {
                if(!zxCompare(q->zx,q->next->zx)) {
                	tmr=q;
                    tmp=q->next;
                	if(q->next==pos) {
                		break; 
					} 
                    
                }else{
                    qre=q;
                    q=q->next;
                }
        }
        if(tmp!=NULL) {
        	tmr->next=tmp->next;
        	tmp->next=p->next;
        	p->next=tmp;
		}
   
        tmp=p;
        p=p->next;
        if(p==NULL) {
        	continue;
		}
        if(timeCompare(p->arrive,tmp->finish)) {
            p->finish=computer(tmp->finish,p->zx);
        }else {
            p->finish=computer(p->arrive,p->zx);
        }
    }
    computerArgs(cc);  //计算结果
    output(cc);        //输出
}



int main()
{
    while(1)
    {
        int op;
        printf("请输入操作,1:开始进程调度;0:结束进程:\n");
        scanf("%d",&op);
        if(op==0) break;
        queue *cc;
        cc = init();
        input(cc);
        solve_SJF(cc);
    }


    return 0;
}
/*
测试用例：
1001    p1  9:40    20
1004    p4  10:10   10
1005    p5  10:05   30
1002    p2  9:55    15
1003    p3  9:45    25


5001    p1  19:40   20
5002    p4  10:10   10
5003    p5  10:05   30
5004    p2  9:55    15
5005    p3  9:45    25
5006    p6  11:40   20
5007    p8  12:10   10
5008    p9  13:05   30
5009    p1  19:55   15
5010    p7  7:15    15
*/
