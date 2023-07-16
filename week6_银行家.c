/*
操作系统按照银行家制定的规则为进程分配资源：

1. 当进程首次申请资源时，要测试该进程对资源的最大需求量，
如果系统现存的资源可以满足它的最大需求量则按当前的申请量分配资源，否则就推迟分配。

2. 当进程在执行中继续申请资源时，先测试该进程本次申请的资源数是否超过了对该资源剩余总量。
若超过则拒绝分配资源，若能满足则继续比较当前申请量与系统剩余可用量，
若剩余可用量大于本次申请量则按当前的申请量尝试性分配资源，否则也要推迟分配。

3. 对本次尝试性分配进行确认：分别对所有未结束的进程进行判断，若能找到一个安全执行序列
，则本次尝试性分配进行确认，否则执行回滚操作并恢复尝试性分配前的状态。
*/

#include<stdio.h>
#include<string.h>
#define MAX 20
int safe();
int bank();
void show();

int N,M;//N是资源数,M是进程数
int Available[MAX];//avaiable[i]表示系统中第i类资源的剩余量
int Max[MAX][MAX];//Max[i][j]表示第i个进程对第j类资源的最大需求
int Need[MAX][MAX];//need[i][j]表示第i个进程剩余所需第j类资源的数量
int Allocation[MAX][MAX];//Allocation[i][j]表示第i个进程已占有第j类资源的数量
int Finish[MAX];//finish[i]表示系统是否有能力使第i个进程执行完成
int Request[MAX][MAX];//Request[i][j]表示第i个进程某次申请第j类资源的数量
int State[MAX];//State[i]表示第i个进程所处状态（完成，进行中）
int Work[MAX];//Work[i]表示第i类资源数量
char Name[MAX][15];//记录进程的名字
int safe_arr[MAX]; //安全队列

/*
初始化相关资源
*/
void init() {
    printf("请输入进程数量:\n");
    scanf("%d",&M);
    printf("请输入资源种类的数量:\n");
    scanf("%d",&N);
    printf("请输入%d类资源初始化的资源数:\n",N);
    for(int i=0;i<N;i++) {
        scanf("%d",&Available[i]);  //资源初始化的资源
    }
    printf("请输入进程的相关信息:\n");
    printf("进程名\t最大需求量\n");
    for(int i=0;i<M;i++) {
        scanf("%s",&Name[i]);
        for(int j=0;j<N;j++) {
            scanf("%d",&Max[i][j]);
            Need[i][j]=Max[i][j];
        }
    }
    printf("进程开始执行!\n");

    printf("请为进程分配相关资源!\n");
    bank();
    show();

}

/*
安全算法
*/
int safe() {
    
    for(int i=0;i<N;i++) {
        Work[i]=Available[i];  //初始化可工作资源Work
    }
    int i,count,num=0;
    for(i=0;i<M;i++) {  //遍历每一个进程
        count=0;
        for(int j=0;j<N;j++) {  //遍历每一个资源

            if(Finish[i]==0&&Need[i][j]<=Work[j]) {  //满足条件
                count++;
                if(count==N) {   //全满足条件，代表该进程可以执行完成
                    Finish[i]=1;

                    for(int k=0;k<N;k++) {
                        Work[k] += Allocation[i][k];   //可工作资源加上该进程以前分配的资源
                    }

                    safe_arr[num]=i;  //设置安全序列
                    num++;
                    i=-1;  // ***这里是从头开始找可以执行的进程
                }

            }
        }

    }
    for(i=0;i<M;i++) {
        if(Finish[i]==0) {
        	
        	for(int i=0;i<M;i++) {
    			Finish[i]=0;   //将Finish数组重置为0 
			}
			
            return 0;
        }
    }
    printf("申请成功!安全序列为:(可能存在多个安全序列)");
    for(i=0;i<M;i++) {
        if(State[i]==0) {
        	printf("%s",Name[safe_arr[i]]);
	        if(i<M-1) {
	            printf("->");
	        }
		}
    }
    printf("\n");
    
    for(int i=0;i<M;i++) {
    	Finish[i]=0;   //将Finish数组重置为0 
	}
    
    
    return 1;

}


