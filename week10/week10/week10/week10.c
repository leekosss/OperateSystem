#include<stdio.h>
#include<malloc.h>
#include<string.h>


//�ڴ�ṹ��
typedef struct memory_node
{
    int size; //�ڴ��С
    int address; //�ڴ�ʼַ
} memoryNode;

memoryNode memory;

//�����ṹ��
typedef struct link_node
{
    int id;//������
    int size; //��������
    int address; //����ʼַ
    char flag[10]; //����״̬�����л���ռ����ҵ��
    struct link_node* next;
} node;

node* head = NULL;  // �ѷ��������
node* notAllocate = NULL; //δ���������

//�α�
typedef struct segment_node
{
    char name[10];  //��ҵ������
    int a[10][10]; //�洢�α���Ϣ
    int num; //�α��жε�����
    struct segment_node* next;
} segmentNode;
segmentNode* segment = NULL; //��ҵ�α�����


//��������
void firstAllocation(int size, char c[10], int segmentNum);
void reorder();
void print();



//������ :initMemory  ����:��
node* initMemory()
{
    //�������ܣ���ʼ���ڴ�ռ�
    printf("�������ڴ��С:");
    scanf("%d",&memory.size);
    printf("��������ʼ��ַ:");
    scanf("%d",&memory.address);
    notAllocate = (node*)malloc(sizeof(node));
    notAllocate->id = 1;
    notAllocate->size = memory.size;
    notAllocate->address = memory.address;
    strcpy(notAllocate->flag, "����");
    notAllocate->next = NULL;

    return NULL;

}

//������:operation ����:node* head
int operation()
{
    //�������ܣ���ӡ�����˵���ѡ����Ҫ���еĲ���,����1�����ڴ����,����2�����ڴ�ȥ��,����0�˳�
    printf("*********�ɱ��������*********\n");
    printf("   *     1.�ڴ����      *\n");
    printf("   *     2.�ڴ�ȥ��      *\n");
    printf("   *     0.�˳�          *\n");
    printf("������ѡ��:");
    int choice;
    scanf("%d",&choice);
    return choice;

}
//������:allocate ����:node* head
void allocate()
{
    //��������������ҵ���ʹ�С��Ĭ�ϲ������ȷ���
    printf("1.�ڴ����\n");
    print();
    printf("��������ҵ��:");
    char name[10];
    int memorySize,segmentNum;
    scanf("%s",name);
    printf("������%s��Ҫ����������С:",name);
    scanf("%d",&memorySize);
    printf("������Ҫ�ֳɼ���:");
    scanf("%d",&segmentNum);
    if (memorySize > notAllocate->size) {  //����ڴ治�����������
        printf("����ʧ��!");

    }
    firstAllocation(memorySize,name,segmentNum);


}

//������:firstAllocation ����:node* head,int size,char c[10],int segmentNum
void firstAllocation(int size, char c[10],int segmentNum)
{
    //�������ܣ�����ҵ���зֶΣ����������ȷ��䣬�ֶν������ӡ�α�
    int memorySize = size;
    int segSize;

    segmentNode* segNode;
    segNode = (segmentNode*)malloc(sizeof(segmentNode));  //�½�һ���α���
    strcpy(segNode->name, c);  //����ҵ������α���
    segNode->next = NULL;
    if (segment == NULL) {
        segment = segNode;
    }
    else {
        segmentNode* s = segment;
        while (s->next != NULL) {
            s = s->next;
        }
        s->next = segNode;  // �ڶα������������½��
    }

    for (int i = 1; i <= segmentNum; i++) {

        printf("ʣ��%d���ڴ�,�������%d�εĴ�С:",memorySize,i);
        scanf("%d",&segSize);
        while (segSize > memorySize) {
            printf("�����������%d�εĴ�С:",i);
            scanf("%d", &segSize);
        }

        memorySize -= segSize; //����κ�͸���ʣ���ڴ�


        node* allocateSeg;
        allocateSeg = (node*)malloc(sizeof(node));  //��ҵÿһ�εĽ��
        allocateSeg->next = NULL;
        

        //���ѷ�����в����½ڵ�
        if (head == NULL) {
            head = allocateSeg;
        }
        else {
            node* p = head;
            while (p->next != NULL) {
                p = p->next;
            }
            p->next = allocateSeg; // ��β������
        }

        // �޸��ѷ����
        // sprintf()�������Ը�ʽ������
        sprintf(allocateSeg->flag, "%s%d", c, (i - 1));   //����Ӧ�ε�״̬�޸ģ���������ִ�����ҵ�ĵڼ���
        
        allocateSeg->size = segSize;
        allocateSeg->address = notAllocate->address;
        allocateSeg->next = NULL;

        // �޸�δ�����
        notAllocate->address += segSize;
        notAllocate->size -= segSize;

        //�޸��ѷ��䡢δ��������ķ����
        reorder();

        //�޸Ķα�
        segNode->a[i - 1][0] = i-1;   //�޸Ķ̺�
        segNode->a[i - 1][1] = segSize; //�޸Ķγ�
        segNode->a[i - 1][2] = notAllocate->address;  //�޸Ļ�ַ
        
    }
    segNode->num = segmentNum;
    printf("����ɹ�!\n");
    
    // ��ӡ�α�
    printf("********************��ӡ%s�α�********************\n",c);
    printf("�κ�\t�γ�\t��ַ\n");
    for (int i = 0; i < segmentNum; i++) {
        printf("%d\t%d\t%d\n",segNode->a[i][0],segNode->a[i][1],segNode->a[i][2]);
    }


}

