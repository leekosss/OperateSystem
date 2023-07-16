#include<stdio.h>
#include<malloc.h>
#include<string.h>


//内存结构体
typedef struct memory_node
{
    int size; //内存大小
    int address; //内存始址
} memoryNode;

memoryNode memory;

//分区结构体
typedef struct link_node
{
    int id;//分区号
    int size; //分区长度
    int address; //分区始址
    char flag[10]; //分区状态，空闲或者占用作业名
    struct link_node* next;
} node;

node* head = NULL;  // 已分配分区表
node* notAllocate = NULL; //未分配分区表

//段表
typedef struct segment_node
{
    char name[10];  //作业的名称
    int a[10][10]; //存储段表信息
    int num; //段表中段的数量
    struct segment_node* next;
} segmentNode;
segmentNode* segment = NULL; //作业段表链表


//函数声明
void firstAllocation(int size, char c[10], int segmentNum);
void reorder();
void print();



//函数名 :initMemory  参数:无
node* initMemory()
{
    //函数功能：初始化内存空间
    printf("请输入内存大小:");
    scanf("%d",&memory.size);
    printf("请输入起始地址:");
    scanf("%d",&memory.address);
    notAllocate = (node*)malloc(sizeof(node));
    notAllocate->id = 1;
    notAllocate->size = memory.size;
    notAllocate->address = memory.address;
    strcpy(notAllocate->flag, "空闲");
    notAllocate->next = NULL;

    return NULL;

}

//函数名:operation 参数:node* head
int operation()
{
    //函数功能：打印操作菜单，选择需要进行的操作,输入1进行内存分配,输入2进行内存去配,输入0退出
    printf("*********可变分区管理*********\n");
    printf("   *     1.内存分配      *\n");
    printf("   *     2.内存去配      *\n");
    printf("   *     0.退出          *\n");
    printf("请输入选项:");
    int choice;
    scanf("%d",&choice);
    return choice;

}
//函数名:allocate 参数:node* head
void allocate()
{
    //函数名：输入作业名和大小，默认采用最先分配
    printf("1.内存分配\n");
    print();
    printf("请输入作业名:");
    char name[10];
    int memorySize,segmentNum;
    scanf("%s",name);
    printf("请输入%s需要分配的主存大小:",name);
    scanf("%d",&memorySize);
    printf("请输入要分成几段:");
    scanf("%d",&segmentNum);
    if (memorySize > notAllocate->size) {  //如果内存不足则不允许分配
        printf("分配失败!");

    }
    firstAllocation(memorySize,name,segmentNum);


}

//函数名:firstAllocation 参数:node* head,int size,char c[10],int segmentNum
void firstAllocation(int size, char c[10],int segmentNum)
{
    //函数功能：对作业进行分段，并采用最先分配，分段结束后打印段表
    int memorySize = size;
    int segSize;

    segmentNode* segNode;
    segNode = (segmentNode*)malloc(sizeof(segmentNode));  //新建一个段表结点
    strcpy(segNode->name, c);  //将作业名存入段表中
    segNode->next = NULL;
    if (segment == NULL) {
        segment = segNode;
    }
    else {
        segmentNode* s = segment;
        while (s->next != NULL) {
            s = s->next;
        }
        s->next = segNode;  // 在段表链表后面插入新结点
    }

    for (int i = 1; i <= segmentNum; i++) {

        printf("剩余%d的内存,请输入第%d段的大小:",memorySize,i);
        scanf("%d",&segSize);
        while (segSize > memorySize) {
            printf("请重新输入第%d段的大小:",i);
            scanf("%d", &segSize);
        }

        memorySize -= segSize; //分配段后就更改剩余内存


        node* allocateSeg;
        allocateSeg = (node*)malloc(sizeof(node));  //作业每一段的结点
        allocateSeg->next = NULL;
        

        //在已分配表中插入新节点
        if (head == NULL) {
            head = allocateSeg;
        }
        else {
            node* p = head;
            while (p->next != NULL) {
                p = p->next;
            }
            p->next = allocateSeg; // 队尾插入结点
        }

        // 修改已分配表
        // sprintf()函数可以格式化输入
        sprintf(allocateSeg->flag, "%s%d", c, (i - 1));   //将对应段的状态修改，后面的数字代表作业的第几段
        
        allocateSeg->size = segSize;
        allocateSeg->address = notAllocate->address;
        allocateSeg->next = NULL;

        // 修改未分配表
        notAllocate->address += segSize;
        notAllocate->size -= segSize;

        //修改已分配、未分配分区的分配号
        reorder();

        //修改段表
        segNode->a[i - 1][0] = i-1;   //修改短号
        segNode->a[i - 1][1] = segSize; //修改段长
        segNode->a[i - 1][2] = notAllocate->address;  //修改基址
        
    }
    segNode->num = segmentNum;
    printf("分配成功!\n");
    
    // 打印段表
    printf("********************打印%s段表********************\n",c);
    printf("段号\t段长\t基址\n");
    for (int i = 0; i < segmentNum; i++) {
        printf("%d\t%d\t%d\n",segNode->a[i][0],segNode->a[i][1],segNode->a[i][2]);
    }


}

