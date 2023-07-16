#include<stdio.h>
#include<malloc.h>

//ʱ��ṹ��
typedef struct data {
	int hour;
	int min;
}Time;
typedef struct link_node {
	char name[20];//��ҵ����
	Time arrive;//��ҵ�뾮ʱ��
	int zx;//��ҵִ��ʱ��
	Time JobTime;//��ҵ����ʱ��
	int JobWait;//��ҵ���ȵȴ�ʱ��
	Time ProcessTime;//���̵���ʱ��
	int ProcessWait;//���̵��ȵȴ�ʱ��
	Time finish;//��ҵ���ʱ��
	int zz;//��ҵ��תʱ��=��ҵ���ʱ��-��ҵ�뾮ʱ��
	float zzxs;//��ҵ��Ȩ��תʱ��=��ҵ��תʱ��/��ҵִ��ʱ��
	struct link_node* next;//���ָ��
	int finished;//�����ҵ���
	float response;//��ҵ��Ӧ��
}node;
typedef struct {
	node* front; //����ָ��
	node* rear; //��βָ��
}Queue;  //���нṹ��


//��������init   ��������
Queue* init() {
	//���ܣ���ʼ�����У����ض���ָ��
	Queue* cc = (Queue*)malloc(sizeof(Queue));
	cc->front = NULL;
	cc->rear = NULL;
	return cc;
}
//��������insert  ������Queue *cc, node *x
Queue* insert(Queue* cc, node* x) {
	//���ܣ������x���յ���ʱ���Ⱥ���������cc
	if (cc->front == NULL && cc->rear == NULL) {
		cc->front = cc->rear = x;
		return cc;
	}
	if (x->arrive.hour < cc->front->arrive.hour || (x->arrive.hour == cc->front->arrive.hour && x->arrive.min <= cc->front->arrive.min)) {
		x->next = cc->front;
		cc->front = x;
		return cc;
	}
	node* p = cc->front;
	node* pre = NULL;
	while (p != NULL&&((x->arrive.hour > p->arrive.hour) || (x->arrive.hour == p->arrive.hour && x->arrive.min > p->arrive.min))) {
		pre = p;
		p = p->next;
	}
	x->next = p;
	pre->next = x;
	if (x->next == NULL) {
		cc->rear = x;
	}
	return cc;

}
//��������dele  ������Queue *cc
Queue* dele(Queue* cc) {
	//���ܣ�������
	node* p = cc->front;
	while (p != NULL) {
		node* temp = p;
		p = p->next;
		free(temp);
	}
	cc->front = NULL;
	cc->rear = NULL;
	return cc;

}
//��������input   ������Queue *cc
Queue* input(Queue* cc) {
	//���ܣ�ʵ����ҵ���е�����
	printf("����������Ҫ��������ҵ��:");
	int num;
	scanf("%d",&num);
	printf("������%d����ҵ��:\n",num);
	printf("��ҵ��	�뾮ʱ�� ����ʱ��(����):\n");
	node* newNode;
	for (int i = 0; i < num; i++) {
		newNode = (node*)malloc(sizeof(node));
		newNode->next = NULL;
		scanf("%s %d:%d %d",newNode->name, &newNode->arrive.hour, &newNode->arrive.min, &newNode->zx);
		insert(cc, newNode);
	}
	return cc;

}


