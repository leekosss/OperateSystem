#include<stdio.h>


const int N = 1e3 + 10;
int length;
int sequence[N];//访问柱面的顺序（从1开始）
int result[N];//存放最短优先的结果
int now;//当前柱面 


//函数名：in参数：无
void in() {
    //函数功能:输入访问序列信息
}
//函数名：FCFS参数：无
void FCFS() {
    //函数功能:调用先来先服务算法
}
//函数名：SSTF参数：无
void SSTF() {
    //函数功能:调用短进程优先调度算法
}
//函数名：Elevator参数：无
void Elevator() {
    //函数功能:调用电梯调度算法
}
//函数名：Oneway_scan参数：无
void Oneway_scan() {
    //函数功能:调用单向扫描算法
}
//函数名：Twoway_scan参数：无
void Twoway_scan() {
    //函数功能:调用双向扫描算法
}
//函数名：Out参数：无
void Out() {
    //函数功能:输出选项菜单
}
int main()
{
    while (1)
    {
        Out();//给出选项
        int c;
        scanf("%d", &c);
        switch (c) {//选择算法
        case 1:FCFS();
            break;
        case 2:SSTF();
            break;
        case 3:Oneway_scan();
            break;
        case 4:Twoway_scan();
            break;
        case 5:Elevator();
            break;
        default:printf("\n调度结束！");
            exit(1);
        }
    }
    return 0;
}

