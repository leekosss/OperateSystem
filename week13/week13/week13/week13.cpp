#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include<string.h>


typedef struct node
{
    int size;//��Ҫ����ռ�Ĵ�С
    int a[1000];//���
    int zihao[1000];//�ֺ�
    int weihao[1000];//λ��
    int zhu[1000];//�����
    int citou[1000];//��ͷ��
    int shanqu[1000];//������
    char name[10];//��ҵ��
    struct node* next;
}linklist;


linklist* work = NULL;


int disksize;//Ӳ�̴�С
int word;//�ֳ�
int blocksize;//������С
int head;//��ͷ��
int sector;//������
int cylinder;//ÿ�������ϵ�������
int sum;//��������
int available; //����ʣ����п���
int map[4096][4096];  //����λ��ͼ
int line;  //λʽͼ����

int currentZhu;   //��ǰ�����
int currentCitou; //��ǰ��ͷ��
int currentShanqu;//��ǰ������


//��������print    ��������
void print()
{
    //�������ܣ��������λʾͼ
    available = 0;
    printf("          ********************����ʹ�����********************\n");
    printf("   ");
    for (int i = 0; i < word; i++) {
        printf("%-2d ", i);
    }
    printf("\n");

    for (int i = 0; i < line; i++) {
        printf("%-2d ", i);
        for (int j = 0; j < word; j++) {
            if (i * word + j >= disksize) {
                break;
            }
            if (map[i][j] == 0) {
                available++;
            }
            printf("%-2d ", map[i][j]);
        }
        printf("\n");
    }
    printf("����ʣ����п�����%d\n", available);

}
//��������allocate  ��������
void allocate()
{
    //�������ܣ�Ϊ��ҵ����ռ䣬 �ֱ��С���š��顢�֡�λ������š���	ͷ�š������ţ������
    linklist* node = (linklist*)malloc(sizeof(linklist));
    node->next = NULL;
    printf("��������Ҫ����ռ����ҵ���ֺ���Ҫ���丨��ռ�Ĵ�С����λ��K����");
    scanf("%s",node->name);
    scanf("%d",&node->size);
    if (node->size > available) {
        printf("��������ʧ��!\n");
        return;
    }
    int num = 0;
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < word; j++) {
            if (i * word + j >= disksize) {
                break;
            }
            if (map[i][j] == 0 && num < node->size) {
                int flag = i * word + j;
                node->a[num] = flag;
                node->zihao[num] = i;
                node->weihao[num] = j;

                node->zhu[num] = currentZhu;
                if (flag / sector != (flag - 1) / sector) {
                    currentCitou++;
                }
                node->citou[num] = currentCitou;
                node->shanqu[num] = flag % sector;
                num++;
                map[i][j] = 1;
            }
        }
    }
    if (work == NULL) {
        work = node;
    }
    else {
        linklist* p = work;
        while (p->next != NULL) {
            p = p->next;
        }
        p->next = node;
    }
    


    printf("��������ɹ�!\n");
    print();
    printf("����ʣ����п���:%d\n",available);
    printf("          ********************%s������ĸ���********************\n",node->name);
    printf("���\t��\t��\tλ\t�����\t��ͷ��\t������\n");
    for (int i = 0; i < node->size; i++) {
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",i+1,node->a[i],node->zihao[i],node->weihao[i],node->zhu[i],node->citou[i],node->shanqu[i]);
    }

}
//��������recycle����:��
void recycle()
{
    //�������ܣ�������ҵ�Ĵ洢�ռ䣬�����λʾͼ
    linklist* p = work;
    if (p == NULL) {
        printf("��ǰδ������ҵ!\n");
        return;
    }
    printf("��ǰ�������ҵ��");
    while (p != NULL) {
        printf("%s",p->name);
        if (p->next != NULL) {
            printf("->");
        }
        p = p->next;
    }
    printf("\nҪ���յ���ҵ����");
    char name[20];
    scanf("%s",name);
    p = work;
    while (p!=NULL && strcmp(p->name,name)!=0) {
        p = p->next;
    }
    if (p == NULL) {
        printf("����ʧ��!\n");
        return;
    }
    for (int i = 0; i < p->size; i++) {
        map[p->zihao[i]][p->weihao[i]] = 0;
    }
    if (p == work) {
        work = work->next;
    }
    else {
        linklist* pre = work;
        while (pre->next!=p) {
            pre = pre->next;
        }
        pre->next = p->next;
    }

    printf("���ճɹ�!\n");
    print();


}
//��������init ����:��
void init()
{
    //�������ܣ������ʼλʾͼ�������λʾͼ
    for (int i = 0; i < line; i++) {
        for (int j = 0; j < word; j++) {
            if (i * word + j >= disksize) {
                break;
            }
            map[i][j] = rand() % 2;
        }
    }
    print();



}
//��������menu  ����:��
int menu()
{
    //�������ܣ�����˵�
    printf("***************������ȹ���***************\n");
    printf("       *       1.�ռ����         *\n");
    printf("       *       2.�ռ�ȥ��         *\n");
    printf("       *       0.�˳�             *\n");
    printf("               ������ѡ��[ ]\b\b");
    int choice;
    scanf("%d",&choice);
    if (choice == 1) {
        return 1;
    }
    else if (choice == 2) {
        return 2;
    }
    else if (choice == 0) {
        return 0;
    }

}

int main()
{
    printf("�����븨��ռ�Ĵ�С����λ��K�����ֳ���32 or 64���Ϳ鳤����λ��K����");
    scanf("%d %d %d", &disksize, &word, &blocksize);
    printf("������ø���Ӳ�̵Ĵŵ�������ͷ������ÿ���ŵ�����������");
    scanf("%d %d", &head, &sector);
    sum = disksize / blocksize;  //��������
    line = sum / word + (sum % word ? 1 : 0);   //����
    cylinder = head * sector;  //ÿ�������ϵ�������

    init();

    while (1)
    {
        int c;
        c = menu();
        if (c == 0)
        {
            break;
        }
        else if (c == 1) {
            allocate();
        }
        else if (c == 2) {
            recycle();
        }
    }
    return 0;
}