/*
	�������ʱ��
*/
void computer(Queue* cc) {
	node* p, * pre;
	p = cc->front;
	pre = NULL;
	while (p != NULL) {
		if (p == cc->front) {  //�׽ڵ�
			p->JobTime.hour = p->arrive.hour;
			p->JobTime.min = p->arrive.min;
			p->JobWait = 0;

		}
		else {
			p->JobTime.hour = pre->finish.hour;
			p->JobTime.min = pre->finish.min;
			p->JobWait = p->JobTime.hour * 60 + p->JobTime.min - p->arrive.hour * 60 - p->arrive.min;
		}
		p->ProcessTime.hour = p->JobTime.hour;
		p->ProcessTime.min = p->JobTime.min;
		p->ProcessWait = 0;
		p->finish.hour = ((p->ProcessTime.hour) * 60 + p->ProcessTime.min + p->zx) / 60;
		p->finish.min = ((p->ProcessTime.hour) * 60 + p->ProcessTime.min + p->zx) % 60;
		p->zz = p->finish.hour * 60 + p->finish.min - p->arrive.hour * 60 - p->arrive.min;
		p->zzxs = 1.0 * p->zz / p->zx;

		pre = p;
		p = p->next;
	}
}

//��������FCFS   ������Queue *cc
void FCFS(Queue* cc) {
	//���ܣ�ʵ�������ȷ����㷨
	computer(cc);
}



int timeCompare(Time t1, Time t2) {
	if (t1.hour < t2.hour) {
		return 1;
	}
	if (t1.hour > t2.hour) {
		return 0;
	}
	if (t1.min <= t2.min) {
		return 1;
	}
	else {
		return 0;
	}
}

int zxCompare(int t1, int t2) {
	if (t1 <= t2) {
		return 1;
	}
	else {
		return 0;
	}
}

Time computerTime(Time arrive, int zx) {
	int total = arrive.hour * 60 + arrive.min + zx;
	Time t;
	t.hour = total / 60;
	t.min = total % 60;
	return t;
}


//��������SJF   ������Queue *cc
void SJF(Queue* cc) {
	//���ܣ�ʵ�ֶ���ҵ���ȵ����㷨
	node* p = cc->front;
	p->finish.hour = ((p->arrive.hour) * 60 + p->arrive.min + p->zx) / 60;
	p->finish.min = ((p->arrive.hour) * 60 + p->arrive.min + p->zx) % 60;

	while (p != NULL) {
		node* tmp = p->next;
		node* pos = NULL;
		while (tmp != NULL && timeCompare(tmp->arrive, p->finish)) {
			pos = tmp; // �ҵ�ִ��ʱ����������һ������
			tmp = tmp->next;
		}
		node* qre, *q, *tmr;
		qre = p;
		q = p->next;
		tmr = p;
		tmp = p->next;

		while (pos != NULL && q != pos) {
			if (!zxCompare(q->zx, q->next->zx)) {
				tmr = q;
				tmp = q->next;
				if (q->next == pos) {
					break;
				}

			}
			
			qre = q;
			q = q->next;
			
		}
		if (tmp != NULL) {
			tmr->next = tmp->next;
			tmp->next = p->next;
			p->next = tmp;
		}

		tmp = p;
		p = p->next;
		if (p == NULL) {
			continue;
		}
		if (timeCompare(p->arrive, tmp->finish)) {
			p->finish = computerTime(tmp->finish, p->zx);
		}
		else {
			p->finish = computerTime(p->arrive, p->zx);
		}
	}
	computer(cc);  //������

}
//��������HRRF    ������Queue *cc
void HRRF(Queue* cc) {
	//���ܣ�ʵ�������Ӧ�������㷨
	node* p = cc->front;
	node *q, *qre;
	node* pos, * posPre;
	double rate; //�����Ӧ��
	while (p != NULL) {
		if (p == cc->front) {
			p->finish = computerTime(p->arrive, p->zx);
		}
		qre = p;
		q = p->next;
		posPre = p;
		pos = p->next;

		if (q != NULL) {
			rate = 1 + (1.0 * (p->finish.hour*60+p->finish.min- q->arrive.hour * 60- q->arrive.min) / q->zx);
		}
		double tmpRate;
		while (q != NULL) {
			tmpRate = 1 + (1.0 * (p->finish.hour * 60 + p->finish.min - q->arrive.hour * 60 - q->arrive.min) / q->zx);
			if(tmpRate > rate) {
				rate = tmpRate;
				pos = q;
				posPre = qre;
			}
			qre = q;
			q = q->next;
		}
		if (pos != NULL && pos != p->next) { //�������
			posPre->next = pos->next;
			pos->next = p->next;
			p->next = pos;
		}
		if (pos != NULL) {
			pos->finish.hour = (p->finish.hour * 60 + p->finish.min - pos->arrive.hour * 60 - pos->arrive.min + pos->zx) / 60;
			pos->finish.min = (p->finish.hour * 60 + p->finish.min - pos->arrive.hour * 60 - pos->arrive.min + pos->zx) % 60;

		}

		p = p->next;
	}

	computer(cc);


}
//��������output   ������Queue *cc
void output(Queue* cc) {
	//���ܣ�ʵ�ֵ��Ƚ�������
	int count=0;
	float zzxs = 0, zz = 0;
	printf("���� �뾮ʱ�� ����ʱ�� ��ҵ����ʱ�� ��ҵ���ȵȴ�ʱ�� ���̵���ʱ�� ���̵��ȵȴ�ʱ�� ���ʱ�� ��תʱ�� ��Ȩ��תϵ��:\n");
	node* p = cc->front;
	while (p != NULL) {
		printf("%s\t%d:%02d\t%d\t%d:%02d\t\t%d\t\t%d:%02d\t\t\t%d\t%d:%02d\t%d\t%0.4f\n",p->name,p->arrive.hour,p->arrive.min,p->zx,p->JobTime.hour,p->JobTime.min,p->JobWait,p->ProcessTime.hour,p->ProcessTime.min,p->ProcessWait,p->finish.hour,p->finish.min,p->zz,p->zzxs);
		zz += p->zz;
		zzxs += p->zzxs;
		p = p->next;
		count++;
	}
	zz = 1.0 * zz / count;
	zzxs = 1.0 * zzxs / count;
	printf("ϵͳƽ����תʱ��:\t\t\t\t\t\t\t\t\t\t%0.2f\n",zz);
	printf("ϵͳ��Ȩƽ����תϵ��Ϊ:\t\t\t\t\t\t\t\t\t\t\t%0.4f\n",zzxs);

}


