#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>


typedef struct node
{
    int size;//需要分配空间的大小
    int a[1000];//块号
    int zihao[1000];//字号
    int weihao[1000];//位号
    int zhu[1000];//柱面号
    int citou[1000];//磁头号
    int shanqu[1000];//扇区号
    char name[10];//作业名
    struct node* next;
}linklist;


linklist* work = NULL;


int disksize;//硬盘大小
int word;//字长
int blocksize;//物理块大小
int head;//磁头数
int sector;//扇区数
int cylinder;//每个柱面上的扇区数
int sum;//总扇区数
int available; //辅存剩余空闲块数
int map[4096][4096];  //主存位视图
int line;  //位式图行数

int currentZhu;   //当前柱面号
int currentCitou; //当前磁头号
int currentShanqu;//当前扇区号


//函数名：print    参数：无
void print()
{
    //函数功能：输出辅存位示图
    available = 0;
    printf("          ********************辅存使用情况********************\n");
    printf("   ");
    for (int i = 0; i < word; i++) {
        printf("%-2d ", i);
    }
    printf("\n");

    for (int i = 0; i < line; i++) {
        printf("%-2d ", i);
        for (int j = 0; j < word; j++) {
            if (i * word + j >= disksize) {
                break;
            }
            if (map[i][j] == 0) {
                available++;
            }
            printf("%-2d ", map[i][j]);
        }
        printf("\n");
    }
    printf("辅存剩余空闲块数：%d\n", available);

}
//函数名：allocate  参数：无
void allocate()
{
    //函数功能：为作业申请空间， 分别有、序号、块、字、位、柱面号、磁	头号、扇区号，并输出
    linklist* node = (linklist*)malloc(sizeof(linklist));
    node->next = NULL;
    printf("请输入需要申请空间的作业名字和需要分配辅存空间的大小（单位：K）：");
    scanf("%s",node->name);
    scanf("%d",&node->size);
    if (node->size > available) {
        printf("辅存申请失败!\n");
        return;
    }
    int num = 0;
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < word; j++) {
            if (i * word + j >= disksize) {
                break;
            }
            if (map[i][j] == 0 && num < node->size) {
                int flag = i * word + j;
                node->a[num] = flag;
                node->zihao[num] = i;
                node->weihao[num] = j;

                node->zhu[num] = currentZhu;
                if (flag / sector != (flag - 1) / sector) {
                    currentCitou++;
                }
                node->citou[num] = currentCitou;
                node->shanqu[num] = flag % sector;
                num++;
                map[i][j] = 1;
            }
        }
    }
    if (work == NULL) {
        work = node;
    }
    else {
        linklist* p = work;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = node;
    }
    


    printf("辅存申请成功!\n");
    print();
    printf("辅存剩余空闲块数:%d\n",available);
    printf("          ********************%s被分配的辅存********************\n",node->name);
    printf("序号\t块\t字\t位\t柱面号\t磁头号\t扇区号\n");
    for (int i = 0; i < node->size; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",i+1,node->a[i],node->zihao[i],node->weihao[i],node->zhu[i],node->citou[i],node->shanqu[i]);
    }

}
//函数名：recycle参数:无
void recycle()
{
    //函数功能：回收作业的存储空间，并输出位示图
    linklist* p = work;
    if (p == NULL) {
        printf("当前未分配作业!\n");
        return;
    }
    printf("当前分配的作业：");
    while (p != NULL) {
        printf("%s",p->name);
        if (p->next != NULL) {
            printf("->");
        }
        p = p->next;
    }
    printf("\n要回收的作业名：");
    char name[20];
    scanf("%s",name);
    p = work;
    while (p!=NULL && strcmp(p->name,name)!=0) {
        p = p->next;
    }
    if (p == NULL) {
        printf("回收失败!\n");
        return;
    }
    for (int i = 0; i < p->size; i++) {
        map[p->zihao[i]][p->weihao[i]] = 0;
    }
    if (p == work) {
        work = work->next;
    }
    else {
        linklist* pre = work;
        while (pre->next!=p) {
            pre = pre->next;
        }
        pre->next = p->next;
    }

    printf("回收成功!\n");
    print();


}
//函数名：init 参数:无
void init()
{
    //函数功能：随机初始位示图，并输出位示图
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < word; j++) {
            if (i * word + j >= disksize) {
                break;
            }
            map[i][j] = rand() % 2;
        }
    }
    print();



}
//函数名：menu  参数:无
int menu()
{
    //函数功能：输出菜单
    printf("***************辅存调度管理***************\n");
    printf("       *       1.空间分配         *\n");
    printf("       *       2.空间去配         *\n");
    printf("       *       0.退出             *\n");
    printf("               请输入选项[ ]\b\b");
    int choice;
    scanf("%d",&choice);
    if (choice == 1) {
        return 1;
    }
    else if (choice == 2) {
        return 2;
    }
    else if (choice == 0) {
        return 0;
    }

}

int main()
{
    printf("请输入辅存空间的大小（单位：K）和字长（32 or 64）和块长（单位：K）：");
    scanf("%d %d %d", &disksize, &word, &blocksize);
    printf("请输入该辅存硬盘的磁道数（磁头数）和每个磁道的扇区数：");
    scanf("%d %d", &head, &sector);
    sum = disksize / blocksize;  //总扇区数
    line = sum / word + (sum % word ? 1 : 0);   //行数
    cylinder = head * sector;  //每个柱面上的扇区数

    init();

    while (1)
    {
        int c;
        c = menu();
        if (c == 0)
        {
            break;
        }
        else if (c == 1) {
            allocate();
        }
        else if (c == 2) {
            recycle();
        }
    }
    return 0;
}




