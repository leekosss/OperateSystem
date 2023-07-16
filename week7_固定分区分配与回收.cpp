/*
本实验是模拟存储管理方式中的固定分区分配与回收算法，
系统在作业装入前预分将整个用户区划分为若干个大小确定的分区，
然后根据待装入作业的名称和大小到分区列表中查找满足要求的空闲分区，
再将该分区状态标志修改为作业名； 当作业运行结束时，
根据该作业名查找分区列表找到与作业名相同的分区，再将其状态标志修改为空
*/

#include<iostream>
#include<string>
#include <iterator>
using namespace std;



int n, m;//分区数量， 作业个数 
int v[1000000];//存储作业大小 
struct Y{
	int id;//分区号
	int sz;//分区大小
	int address;//分区始址
	string station;//状态 
}a[110];



//函数名：input    函数参数：无
Y input() {
//函数功能：输入一个分区信息包括分区号，分区大小和分区始址
	Y y;
	scanf("%d\t%d\t%d",&y.id,&y.sz,&y.address);
	y.station = "0";
	return y;
}

//函数名：print    函数参数：无
void print() {
    //函数功能：输出所有分区信息（包括此时的状态，未被占用输出0，被占	用输出占用该分区的作业名）
    printf("*********打印分区信息**********\n");
    printf("分区号\t大小(kb) 起始(kb)\t状态\n");
    for(int i=1;i<=n;i++) {
        printf("%d\t%d\t%d\t\t%s\n",a[i].id,a[i].sz,a[i].address,a[i].station.c_str());
    }
    
}

int main() {
	printf("请输入系统的分区块数：");
    scanf("%d", &n);
	printf("请依次输入：\n分区号\t大小\t起始\n");
	for (int i = 1; i <= n; i++){
        a[i] = input(); 
    }
    print();
	printf("请输入作业的个数：");
    scanf("%d", &m);
	printf("请输入这%d个作业的信息：\n", m);
	for(int i = 1; i <= m; i++) {
		printf("请输入作业%d的大小：",i);
		scanf("%d", &v[i]);
	}
	printf("打印各作业的信息：\n");
	printf("作业名 作业大小\n");
	for(int i = 1; i <= m; i++) { 
        printf("JOB%d %dKB\n", i, v[i]);
		for(int j = 1; j <= n; j++) {
			if(a[j].sz >= v[i] && a[j].station == "0") {
				//找到可以容纳得下该作业且未被占用的分区
				a[j].station = "JOB";
				a[j].station += '0'+i;
				break;
			}
		}
	}
	print();
	while(true) {
		char op[2];
		printf("是否需要回收(y/n)？");
		scanf("%s", op);
		if(op[0] == 'n') 
			break; 
		print();
		string str;
		printf("请输入回收的作业名：");
		cin >> str;
		puts("回收成功");
		for(int i = 1; i <= n; i++) 
			if(a[i].station == str) 
			a[i].station = "0";//标记回未占用状态
		print();
	}
	return 0;
}


