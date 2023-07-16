#include<iostream>
#include<stdio.h>
#include<string.h>
struct Time {
    int hour, min;
}time;
struct node {
    int id;//编号
    char name[20];//进程名
    Time arrive;//到达就绪队列时间
    int zx;//执行时间
    Time start;//开始时间
    Time finish;//完成时间
    int zz;//周转时间=完成时间-到达时间
    float zzxs;//带权周转系数=周转间/执行时间,下同
    struct node* next;
};

//函数名：check，函数参数：Time x（当前进程的到达时间）,Time y（待插入进程的到达时间）
bool check(Time x, Time y) {
    //函数功能：比较到达时间的早晚
    if(x.hour < y.hour) {
        return true;
    }else if(x.hour > y.hour) {
        return false;
    }
    if(x.min <= y.min) {
        return true;
    }else {
        return false;
    }
}

struct Q{
    node *front, *end;
}SQ;
//函数名：init，函数参数：Q &x（含有就绪队列的队头和队尾节点地址）
void init(Q &x) {
     //函数功能：初始化，为之后操作做准备
    // x = (SQ*)malloc(sizeof(SQ));
    x.front = NULL;
    x.end = NULL;

}
//函数名：insert，函数参数：Q *x（指向含有就绪队列的队头和队尾节点的指针）,node tmp（）
void insert(Q *x, node tmp) {
    //函数功能：构建并按到达时间先后插入进程节点放入就绪队列中
    tmp.next=NULL;
    if(x->front==NULL&&x->end==NULL){
        x->front=x->end=&tmp;
        return;
    }
    node *p=x->front;
    while(p->next!=NULL&&check(p->next->arrive,tmp.arrive)) {
        p=p->next;
    }
    if(p->next!=NULL) {
        tmp.next=p->next;
        p->next=&tmp;
    }else{
        bool flag;
        flag=check(p->arrive,tmp.arrive);
        if(flag){
            p->next=&tmp;
            tmp.next=NULL;
            x->end=&tmp;
        }else{
            tmp.next=p;
            p->next=NULL;
            x->end=p;
            x->front=&tmp;
        }
    }

}
//函数名：dele，函数参数：Q &x（含有就绪队列的队头和队尾节点地址）
void dele(Q* x) {
    //函数功能：初始化，为下一个进程序列做准备
    x->front=NULL;
    x->end=NULL;

}
//函数名：input，函数参数：Q &x（含有就绪队列的队头和队尾节点地址）
void input(Q* x) {
    //函数功能：输入节点信息，构建并按到达时间先后依次插入进程节点放入就绪队列中
    int op;
    printf("请输入操作(1:开始进程调度; 0:结束进程):"); 
    scanf("%d",&op);
    if(op!=1){
        exit(0);
    }
    int num;
    printf("请输入进程数量:");
    scanf("%d",&num);
    while(num<=0){
    	printf("请重新输入:");
        scanf("%d",&num);
    }
    printf("id号   名字   到达   执行(分钟):(使用tab进行分隔输入)\n");
    int id;
    char name[20];
    struct Time arrive;
    int h,m;
    int zx;
    for(int i=0;i<num;i++){
        scanf("%d\t%s\t%d:%d\t%d",&id,name,&h,&m,&zx);
        struct node n;
        n.arrive.hour=h;
        n.arrive.min=m;   
        n.id=id;
        for(int i=0;i<20;i++){
            n.name[i]=name[i];
        }
        n.zx=zx;
        insert(x,n);
    }

}
//函数名：cal，函数参数：Time x（当前进程的开始时间）, int y（当前进程的执行时间）
Time cal(Time x, int y) {
    //函数功能：计算当前进程的结束时间（开始时间+执行时间）
    int time=0;
    time = x.hour*60+x.min+y;
    int h=time/60;
    int m=time%60;
    struct Time t;
    t.hour=h;
    t.min=m;
    // printf("%d---%d",t.hour,t.min);
    return t;
}
//函数名：deline，函数参数：Time x（上一进程的结束时间）,Time y（当前进程的到达时间）
int deline(Time x, Time y) {
    //函数功能：计算进程的等待时间（即上一进程的结束时间与当前进程的到达时间的时间差）
    int wait=0;
    wait = y.hour*60+y.min-x.hour*60-x.min;
    if(wait>0) {
        return wait;
    }else{
        return 0;
    }

    
}
//函数名：fcfs，函数参数：Q *x（指向含有就绪队列的队头和队尾节点的指针）
void fcfs(Q *x) {
    //函数功能：计算进程相关时间信息，分别确定开始时间，结束时间（开始时间+执行时间），周转时间（结束时间-到达时间），周转系数（周转时间/执行时间）
    node *p=x->front;
    node *q=p->next;
    int hs,ms,hf,mf;
    struct Time start,finish;
    int zz;
    float zzxs;
    int wait;
    if(p==x->front) {
        
        p->start.hour=p->arrive.hour;
        p->start.min=p->arrive.min;
        printf("------------%d:%d",p->start.hour,p->start.min);


        int time=0;
        time = p->start.hour*60+p->start.min+p->zx;
        int h=time/60;
        int m=time%60;

        p->finish.hour=h;
        p->finish.min=m;
        printf("------------%d:%d",p->finish.hour,p->finish.min);
        p->zz=p->finish.hour*60+p->finish.min-p->arrive.hour*60-p->arrive.min;
        p->zzxs=p->zz/p->zx;
        // p=p->next;
        // q=q->next;
    }
    while(q!=NULL) {
        int time=0,h,m;
        time = p->start.hour*60+p->start.min+p->zx;
        h=time/60;
        m=time%60;
        p->finish.hour=h;
        p->finish.min=m;


        wait=deline(p->finish,q->arrive);
        // start = (Time*)malloc(sizeof(Time));
        q->start.hour=(p->finish.hour*60+p->finish.min+wait)/60;
        q->start.min=(p->finish.hour*60+p->finish.min+wait)%60;

        time = q->start.hour*60+q->start.min+q->zx;
        h=time/60;
        m=time%60;

        q->finish.hour=h;
        q->finish.min=m;
        q->zz=q->finish.hour*60+q->finish.min-q->arrive.hour*60-q->arrive.min;
        q->zzxs=(q->zz*1.0)/q->zx;
        p=p->next;
        q=q->next;
    }

}
//函数名：output，函数参数：Q &x（含有就绪队列的队头和队尾节点的地址）
void output(Q* x) {
    //函数功能：按进程执行顺序分别输出每个进程全部结果信息和系统平均时间
    printf("模拟进程FCFS调度过程输出结果:\n");
    printf("id号    名字    到达时间    执行时间(分钟)  开始时间    完成时间    周转时间(分钟)  带权周转系数\n");
    node *p=x->front;
    int zzsj=0;
    int zzxs=0,count=0;
    
    while(p!=NULL) {
        printf("%d\t%s\t%2d:%2d\t%d(分钟)\t%2d:%2d\t%2d:%2d\t%d(分钟)\t%2f\n",p->id,p->name,p->arrive.hour,p->arrive.min,p->zx,p->start.hour,p->start.min,p->finish.hour,p->finish.min,p->zz,p->zzxs);
        zzsj+=p->zz;
        zzxs+=p->zzxs;
        count+=1;
        p=p->next;
    }

    printf("系统平均周转时间为:%2f\t\t\t\t\n",zzsj/count);
    printf("系统平均带权周转系数为:%2f\t\t\t\t\t\n",zzxs/count);
}

int main() {
	struct Q x;
    while(1) {
    	init(x);
    	input(&x);
   	    fcfs(&x);
    	output(&x);
    	while(x.front != NULL) dele(&x);

	}
    return 0;
}
