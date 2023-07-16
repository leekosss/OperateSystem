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
#include<malloc.h>
#include<string.h>

#define MAX 1000

int sc[MAX][MAX] = { 0 };//系统内存
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
    struct link_node* next;
} node;
node* h = NULL;


//函数名：OperationChoice       函数参数：无
int OperationChoice()   //根据所输入的选项选择相关操作
{
    //函数功能：打印操作菜单，选择需要进行的操作
    int choice=0;
    printf("*********分页式管理**********\n");
    printf("   *     1.内存分配      *\n");
    printf("   *     2.内存去配      *\n");
    printf("   *     0.退出          *\n");
    printf("请输入选项:");
    scanf("%d",&choice);
    return choice;
}
//函数名：print       函数参数：无
void print()
{
    //函数功能：打印系统内存信息
    int count = 0,num=0;
    printf("    ");
    for (int y = 0; y < WordLength; y++) {
        printf("%2d ", y);
    }
    printf("\n");
    for (int x = 0; x < WordNum; x++) {
        printf("%2d |", x);
        for (int y = 0; y < WordLength; y++) {
            if (sc[x][y] == 1) {
                count++;    //统计内存块中值为1的数量
            }
            if (num < MemorySize) {
                printf("%2d ", sc[x][y]);   //判断需要输出多少个块
            }
            num += BlockSize;
        }
        printf("\n");
    }
    UsingBolckNum = count;    //给已使用的物理块赋值
    AvailableBlockNum = BlockNum - UsingBolckNum;    //给可用的物理块赋值
    printf("剩余物理块数:%d\n",AvailableBlockNum);

}
//函数名：allocate    函数参数：head（作业链表的第一个节点）
node* allocate(node* head)
{
    //函数功能：建立作业节点，判断是否可以分配，可以进行下一步，进行内存分配，
    //插入作业链表，打印该作业的页表信息，打印系统内存信息
    printf("主存位视图如图所示：\n");
    print();   

    node* work;
    work = (node*)malloc(sizeof(node));   //初始化一个结点
    work->next = NULL;
    int workSize;
    printf("请输入作业的名字:");
    scanf("%s",work->name);
    printf("请输入作业需要分配的大小:");
    scanf("%d",&workSize);   //给节点赋初值
    workSize = workSize / BlockSize;   
    if (workSize > AvailableBlockNum) {   //判断所需分配内存是否够
        printf("分配失败!\n");
        return head;
    }
    int count = 0;
    for (int x = 0; x < WordNum;x++) {
        for (int y = 0; y < WordLength; y++) {
            if (count < workSize && sc[x][y] == 0) {
                work->size[count][0] = (x * WordLength) + y; //给页表赋值
                count++;
                sc[x][y] = 1;  //给内存赋值
            }
        }
    }
    printf("分配成功!\n");
    printf("主存位视图如图所示：\n");
    print();
    printf("*********打印%s作业的页表*********\n",work->name);

    for (int i = 0; i < workSize;i++) {
        printf("\t%d\t%d\n",i,work->size[i][0]);   //打印页表
    }

    // 将结点插入作业链表中
    if (head == NULL) {
        return work;
    }
    node* p = head;
    while (p->next!=NULL) {
        p = p->next;
    }
    p->next = work;
    return head;
}
//函数名：recycle        函数参数：head（作业链表的第一个节点）
node* recycle(node* head)
{
    //函数功能：找到待回收的作业节点，释放内存，从作业链表中删除，打印系统内存信息
    char n[10];
    printf("请输入要回收的作业名:\n");
    scanf("%s",n);
    node* p = head;
    node* pre = NULL;
    int flag = -1;
    while (p!=NULL) {   
        if (strcmp(n, p->name) == 0) {   //根据输入的作业名匹配作业链表
            flag = 1;
            break;
        }
        pre = p;
        p = p->next;
    }
    if (flag == -1) {
        printf("无此作业，回收失败!\n");   //如果没有找到就回收失败
        return head;
    }
    int count = 0;
    for (int x = 0; x < WordNum;x++) {  //遍历内存块
        for (int y = 0; y < WordLength;y++) {
            if (p->size[count][0]!=0 && p->size[count][0] == (x * WordLength + y)) {
                // 找到对应的页表
                sc[x][y] = 0;   //将内存中对应内存块赋值为0
                AvailableBlockNum += 1;  //修改可使用的块数
                UsingBolckNum -= 1;
                count++;
            }
        }
    }
    printf("回收成功!\n");
    printf("主存位视图如图所示：\n");
    print();
    //将结点从链表中去掉
    if (p == head) {
        return head->next;
    }
    pre->next = p->next;
    return head;


}
//函数名：initMemory       函数参数：无
void initMemory() {
    //函数功能：定义内存大小，字长与物理块大小，初始化系统内存，打印系统内存信息
    printf("请输入系统内存空间大小:");
    scanf("%d", &MemorySize);   //初始化系统内存空间
    printf("请输入字长(16/32/64):");
    scanf("%d", &WordLength);   //初始化字长

    printf("请输入物理块大小:");
    scanf("%d", &BlockSize);   //初始化物理块大小
    if (MemorySize % (WordLength * BlockSize) == 0) {
        WordNum = MemorySize / (WordLength * BlockSize); //计算页数
    }
    else {
        WordNum = MemorySize / (WordLength * BlockSize) + 1;
    }
    BlockNum = MemorySize / BlockSize;  //计算块数
    int count = 0;
    for (int x = 0; x < WordNum; x++) {
        for (int y = 0; y < WordLength; y++) {
            if (count < MemorySize) {
                sc[x][y] = rand() % 2;   //初始化主存位视图
            }
            count++;
        }
    }
    print();   //打印系统内存信息


}
//函数名：Apply          函数参数：无
void Apply()
{
    initMemory();   //初始化内存
    while (1)
    {
        int c;
        c = OperationChoice();
        switch (c)
        {
        case 1:
            h=allocate(h);   //分配
            break;
        case 2:
            h=recycle(h);    //回收
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


