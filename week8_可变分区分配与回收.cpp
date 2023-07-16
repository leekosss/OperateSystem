/*
本实验是模拟存储管理方式中的可变分区分配与回收算法，系统需要建立两张分区表，分别是未(已)分配分区表，
然后根据待装入作业的名称和大小到未分配分区列表中查找(具体包含最先/最优/最坏适配算法)满足要求的空闲分区，
再将该分区一分为二，前半部分分配给作业使用并登记到已分配分区表的空栏中，剩余部分仍修改为新的空闲分区，
最后通过动态重定位方式完成地址转换；当作业运行结束时，根据该作业名查找已分配分区表，找到与作业名相同的分区，
然后将该分配从已分配分区表中删除，最后按照无上邻无下邻、有上邻无下邻、无上邻有下邻和有上邻有下邻四种形式分别修改未分配分区表。
*/

#include<stdio.h>
#include<stdlib.h>
#include<string>


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
    int size;//分区大小
    int address; //分区始址
    char flag[20]; //分区状态，空闲或者占用作业
    struct link_node *next;

} node;

//作业结构体
typedef struct job_node
{    
    int id;//作业编号
    char name[20];//作业名
    int size;//作业大小
} job;


//函数名：out  参数：无
void out() {
//函数功能：打印主存分配表和未分配表  
    
}
//函数名：menu1  参数：无
void menu1() { 
//函数功能：打印分区管理菜单   
    printf("**********可变分区管理************\n");
    printf("\t*\t1.内存管理\t*\n");
    printf("\t*\t2.内存去配\t*\n");
    printf("\t*\t0.退出\t\t*\n");
    printf("\t\t请输入选项:");
}
//函数名：menu2  参数：无
void menu2() { 
//函数功能：打印算法选择菜单
    printf("**********分配算法************\n");
    printf("\t*\t1.最先分配算法\t*\n");
    printf("\t*\t2.最优分配算法\t*\n");
    printf("\t*\t3.最坏分配算法\t*\n");
    printf("\t\t请输入选项:");

}
//函数名：C1  参数：size 作业大小 , x 作业名
void C1(int size, char x[]) {
    //函数功能：模拟最先分配算法，并调用out()输出


    out();
}
//函数名：C2  参数：size 作业大小 , x 作业名
void C2(int size, char x[]) { 
    //函数功能：模拟最优分配算法，并调用out()输出
}
//函数名：C3  参数：size 作业大小 , x 作业名
void C3(int size, char x[]) { 
    //函数功能：模拟最差分配算法，并调用out()输出
}
//函数名：D  参数：x 作业名
void D(char x[]) { //回收算法
    //函数功能：回收作业内存，根据情况对分区进行管理，并调用out()输出
    
}
int main() {
    printf("请输入内存大小为：");
    scanf("%d", &memory.size);
    printf("请输入起始地址大小为:");
    scanf("%d", &memory.address);
    menu1();

    int choice;
    scanf("%d",&choice);
    while(1) {
        if(choice == 0) break;
        if(choice == 1) {
            menu2();
            int flag;
            scanf("%d", &flag);
            char tmp[20];
            // out();
            printf("请输入作业名:");
            scanf("%s",tmp);
            printf("请输入%s需要分配的主存大小(单位:KB):", tmp);
            int size;
            scanf("%d", &size);
            
            if(flag == 1) {
                C1(size, tmp);
            }
            else if(flag == 2) {
                C2(size, tmp);
            }
            else {
                C3(size, tmp);
            }
        }
        else {
            out();
            printf("请输入你要回收的作业名：");
            char tmp[20];
            scanf("%s",tmp);
            D(tmp);
        }
        // system("pause");
        printf("请按任意键继续...");
        getchar();
        getchar();
        menu1();
        scanf("%d",&choice);
    }
    return 0;
}


/*

测试用例：

256
40
1
1
JOB_A 
15
1
1
JOB_B
50
1
1
JOB_C 
10
1
1
JOB_D 
25
1
1
JOB_E 
14
2
JOB_B
2
JOB_D
1
1
JOB_F 
32
0
 */

