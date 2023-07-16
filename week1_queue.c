/*队列顺序存储用链表来表示，用front指针表示队首，
用rear指针表示队尾，实现队列的几个基本的操作。*/ 

#include<stdio.h>
#include<malloc.h>
#define MAXSIZE 10
typedef int datatype;
typedef struct
{
	datatype a[MAXSIZE];
	int front;
	int rear;
	int size;
}sequence_queue;

//函数名：init   参数：sequence_queue * sq
void init(sequence_queue* sq)
{
	//功能: 队列初始化 
    sq->rear=sq->front=0;
    sq->size=0;
} 

//函数名：empty   参数：sequence_queue * sq
int empty(sequence_queue * sq)
{
	//功能: 判断队列是否为空
    if(sq->size==0) {
        return 1;
    }else {
        return 0;
    }

} 

//函数名：full   参数：sequence_queue * sq
int full(sequence_queue* sq)
{
	//功能: 判断队列是否为满
    if(sq->rear == MAXSIZE - 1 || sq->size == MAXSIZE) {
		return 1;
    } else {
		return 0;
	}

} 

//函数名：display   参数：sequence_queue * sq
void display(sequence_queue * sq)
{
	//功能: 打印队列
	for(int i=sq->front;i < sq->rear;i++) {
		printf("%d ",sq->a[i]);
	}
	printf("\n");

}

//函数名：dequeue   参数：sequence_queue * sq
datatype dequeue(sequence_queue * sq)
{
	//功能: 出队
	if(sq->front==sq->rear) {
		return -1;
	}
	sq->size--;
	return sq->a[sq->front++];
}

//函数名：insertqueue   参数：sequence_queue * sq, datatype x
void insertqueue(sequence_queue * sq, datatype x)
{
	//功能: 入队 
	if(sq->rear == MAXSIZE) {
		printf("队列已满！\n");
	}
	sq->a[sq->rear++] = x;

}

//函数名：clearqueue  参数：sequence_queue * sq
void clearqueue(sequence_queue * sq)
{
	//功能: 清空队列 
	sq->front = sq->rear=0;
	sq->size=0;
} 

int main()
{
	datatype x;
	sequence_queue* sq = (sequence_queue*)malloc(sizeof(sequence_queue));
	init(sq);
	int n;
	printf("请输入需要创建队列的个数:");
	scanf("%d", &n);
	for(int i = 1; i <= n; ++ i)
	{
		printf("请输入需要插入的值:");
		scanf("%d", &x);
		insertqueue(sq, x);
	} 
	int flag;
	do
	{
		printf("是否进行出队操作(1: 是, 0: 否):");
		scanf("%d", &flag);
		if(flag)
		{
			x = dequeue(sq);
			printf("出队的元素为: %d\n", x); 
		}
	    display(sq);
	}while(flag);
	do
	{
		printf("是否进行插入操作(1: 是, 0: 否):");
		scanf("%d", &flag);
		if(flag)
		{
			printf("请输入需要插入的值:"); 
		    scanf("%d", &x);
		    insertqueue(sq, x);
		}
		display(sq);
	}while(flag);
	
	clearqueue(sq);
	display(sq);
	return 0;
}

