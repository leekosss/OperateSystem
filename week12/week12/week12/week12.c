#include<stdio.h>
#include<math.h>

#define N 1000

int length;     //访问序列的长度
int sequence[N];//访问柱面的顺序（从1开始）
int result[N];//存放最短优先的结果
int now;      //当前柱面 


//函数名：in参数：无
void in() {
    //函数功能:输入访问序列信息
    printf("请输入访问序列的长度:");
    scanf("%d",&length);
    printf("请输入访问的柱面顺序:");

    for (int i = 1; i <= length ; i++) {  //从1开始
        scanf("%d",&sequence[i]);
    }
    printf("请输入正在访问的柱面:");
    scanf("%d",&now);


}
//函数名：FCFS参数：无
void FCFS() {
    //函数功能:调用先来先服务算法
    printf("***************FCFS磁盘移臂调度过程***************\n");
    in();
    printf("FCFS移动的顺序为:\n");
    printf("%d ",now);
    for (int i = 1; i <= length; i++) {
        printf("%d ",sequence[i]);
    }
    int num = 0;
    num += abs(now - sequence[1]);
    for (int i = 2; i <= length; i++) {
        num += abs(sequence[i] - sequence[i - 1]);
    }
    printf("\n移动柱面为:%d\n",num);
    
}
//函数名：SSTF参数：无
void SSTF() {
    //函数功能:调用短进程优先调度算法
    printf("***************SSTF磁盘移臂调度过程***************\n");
    in();
    for (int i = 1; i <= length; i++) {  //先给结果数组赋初值
        result[i] = sequence[i];
    }
    result[0] = now;
    int min = N , minPos;
    int temp , number,flag;
    for (int i = 1; i < length; i++) {
        flag = -1;
        for (int j = i; j <= length; j++) {
            number = abs(result[i-1] - result[j]);
            if (number < min) {
                min = number;
                minPos = j;
                flag = 1;
            }
        }
        if (flag != -1) {
            temp = result[minPos];
            result[minPos] = result[i];
            result[i] = temp;
            
        }
        min = N;
    }
    int num = 0;
    printf("最短优先的顺序为:\n");
    for (int i = 0; i <= length; i++) {
        printf("%d ",result[i]);
        if (i != 0) {
            num += abs(result[i] - result[i - 1]);
        }
    }
    printf("\n移动柱面为:%d\n",num);

}
//函数名：Elevator参数：无
void Elevator() {
    //函数功能:调用电梯调度算法
    printf("***************电梯调度过程***************\n");
    in();
    int min = sequence[1], minPos, flag, temp;
    for (int i = 1; i < length; i++) {   //首先将值进行升序排列
        flag = -1;
        for (int j = i; j <= length; j++) {
            if (sequence[j] < min) {
                min = sequence[j];
                minPos = j;
                flag = 1;
            }


        }
        if (flag != -1) {
            temp = sequence[minPos];
            sequence[minPos] = sequence[i];
            sequence[i] = temp;

        }
        min = sequence[i + 1];
    }
    sequence[0] = now;


    printf("电梯调度(由里向外)移动的顺序为\n");
    int num = 0;
    flag = 0;//flag标记位置
    printf("%d ", now);
    for (int i = length; i >= 1; i--) {
        if (sequence[i] < now) {
            printf("%d ", sequence[i]);
            num += now - sequence[i];
            now = sequence[i];
        }
        else {
            flag = i;
        }
    }


    for (int i = flag; i <= length; i++) {
        printf("%d ", sequence[i]);
        num += sequence[i] - now;
        now = sequence[i];
    }
    printf("\n移动柱面为:%d\n", num);


    now = sequence[0];

    printf("电梯调度(由外向里)移动的顺序为\n");
    num = 0;
    flag = 0;//flag标记位置
    printf("%d ", now);
    for (int i = 1; i <= length; i++) {
        if (sequence[i] > now) {
            printf("%d ", sequence[i]);
            num += sequence[i] - now;
            now = sequence[i];
        }
        else {
            flag = i;
        }
        if (i == length) {  
            now = sequence[i];
        }
    }


    for (int i = flag; i >= 1; i--) {
        printf("%d ", sequence[i]);
        num += now - sequence[i];
        now = sequence[i];
    }
    printf("\n移动柱面为:%d\n", num);


}
//函数名：Oneway_scan参数：无
void Oneway_scan() {
    //函数功能:调用单向扫描算法
    printf("***************单向扫描过程***************\n");
    in();
    int min = sequence[1] , minPos , flag, temp;
    for (int i = 1; i < length; i++) {   //首先将值进行升序排列
        flag = -1;
        for (int j = i; j <= length; j++) {
            if (sequence[j] < min) {
                min = sequence[j];
                minPos = j;
                flag = 1;
            }


        }
        if (flag != -1) {
            temp = sequence[minPos];
            sequence[minPos] = sequence[i];
            sequence[i] = temp;
            
        }
        min = sequence[i + 1];
    }
    sequence[0] = now;

    
    printf("单向扫描移动的顺序为\n");
    int num = 0;  
    flag = 0;//flag标记位置
    printf("%d ",now);
    for (int i = 1; i <= length; i++) {
        if (sequence[i] > now) {
            printf("%d ", sequence[i]);
            num += sequence[i] - now;
            now = sequence[i];
        }
        else {
            flag = i;
        }
        if (i == length) {  //如果到了最后一个，还是需要移动到最后
            num += 199 - sequence[i];
        }
        
    }
    num += 199;   //柱面长度
    now = 0;
    printf("%d %d ",199,0);
    for (int i = 1; i <= flag; i++) {
        printf("%d ",sequence[i]);
        num += sequence[i] - now;
        now = sequence[i];
    }
    printf("\n移动柱面为:%d\n",num);



}
//函数名：Twoway_scan参数：无
void Twoway_scan() {
    //函数功能:调用双向扫描算法
    printf("***************双向扫描过程***************\n");
    in();
    int min = sequence[1], minPos, flag, temp;
    for (int i = 1; i < length; i++) {   //首先将值进行升序排列
        flag = -1;
        for (int j = i; j <= length; j++) {
            if (sequence[j] < min) {
                min = sequence[j];
                minPos = j;
                flag = 1;
            }


        }
        if (flag != -1) {
            temp = sequence[minPos];
            sequence[minPos] = sequence[i];
            sequence[i] = temp;

        }
        min = sequence[i + 1];
    }
    sequence[0] = now;

    printf("双向扫描移动的顺序为\n");
    int num = 0;
    flag = 0;//flag标记位置
    printf("%d ", now);
    for (int i = 1; i <= length; i++) {
        if (sequence[i] > now) {
            printf("%d ", sequence[i]);
            num += sequence[i] - now;
            now = sequence[i];
        }
        else {
            flag = i;
        }
        if (i == length) {  //如果到了最后一个，还是需要移动到最后
            num += 199 - sequence[i];
        }

    }

    //num += 199;   //柱面长度
    now = 199;
    printf("%d ",now);
    for (int i = flag; i >= 1; i--) {
        printf("%d ", sequence[i]);
        num += abs(sequence[i] - now);
        now = sequence[i];
    }
    printf("\n移动柱面为:%d\n", num);



}
//函数名：Out参数：无
void Out() {
    //函数功能:输出选项菜单
    printf("***************磁盘驱动调度***************\n");
    printf("     *         1.FCFS          *\n");
    printf("     *         2.SSTF          *\n");
    printf("     *         3.单项扫描      *\n");
    printf("     *         4.双向扫描      *\n");
    printf("     *         5.电梯调度      *\n");
    printf("     *         0.退出          *\n");
    printf("               请输入选项[ ]\b\b");


}
int main()
{
    while (1)
    {
        Out();  //给出选项
        int c;
        scanf("%d", &c);
        switch (c) {  //选择算法
        case 1:
            FCFS();
            break;
        case 2:
            SSTF();
            break;
        case 3:
            Oneway_scan();
            break;
        case 4:
            Twoway_scan();
            break;
        case 5:
            Elevator();
            break;
        default:
            printf("\n调度结束！");
            exit(1);
        }
    }
    return 0;
}

