#include<stdio.h>
#include<math.h>

#define N 1000

int length;     //�������еĳ���
int sequence[N];//���������˳�򣨴�1��ʼ��
int result[N];//���������ȵĽ��
int now;      //��ǰ���� 


//��������in��������
void in() {
    //��������:�������������Ϣ
    printf("������������еĳ���:");
    scanf("%d",&length);
    printf("��������ʵ�����˳��:");

    for (int i = 1; i <= length ; i++) {  //��1��ʼ
        scanf("%d",&sequence[i]);
    }
    printf("���������ڷ��ʵ�����:");
    scanf("%d",&now);


}
//��������FCFS��������
void FCFS() {
    //��������:���������ȷ����㷨
    printf("***************FCFS�����Ʊ۵��ȹ���***************\n");
    in();
    printf("FCFS�ƶ���˳��Ϊ:\n");
    printf("%d ",now);
    for (int i = 1; i <= length; i++) {
        printf("%d ",sequence[i]);
    }
    int num = 0;
    num += abs(now - sequence[1]);
    for (int i = 2; i <= length; i++) {
        num += abs(sequence[i] - sequence[i - 1]);
    }
    printf("\n�ƶ�����Ϊ:%d\n",num);
    
}
//��������SSTF��������
void SSTF() {
    //��������:���ö̽������ȵ����㷨
    printf("***************SSTF�����Ʊ۵��ȹ���***************\n");
    in();
    for (int i = 1; i <= length; i++) {  //�ȸ�������鸳��ֵ
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
    printf("������ȵ�˳��Ϊ:\n");
    for (int i = 0; i <= length; i++) {
        printf("%d ",result[i]);
        if (i != 0) {
            num += abs(result[i] - result[i - 1]);
        }
    }
    printf("\n�ƶ�����Ϊ:%d\n",num);

}
//��������Elevator��������
void Elevator() {
    //��������:���õ��ݵ����㷨
    printf("***************���ݵ��ȹ���***************\n");
    in();
    int min = sequence[1], minPos, flag, temp;
    for (int i = 1; i < length; i++) {   //���Ƚ�ֵ������������
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


    printf("���ݵ���(��������)�ƶ���˳��Ϊ\n");
    int num = 0;
    flag = 0;//flag���λ��
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
    printf("\n�ƶ�����Ϊ:%d\n", num);


    now = sequence[0];

    printf("���ݵ���(��������)�ƶ���˳��Ϊ\n");
    num = 0;
    flag = 0;//flag���λ��
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
    printf("\n�ƶ�����Ϊ:%d\n", num);


}
//��������Oneway_scan��������
void Oneway_scan() {
    //��������:���õ���ɨ���㷨
    printf("***************����ɨ�����***************\n");
    in();
    int min = sequence[1] , minPos , flag, temp;
    for (int i = 1; i < length; i++) {   //���Ƚ�ֵ������������
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

    
    printf("����ɨ���ƶ���˳��Ϊ\n");
    int num = 0;  
    flag = 0;//flag���λ��
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
        if (i == length) {  //����������һ����������Ҫ�ƶ������
            num += 199 - sequence[i];
        }
        
    }
    num += 199;   //���泤��
    now = 0;
    printf("%d %d ",199,0);
    for (int i = 1; i <= flag; i++) {
        printf("%d ",sequence[i]);
        num += sequence[i] - now;
        now = sequence[i];
    }
    printf("\n�ƶ�����Ϊ:%d\n",num);



}
//��������Twoway_scan��������
void Twoway_scan() {
    //��������:����˫��ɨ���㷨
    printf("***************˫��ɨ�����***************\n");
    in();
    int min = sequence[1], minPos, flag, temp;
    for (int i = 1; i < length; i++) {   //���Ƚ�ֵ������������
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

    printf("˫��ɨ���ƶ���˳��Ϊ\n");
    int num = 0;
    flag = 0;//flag���λ��
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
        if (i == length) {  //����������һ����������Ҫ�ƶ������
            num += 199 - sequence[i];
        }

    }

    //num += 199;   //���泤��
    now = 199;
    printf("%d ",now);
    for (int i = flag; i >= 1; i--) {
        printf("%d ", sequence[i]);
        num += abs(sequence[i] - now);
        now = sequence[i];
    }
    printf("\n�ƶ�����Ϊ:%d\n", num);



}
//��������Out��������
void Out() {
    //��������:���ѡ��˵�
    printf("***************������������***************\n");
    printf("     *         1.FCFS          *\n");
    printf("     *         2.SSTF          *\n");
    printf("     *         3.����ɨ��      *\n");
    printf("     *         4.˫��ɨ��      *\n");
    printf("     *         5.���ݵ���      *\n");
    printf("     *         0.�˳�          *\n");
    printf("               ������ѡ��[ ]\b\b");


}
int main()
{
    while (1)
    {
        Out();  //����ѡ��
        int c;
        scanf("%d", &c);
        switch (c) {  //ѡ���㷨
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
            printf("\n���Ƚ�����");
            exit(1);
        }
    }
    return 0;
}