//函数名:reorder 参数:node* head
void reorder()
{
    //函数功能：对分区和未分区的存储区域进行编号  
    int count = 1;
    node* p = head;
    // 先对已分配表进行编号
    while (p != NULL) {
        p->id = count;
        p = p->next;
        count++;
    }
    node* q = notAllocate;
    while (q != NULL) {
        q->id = count;
        q = q->next;
        count++;
    }

}

//函数名:recycle 
void recyle() //回收算法
{
    //函数功能：对归还分区按情况进行处理，有上邻有下邻，有上邻无下邻，无上邻有下邻，无上邻无下邻
    printf("2.内存去配\n");
    print();
    printf("请输入您想回收的作业名:");
    char name[10];
    scanf("%s",name);

    segmentNode* p = segment;  
    int flag = -1;
    while (p != NULL) {
        if (strcmp(p->name, name) == 0) {
            flag = 1; //p指针找到对应作业名对应的段表
            break;
        }
        p = p->next;  
    }
    if (flag == -1) {
        printf("回收%s失败,无此作业!\n",name);
        return;
    }

    node* h = head,* hpre = NULL; //分别指向分配表结点、及前驱结点
    int count = 0,address;  //count代表当前正在查询段表中第几个段号，address代表该段对应的起始地址
     
    while (h != NULL && count < p->num) {  //遍历分配表
        address = p->a[count][2] - p->a[count][1];  //获得某段对应起始地址
        if (h->address != address) {
            hpre = h;
            h = h->next;
        }
        else {  //找到该作业的该段
            node* pos; //指向需要删除的结点
            //删除已分配表中的结点
            if (h == head) { //如果要去配的是已分配表的第1个结点
                pos = head;
                head = head->next;
                h = head;
                hpre = NULL;
                
            }
            else {
                pos = h;
                hpre->next = h->next;
                h = h->next;
            }

            //在未分配表中添加结点，需要分4种情况
            node * not = notAllocate,*notPre = NULL; //分别指向未分配表结点、及其前驱结点
            while (not!= NULL) {  //遍历未分配表
                
                if (notPre == NULL &&(pos->address + pos->size) < not->address) {  //无上下相邻
                    pos->next = notAllocate;  //插到最前面
                    notAllocate = pos;
                    strcpy(pos->flag, "空闲");
                    break;
                }
                if ((not->address + not->size) == pos->address) {  //上邻
                    not->size += pos->size;
                    break;
                }
                if (notPre == NULL && (pos->address + pos->size) > not->address) {
                    notPre = not;
                    not= not->next;
                    continue;
                }
                if (notPre != NULL && (notPre->address + notPre->size) < pos->address && (pos->address + pos->size) < not->address) {  //无上下相邻
                    pos->next = not;
                    notPre->next = pos;
                    strcpy(pos->flag, "空闲");
                    break;
                }
                if ((pos->address + pos->size) == not->address && (notPre->address + notPre->size) == pos->address) { //上下相邻合并
                    notPre->size += (pos->size + not->size);
                    notPre->next = not->next;
                    break;
                }
                else if ((pos->address + pos->size) == not->address) {  //下邻
                    not->address -= pos->address;
                    not->size += pos->size;
                    break;
                }
                //else if ((not->address + not->size) == pos->address) {  //上邻
                //    not->size += pos->size;
                //    break;
                //}
                
                notPre = not;
                not= not->next;
            }
            printf("回收%s的段%s%d成功!\n",p->name,p->name,count);

            count++;

        }


        
    }
    
    reorder();
    print();


}
//函数名:print参数:node* head
void print()
{
    //函数功能：打印主存分配表
    printf("********************主存分配情况********************\n");
    printf("已分配:\n");
    printf("分配号\t大小(KB)\t起始(KB)\t状态\n");
    node* p = head;
    while (p != NULL) {
        printf("%d\t%d\t\t%d\t\t%s\n",p->id,p->size,p->address,p->flag);
        p = p->next;
    }
    printf("\n未分配:\n");
    printf("分配号\t大小(KB)\t起始(KB)\t状态\n");
    node* q = notAllocate;
    while (q != NULL) {
        printf("%d\t%d\t\t%d\t\t%s\n", q->id, q->size, q->address, q->flag);
        q = q->next;
    }
}

int main()
{
    
    head = initMemory();
    while (1)
    {
        int c;
        c = operation();
        if (c == 1) {
            allocate();

        }
        else if (c == 2) {
            recyle();
        }else 
        {
            break;
        }
    }
    return 0;
}

/*

测试用例：

256
40
1
jobA
50
2
20
30
1
jobB
100
3
30
35
35
2
jobB
2
jobA
0

*/