//������:reorder ����:node* head
void reorder()
{
    //�������ܣ��Է�����δ�����Ĵ洢������б��  
    int count = 1;
    node* p = head;
    // �ȶ��ѷ������б��
    while (p != NULL) {
        p->id = count;
        p = p->next;
        count++;
    }
    node* q = notAllocate;
    while (q != NULL) {
        q->id = count;
        q = q->next;
        count++;
    }

}

//������:recycle 
void recyle() //�����㷨
{
    //�������ܣ��Թ黹������������д��������������ڣ������������ڣ������������ڣ�������������
    printf("2.�ڴ�ȥ��\n");
    print();
    printf("������������յ���ҵ��:");
    char name[10];
    scanf("%s",name);

    segmentNode* p = segment;  
    int flag = -1;
    while (p != NULL) {
        if (strcmp(p->name, name) == 0) {
            flag = 1; //pָ���ҵ���Ӧ��ҵ����Ӧ�Ķα�
            break;
        }
        p = p->next;  
    }
    if (flag == -1) {
        printf("����%sʧ��,�޴���ҵ!\n",name);
        return;
    }

    node* h = head,* hpre = NULL; //�ֱ�ָ�������㡢��ǰ�����
    int count = 0,address;  //count����ǰ���ڲ�ѯ�α��еڼ����κţ�address����öζ�Ӧ����ʼ��ַ
     
    while (h != NULL && count < p->num) {  //���������
        address = p->a[count][2] - p->a[count][1];  //���ĳ�ζ�Ӧ��ʼ��ַ
        if (h->address != address) {
            hpre = h;
            h = h->next;
        }
        else {  //�ҵ�����ҵ�ĸö�
            node* pos; //ָ����Ҫɾ���Ľ��
            //ɾ���ѷ�����еĽ��
            if (h == head) { //���Ҫȥ������ѷ����ĵ�1�����
                pos = head;
                head = head->next;
                h = head;
                hpre = NULL;
                
            }
            else {
                pos = h;
                hpre->next = h->next;
                h = h->next;
            }

            //��δ���������ӽ�㣬��Ҫ��4�����
            node * not = notAllocate,*notPre = NULL; //�ֱ�ָ��δ������㡢����ǰ�����
            while (not!= NULL) {  //����δ�����
                
                if (notPre == NULL &&(pos->address + pos->size) < not->address) {  //����������
                    pos->next = notAllocate;  //�嵽��ǰ��
                    notAllocate = pos;
                    strcpy(pos->flag, "����");
                    break;
                }
                if ((not->address + not->size) == pos->address) {  //����
                    not->size += pos->size;
                    break;
                }
                if (notPre == NULL && (pos->address + pos->size) > not->address) {
                    notPre = not;
                    not= not->next;
                    continue;
                }
                if (notPre != NULL && (notPre->address + notPre->size) < pos->address && (pos->address + pos->size) < not->address) {  //����������
                    pos->next = not;
                    notPre->next = pos;
                    strcpy(pos->flag, "����");
                    break;
                }
                if ((pos->address + pos->size) == not->address && (notPre->address + notPre->size) == pos->address) { //�������ںϲ�
                    notPre->size += (pos->size + not->size);
                    notPre->next = not->next;
                    break;
                }
                else if ((pos->address + pos->size) == not->address) {  //����
                    not->address -= pos->address;
                    not->size += pos->size;
                    break;
                }
                //else if ((not->address + not->size) == pos->address) {  //����
                //    not->size += pos->size;
                //    break;
                //}
                
                notPre = not;
                not= not->next;
            }
            printf("����%s�Ķ�%s%d�ɹ�!\n",p->name,p->name,count);

            count++;

        }


        
    }
    
    reorder();
    print();


}
//������:print����:node* head
void print()
{
    //�������ܣ���ӡ��������
    printf("********************����������********************\n");
    printf("�ѷ���:\n");
    printf("�����\t��С(KB)\t��ʼ(KB)\t״̬\n");
    node* p = head;
    while (p != NULL) {
        printf("%d\t%d\t\t%d\t\t%s\n",p->id,p->size,p->address,p->flag);
        p = p->next;
    }
    printf("\nδ����:\n");
    printf("�����\t��С(KB)\t��ʼ(KB)\t״̬\n");
    node* q = notAllocate;
    while (q != NULL) {
        printf("%d\t%d\t\t%d\t\t%s\n", q->id, q->size, q->address, q->flag);
        q = q->next;
    }
}

int main()
{
    
    head = initMemory();
    while (1)
    {
        int c;
        c = operation();
        if (c == 1) {
            allocate();

        }
        else if (c == 2) {
            recyle();
        }else 
        {
            break;
        }
    }
    return 0;
}

/*

����������

256
40
1
jobA
50
2
20
30
1
jobB
100
3
30
35
35
2
jobB
2
jobA
0

*/



