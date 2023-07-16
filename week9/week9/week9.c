/*
week9_��ɢ�洢����ʽ��ģ����ʵ��_��ҳʽ

��ʵ����ģ��洢����ʽ�еķ�ҳʽ����������㷨��ϵͳ��Ҫ�������ű�
�ֱ�������λʾͼ��(�����������ģ��ʵ��0/1��ʼֵ)����ҵҵ��
Ȼ����ݴ�װ����ҵ�����ƺ�ҳ����λʾͼ���в�������Ҫ��Ŀ�������飬
Ȼ��λʾͼ���Ӧλ�õ�0�ó�1���޸�ʣ����п������½�һ����ҵҳ��
����ҵ�߼�ҳ�ż�����ɹ�����������������˱����ͨ����̬�ض�λ��ʽ��ɵ�ַת����
����ҵ���н���ʱ�����ݸ���ҵ��������ҵҳ���ҵ�����ҵռ�õ������ţ�
����޸�λʾͼ����Ӧ�ϵĿ�ű�־�����ʣ����п�����
*/
#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>

#define MAX 1000

int sc[MAX][MAX] = { 0 };//ϵͳ�ڴ�
int MemorySize;//ϵͳ�ڴ��С
int WordLength;//�ֳ�
int WordNum;//ҳ��
int BlockSize;//������С
int BlockNum;//����������
int UsingBolckNum;//ʹ�õ������
int AvailableBlockNum;//��ʹ�õ������
//��ҵ�ڵ�
typedef struct link_node
{
    char name[10];//��ҵ��
    int size[MAX][MAX];//��ҵҳ��
    struct link_node* next;
} node;
node* h = NULL;