/*
银行家算法
*/
int bank() {
    char name[15]; //进程的名字
	int ret = -1; //返回值

    printf("请输入进程名:\n");
    scanf("%s",name);
    int n=-1;
    while(1) { //找进程所在的编号
        for(int i=0;i<sizeof(Name)/sizeof(Name[0]);i++) {
            if(strcmp(name,Name[i])==0) {
                n=i;
            }
        }
        if(n!=-1) {
            break;
        }
        printf("请重新输入进程名:\n");
        scanf("%s",name);
    }

    
    while(1) {

        printf("请输入该进程当前申请的各个资源数量:\n");
        for(int i=0;i<N;i++) {
            scanf("%d",&Request[n][i]);
        }

        int flag=0;
        for(int i=0;i<N;i++) {
            if(Request[n][i]<=Need[n][i]&&Request[n][i]<=Available[i]) {
                flag+=1; //判断是否满足资源，统计满足资源的个数
            }
        }
        if(flag==N) {   //如果满足资源申请的条件
			
			for(int i=0;i<N;i++) {
                Available[i]-=Request[n][i];
                Need[n][i]-=Request[n][i];
                Allocation[n][i]+=Request[n][i];
            }
			
			ret = safe(); 
            if(ret) {
                int stat=0;
                for(int i=0;i<N;i++) {
                    
                    
                    if(Need[n][i]==0) {
                        stat++;
                    }
                }
                if(stat==N) {
                	
                	for(int i=0;i<N;i++) {
                		Available[i] += Allocation[n][i];  //全部满足需求就释放资源 
                		Allocation[n][i] = 0; 
					}
					
                    State[n]=1;  //更新进程状态
                }
                break;
            }else{
            	
            	for(int i=0;i<N;i++) {  //不满足就恢复 
	                Available[i]+=Request[n][i];
	                Need[n][i]+=Request[n][i];
	                Allocation[n][i]-=Request[n][i];
            	}
            	
                printf("无安全序列，请重新输入!\n");
                break;
            }


        }else {  //不满足条件
            printf("申请异常!\n");
            printf("系统可用资源如下:\n");
            for(int i=0;i<N;i++) {
                printf("%d ",Available[i]);
            }
            printf("\n当前进程剩余所需资源如下:\n");
            for(int i=0;i<N;i++) {
                printf("%d ",Need[n][i]);
            }
            printf("\n请重新输入!\n");
        }
    }
    return ret;
}



/*
打印进程信息
*/
void show() {
    
    printf("进程名\t最大需求量\t需求量\t已分配量\t执行状态\n");
    for(int i=0;i<M;i++) {
        printf("%s\t",Name[i]);
        for(int j=0;j<N;j++) {
            printf("%d ",Max[i][j]);
        }
        printf("\t\t");
        for(int j=0;j<N;j++) {
            printf("%d ",Need[i][j]);
        }
        printf("\t");
        for(int j=0;j<N;j++) {
            printf("%d ",Allocation[i][j]);
        }
        if(State[i]==0) {
            printf("\t\tWorking\n");
        }else{
            printf("\t\tFinished\n");
        }
    }
    printf("系统剩余资源:\n");
    for(int i=0;i<N;i++) {
        printf("%d ",Available[i]);
    }
    getchar();
    printf("\n");

}


int main() {
    init();
    char flag;
    while(1) {
        printf("是否需要申请资源?(Y/N):");
        scanf("%c",&flag);
        if(flag=='N'||flag=='n') {
            break;
        }
        if(flag!='N'&&flag!='Y'&&flag!='n'&&flag!='y') {
            printf("请重新输入!\n");
            continue;
        }
        int ret;
        ret = bank();
        if(ret) {
        	show();
		}

    }

    return 0;
}