/*
week9_离散存储管理方式的模拟与实现_分页式

本实验是模拟存储管理方式中的分页式分配与回收算法，系统需要建立两张表，
分别是主存位示图表(可用随机函数模拟实现0/1初始值)和作业业表，
然后根据待装入作业的名称和页数到位示图表中查找满足要求的空闲物理块，
然后将位示图表对应位置的0置成1并修改剩余空闲块数，新建一张作业页表，
将作业逻辑页号及分配成功的物理块号依次填入此表，最后通过动态重定位方式完成地址转换；
当作业运行结束时，根据该作业名查找作业页表，找到该作业占用的物理块号，
最后修改位示图表相应上的块号标志清零和剩余空闲块数。
*/
#include<stdio.h>
#include<stdlib.h>
#define MAX 100

int sc[16][64]= {0};//系统内存
int MemorySize;//系统内存大小
int WordLength;//字长
int WordNum;//页数
int BlockSize;//物理块大小
int BlockNum;//物理块的数量
int UsingBolckNum;//使用的物理块
int AvailableBlockNum;//可使用的物理块
//作业节点
typedef struct link_node
{
    char name[10];//作业名
    int size[MAX][MAX];//作业页表
    struct link_node *next;
} node;



//函数名：OperationChoice       函数参数：无
int OperationChoice()
{
//函数功能：打印操作菜单，选择需要进行的操作
}
//函数名：print       函数参数：无
void print()
{
//函数功能：打印系统内存信息
    printf("    ");
    for(int y=0;y<64;y++) {
        printf("%2d ",y);
    }
    printf("\n");
    for(int x=0;x<16;x++) {
        printf("%2d |",x);
        for(int y=0;y<64;y++) {
            printf("%2d ",sc[x][y]);
        }
        printf("\n");
    }

}
//函数名：allocate    函数参数：head（作业链表的第一个节点）
node* allocate(node *head)
{
//函数功能：建立作业节点，判断是否可以分配，可以进行下一步，进行内存分配，插	入作业链表，打印该作业的页表信息，打印系统内存信息
}
//函数名：recycle        函数参数：head（作业链表的第一个节点）
node* recycle(node *head)
{
//函数功能：找到待回收的作业节点，释放内存，从作业链表中删除，打印系统内存信息
}
//函数名：initMemory       函数参数：无
void initMemory(){
//函数功能：定义内存大小，字长与物理块大小，初始化系统内存，打印系统内存信息
    printf("请输入系统内存空间大小:");
    scanf("%d",&MemorySize);
    printf("请输入字长(16/32/64):");
    scanf("%d",&WordLength);
    printf("请输入物理块大小:");
    scanf("%d",&BlockSize);
    for(int x=0;x<16;x++) {
        for(int y=0;y<64;y++) {
            sc[x][y]=rand()%2;
        }
    }
    print();


}
//函数名：Apply          函数参数：无
void Apply()
{
    initMemory();
    while(1)
    {
        int c;
        c=OperationChoice();
        switch(c)
        {
        case 1:
            // head=allocate(head);
            break;
        case 2:
            // head=recycle(head);
            break;
        case 0:
            return;
        }
    }
}

int main()
{
    Apply();
    return 0;
}





/*
测试用例：

1000
64
1
1
job2
50
1
job1
30
2
job2
2
1
2
job1
0

*/