//��������OperationChoice       ������������
int OperationChoice()   //�����������ѡ��ѡ����ز���
{
    //�������ܣ���ӡ�����˵���ѡ����Ҫ���еĲ���
    int choice=0;
    printf("*********��ҳʽ����**********\n");
    printf("   *     1.�ڴ����      *\n");
    printf("   *     2.�ڴ�ȥ��      *\n");
    printf("   *     0.�˳�          *\n");
    printf("������ѡ��:");
    scanf("%d",&choice);
    return choice;
}
//��������print       ������������
void print()
{
    //�������ܣ���ӡϵͳ�ڴ���Ϣ
    int count = 0,num=0;
    printf("    ");
    for (int y = 0; y < WordLength; y++) {
        printf("%2d ", y);
    }
    printf("\n");
    for (int x = 0; x < WordNum; x++) {
        printf("%2d |", x);
        for (int y = 0; y < WordLength; y++) {
            if (sc[x][y] == 1) {
                count++;    //ͳ���ڴ����ֵΪ1������
            }
            if (num < MemorySize) {
                printf("%2d ", sc[x][y]);   //�ж���Ҫ������ٸ���
            }
            num += BlockSize;
        }
        printf("\n");
    }
    UsingBolckNum = count;    //����ʹ�õ�����鸳ֵ
    AvailableBlockNum = BlockNum - UsingBolckNum;    //�����õ�����鸳ֵ
    printf("ʣ���������:%d\n",AvailableBlockNum);

}
//��������allocate    ����������head����ҵ����ĵ�һ���ڵ㣩
node* allocate(node* head)
{
    //�������ܣ�������ҵ�ڵ㣬�ж��Ƿ���Է��䣬���Խ�����һ���������ڴ���䣬
    //������ҵ������ӡ����ҵ��ҳ����Ϣ����ӡϵͳ�ڴ���Ϣ
    printf("����λ��ͼ��ͼ��ʾ��\n");
    print();   

    node* work;
    work = (node*)malloc(sizeof(node));   //��ʼ��һ�����
    work->next = NULL;
    int workSize;
    printf("��������ҵ������:");
    scanf("%s",work->name);
    printf("��������ҵ��Ҫ����Ĵ�С:");
    scanf("%d",&workSize);   //���ڵ㸳��ֵ
    workSize = workSize / BlockSize;   
    if (workSize > AvailableBlockNum) {   //�ж���������ڴ��Ƿ�
        printf("����ʧ��!\n");
        return head;
    }
    int count = 0;
    for (int x = 0; x < WordNum;x++) {
        for (int y = 0; y < WordLength; y++) {
            if (count < workSize && sc[x][y] == 0) {
                work->size[count][0] = (x * WordLength) + y; //��ҳ��ֵ
                count++;
                sc[x][y] = 1;  //���ڴ渳ֵ
            }
        }
    }
    printf("����ɹ�!\n");
    printf("����λ��ͼ��ͼ��ʾ��\n");
    print();
    printf("*********��ӡ%s��ҵ��ҳ��*********\n",work->name);

    for (int i = 0; i < workSize;i++) {
        printf("\t%d\t%d\n",i,work->size[i][0]);   //��ӡҳ��
    }

    // ����������ҵ������
    if (head == NULL) {
        return work;
    }
    node* p = head;
    while (p->next!=NULL) {
        p = p->next;
    }
    p->next = work;
    return head;
}
//��������recycle        ����������head����ҵ����ĵ�һ���ڵ㣩
node* recycle(node* head)
{
    //�������ܣ��ҵ������յ���ҵ�ڵ㣬�ͷ��ڴ棬����ҵ������ɾ������ӡϵͳ�ڴ���Ϣ
    char n[10];
    printf("������Ҫ���յ���ҵ��:\n");
    scanf("%s",n);
    node* p = head;
    node* pre = NULL;
    int flag = -1;
    while (p!=NULL) {   
        if (strcmp(n, p->name) == 0) {   //�����������ҵ��ƥ����ҵ����
            flag = 1;
            break;
        }
        pre = p;
        p = p->next;
    }
    if (flag == -1) {
        printf("�޴���ҵ������ʧ��!\n");   //���û���ҵ��ͻ���ʧ��
        return head;
    }
    int count = 0;
    for (int x = 0; x < WordNum;x++) {  //�����ڴ��
        for (int y = 0; y < WordLength;y++) {
            if (p->size[count][0]!=0 && p->size[count][0] == (x * WordLength + y)) {
                // �ҵ���Ӧ��ҳ��
                sc[x][y] = 0;   //���ڴ��ж�Ӧ�ڴ�鸳ֵΪ0
                AvailableBlockNum += 1;  //�޸Ŀ�ʹ�õĿ���
                UsingBolckNum -= 1;
                count++;
            }
        }
    }
    printf("���ճɹ�!\n");
    printf("����λ��ͼ��ͼ��ʾ��\n");
    print();
    //������������ȥ��
    if (p == head) {
        return head->next;
    }
    pre->next = p->next;
    return head;


}
//��������initMemory       ������������
void initMemory() {
    //�������ܣ������ڴ��С���ֳ���������С����ʼ��ϵͳ�ڴ棬��ӡϵͳ�ڴ���Ϣ
    printf("������ϵͳ�ڴ�ռ��С:");
    scanf("%d", &MemorySize);   //��ʼ��ϵͳ�ڴ�ռ�
    printf("�������ֳ�(16/32/64):");
    scanf("%d", &WordLength);   //��ʼ���ֳ�

    printf("������������С:");
    scanf("%d", &BlockSize);   //��ʼ��������С
    if (MemorySize % (WordLength * BlockSize) == 0) {
        WordNum = MemorySize / (WordLength * BlockSize); //����ҳ��
    }
    else {
        WordNum = MemorySize / (WordLength * BlockSize) + 1;
    }
    BlockNum = MemorySize / BlockSize;  //�������
    int count = 0;
    for (int x = 0; x < WordNum; x++) {
        for (int y = 0; y < WordLength; y++) {
            if (count < MemorySize) {
                sc[x][y] = rand() % 2;   //��ʼ������λ��ͼ
            }
            count++;
        }
    }
    print();   //��ӡϵͳ�ڴ���Ϣ


}
//��������Apply          ������������
void Apply()
{
    initMemory();   //��ʼ���ڴ�
    while (1)
    {
        int c;
        c = OperationChoice();
        switch (c)
        {
        case 1:
            h=allocate(h);   //����
            break;
        case 2:
            h=recycle(h);    //����
            break;
        case 0:
            return;
        }
    }
}

int main()
{
    Apply();
    return 0;
}





/*
����������

1000
64
1
1
job2
50
1
job1
30
2
job2
2
1
2
job1
0

*/