//��������menu  ��������
void menu() {
	//���ܣ�չʾ�˵���ѡ����Ӧ�㷨
	printf("***************��ҵ����***************\n");
	printf("		*		1.FCFS		*\n");
	printf("		*		2.SJF		*\n");
	printf("		*		3.HRRF		*\n");
	printf("		*		0.�˳�		*\n");
	printf("������Ҫ���еĲ�����:");

}
int main() {
	Queue* me;
	int x, y;
	printf("���������(1:��ʼ���̵��ȣ�0:��������)��");
	scanf("%d", &x);
	while (x) {
		me = init();
		me = input(me);

		menu();
		scanf("%d", &y);
		switch (y) {
		case 1: 
			FCFS(me); 
			printf("\nģ����ҵFCFS���ȹ���������:\n");
			output(me);
			dele(me);
			break;
		case 2: 
			SJF(me); 
			printf("\nģ����ҵSJF���ȹ���������:\n");
			output(me);
			dele(me);
			break;
		case 3: 
			HRRF(me); 
			printf("\nģ����ҵHRRF���ȹ���������:\n");
			output(me);
			dele(me);
			break;
		case 0: 
			printf("����������"); 
			exit(1);
		default: 
			printf("��������Ŵ���\n"); 
			break;
		}
		
		printf("\n���������(1:��ʼ���̵��ȣ�0:��������)��");
		scanf("%d", &x);
	}
	printf("����������");
}


/*
�������ݣ�

1 4
JOB2      8:50         50
JOB1      8:00         120
JOB4      9:50         20
JOB3      9:00         10
1
4
JOB2      8:50         50
JOB1      8:00         120
JOB4      9:50         20
JOB3      9:00         10
2
4
JOB2      8:50         50
JOB1      8:00         120
JOB4      9:50         20
JOB3      9:00         10
3
*/