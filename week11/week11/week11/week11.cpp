/*
本实验是模拟虚拟存储管理方式中的请求分页式技术中的页面置换算法，
对于当前访问的页面号如果有对应的内存物理块号则直接访问(必要时可调整物理块中的页面序号)，
否则先输出缺页访问标志+，然后再判断物理块是否已存满，
未满则直接换入当前页，若已满，则先将内存物理块中块首页面号淘汰且换出，
然后再将当前页换入。
FIFO是选择在内存驻留时间最长的页面进行置换。
LFU是选择最长时间没有被引用的页面进行置换。

*/

#include<bits/stdc++.h>
using namespace std;
const int N = 1e3 + 10;

struct Y
{
    int cnt, val; //cnt为优先级 val为页面  cnt越大,优先级越高，越优先出队
    friend bool operator<(Y a, Y b)
    {
        return a.cnt < b.cnt;
    }
};
priority_queue< Y, vector<Y> > q;

string s; //作业名
int n, m, ss[N], vis[N], ans[N][N], u[N];
//n为物理块的块数
//m为作业长度
//ss[N] 记录需要调度的页面顺序
//vis数组用于记录本次是否产生缺页面
//二维数组ans用于记录调度过程中的页面置换情况



vector<int> v; //用于存放物理块序列


//函数名：out  参数：无
void out()
{
    //函数功能：输出选择界面菜单，1为FIFO，2为LFU，0退出
    puts("***********请求分页式存储管理***********");
    puts("\t*\t1.FIFO分配\t*\t");
    puts("\t*\t2.LRU(LFU)分配\t*\t");
    puts("\t*\t0.退出\t*\t");
    printf("\t\t请输入选项：");

}
//函数名：input  参数：无
void input()
{
    //函数功能：输入作业信息
    printf("请输入作业名:");
    std::cin >> s;    //将输入的作业名字符串赋值给s
    printf("请输入作业页面长度:");
    std::cin >> m;
    printf("请输入作业页面顺序:");
    for (int i = 0; i < m; i++) {
        scanf("%d",&ss[i]);
    }

    for (int i = 0; i < n ; i++) {
        v.push_back(i);  // 为物理块序列赋值
    }

}
//函数名：print    参数：无
void print()
{
    //函数功能：输出页面置换详情和缺页中断率
    printf("作业调度过程:\n");
    


}
//函数名：solve1   参数：无
void solve1()
{
    //函数功能：进行FIFO调度


}
//函数名：solve2   参数：无
void solve2()
{
    //函数功能：进行LRU调度


}

int main()
{
    printf("请输入物理块的块数：");
    cin >> n;
    out();
    int f;
    cin >> f;
    while (f != 0)
    {
        if (!f)
            break;
        input();
        memset(ans, -1, sizeof ans);   //将ans的所有值赋值为-1
        memset(vis, 0, sizeof vis);
        memset(u, -1, sizeof u);
        //二维数组ans用于记录调度过程中的页面置换情况
        //vis数组用于记录本次是否产生缺页面
        
        if (f == 1)
        {
            solve1();
            print();
        }
        else
        {
            solve2();
            print();
        }
        out();
        cin >> f;
    }

    return 0;
}


/*
测试用例：
3
1
jobA
20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
2
jobB
20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
0

4
1
JobC
20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
2
jobD
20
7 0 1 2 0 3 0 4 2 3 0 3 2 1 2 0 1 7 0 1
0

*/

