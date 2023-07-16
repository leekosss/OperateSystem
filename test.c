#include <stdio.h>
#include <stdlib.h>

#define MAX_PROCESS_NUM 10 // 最大进程数
#define TIME_SLICE 3 // 时间片长度

// 进程状态
typedef enum {
    READY, // 就绪
    RUNNING, // 运行
    FINISHED // 完成
} ProcessStatus;

// 进程控制块
typedef struct {
    int pid; // 进程ID
    int arrive_time; // 到达时间
    int need_time; // 需要运行时间
    int run_time; // 已运行时间
    ProcessStatus status; // 进程状态
} PCB;

// 就绪队列
typedef struct {
    PCB *processes[MAX_PROCESS_NUM]; // 进程指针数组
    int front; // 队首指针
    int rear; // 队尾指针
} ReadyQueue;

// 初始化就绪队列
void initReadyQueue(ReadyQueue *queue) {
    queue->front = 0;
    queue->rear = 0;
}

// 将进程插入就绪队列尾部
void insertProcess(ReadyQueue *queue, PCB *process) {
    queue->processes[queue->rear] = process;
    queue->rear = (queue->rear + 1) % MAX_PROCESS_NUM;
}

// 从就绪队列中删除进程
void deleteProcess(ReadyQueue *queue, int index) {
    int i;
    for (i = index; i < queue->rear - 1; i++) {
        queue->processes[i] = queue->processes[i + 1];
    }
    queue->rear--;
}

// 获取就绪队列中的下一个进程
PCB *getNextProcess(ReadyQueue *queue) {
    if (queue->front == queue->rear) {
        return NULL; // 队列为空
    }
    PCB *process = queue->processes[queue->front];
    queue->front = (queue->front + 1) % MAX_PROCESS_NUM;
    return process;
}

// 打印进程信息
void printProcess(PCB *process) {
    printf("PID: %d, arrive_time: %d, need_time: %d, run_time: %d, status: %d\n",
           process->pid, process->arrive_time, process->need_time, process->run_time, process->status);
}

int main() {
    int i;
    int total_time = 0; // 总时间
    int finished_num = 0; // 已完成进程数
    PCB *running_process = NULL; // 当前正在运行的进程
    ReadyQueue ready_queue; // 就绪队列
    initReadyQueue(&ready_queue);

    // 创建进程
    PCB processes[MAX_PROCESS_NUM] = {
            {1, 0, 5, 0, READY},
            {2, 1, 3, 0, READY},
            {3, 2, 4, 0, READY},
            {4, 3, 2, 0, READY},
            {5, 4, 4, 0, READY}
    };

    // 将进程插入就绪队列
    for (i = 0; i < MAX_PROCESS_NUM; i++) {
        insertProcess(&ready_queue, &processes[i]);
    }

    // 时间片轮转调度
    while (finished_num < MAX_PROCESS_NUM) {
        // 获取下一个进程
        PCB *next_process = getNextProcess(&ready_queue);
        if (next_process == NULL) {
            total_time++;
            continue; // 队列为空，继续下一轮调度
        }

        // 将当前正在运行的进程重新插入就绪队列尾部
        if (running_process != NULL && running_process->status == RUNNING) {
            running_process->status = READY;
            insertProcess(&ready_queue, running_process);
        }

        // 将下一个进程设置为当前正在运行的进程
        running_process = next_process;
        running_process->status = RUNNING;

        // 运行一个时间片
        for (i = 0; i < TIME_SLICE; i++) {
            total_time++;
            running_process->run_time++;

            // 进程运行完成
            if (running_process->run_time == running_process->need_time) {
                running_process->status = FINISHED;
                finished_num++;
                printf("Process %d finished at time %d\n", running_process->pid, total_time);
                break;
            }
        }

        // 时间片用完，将当前正在运行的进程重新插入就绪队列尾部
        if (running_process->status == RUNNING) {
            running_process->status = READY;
            insertProcess(&ready_queue, running_process);
        }

        // 打印当前正在运行的进程和就绪队列中各个进程的PCB信息
        printf("Running process: ");
        printProcess(running_process);
        printf("Ready queue: ");
        for (i = ready_queue.front; i != ready_queue.rear; i = (i + 1) % MAX_PROCESS_NUM) {
            printProcess(ready_queue.processes[i]);
        }
    }

    // 输出所有进程的相关信息
    // 输出所有进程的相关信息
    printf("All processes finished at time %d\n", total_time);
    return 0;
}
