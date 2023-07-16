#include <stdio.h>
#include <stdlib.h>

typedef struct Time {
    int hour;
    int min;
} Time;

typedef struct Node {
    int id;
    char name[20];
    Time arrive;
    int zx;
    Time start;
    Time finish;
    int zz;
    float zzxs;
    struct Node *next;
} Node;

Node *createNode() {
    Node *p = (Node *)malloc(sizeof(Node));
    scanf("%d\t%s\t%d:%d\t%d",&p->id,&p->name,&p->arrive.hour,&p->arrive.min,&p->zx);
    p->next = NULL;
    return p;
}

void printNode(Node *p) {
    printf("%d\t%s\t%d\t%02d:%02d\t%02d:%02d\t%02d:%02d\t%d\t%.2f\n", p->id, p->name, p->arrive.hour, p->arrive.min, p->start.hour, p->start.min, p->finish.hour, p->finish.min, p->zz, p->zzxs);
}

void printNodeList(Node *head) {
    printf("编号\t名称\t到达\t执行\t完成\t周转\t周转系数\n");
    Node *p = head;
    while (p != NULL) {
        printNode(p);
        p = p->next;
    }
}

Node *insertNode(Node *head, Node *p) {
    if (head == NULL) {
        head = p;
    } else {
        Node *prev = NULL;
        Node *curr = head;
        while (curr != NULL && curr->arrive.hour*60+curr->arrive.min <= p->arrive.hour*60+p->arrive.min) {
            prev = curr;
            curr = curr->next;
        }
        if (prev == NULL) {
            p->next = head;
            head = p;
        } else {
            p->next = curr;
            prev->next = p;
        }
    }
    return head;
}


int main() {
    Node *head = NULL;
    int n,flag;
    while(1) {
        printf("请输入操作:(1:开始进程;-1:结束进程):");
        scanf("%d",&flag);
        if(flag==-1) {
            break;
        }
        printf("请输入进程数：");
        scanf("%d", &n);
        printf("请输入进程参数：\n");
        printf("id号\t名字\t到达\t执行(使用tab分隔):\n");  
        for (int i = 0; i < n; i++) {
            Node *p = createNode();
            head = insertNode(head, p);
        }
        Node *p = head;
        int time = 0;
        while (p != NULL) {
            if (p->arrive.hour*60+p->arrive.min > time) {
                time = p->arrive.hour*60+p->arrive.min;
            }
            p->start.hour = time / 60;
            p->start.min = time % 60;
            p->finish.hour = (time + p->zx) / 60;
            p->finish.min = (time + p->zx) % 60;
            p->zz = p->finish.hour*60+p->finish.min - p->arrive.hour*60-p->arrive.min;
            p->zzxs = (float)p->zz / p->zx;
            time += p->zx;
            p = p->next;
        }
        printNodeList(head);                                         
    }
    return 0;
}
