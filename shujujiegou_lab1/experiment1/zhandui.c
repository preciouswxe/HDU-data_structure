//
// Created by 13561 on 2024/11/7.
//
#include<stdlib.h>
#include<stdio.h>

#define STACK_INIT_SIZE    10
#define MAXQSIZE 10    //���е���󳤶�
#define STACKINCREMENT  5
#define ERROR   0
#define TRUE    1
#define OK      1



typedef int Status;         //��������������
typedef int SElemType;  //����SElemType������
typedef int QElemType;  //����SElemType������

typedef struct  {
    SElemType *base;  //˳��ջ���ݴ洢�ռ��ַ
    SElemType *top;    //˳��ջջ��ָ��
    int stacksize;
}SqStack;  // SqStackΪ�û������˳��ջ����


typedef struct {
    QElemType *base;    //��ʼ������ʱ��̬����洢�ռ�ĵ�ַ(������)
    int front;                 //ͷԪ�ص��±����
    int rear;                   //βԪ�ص��±����
} SqQueue;   // SqQueueΪ�û������ѭ����������

// ������������
Status InitStack(SqStack *S);
Status ClearStack(SqStack *S);
Status StackLength(SqStack S);
Status GetTop(SqStack S, SElemType *e);
Status Push(SqStack *S, SElemType e);
Status Pop(SqStack *S, SElemType *e);
Status InitQueue(SqQueue *Q);
Status ClearQueue(SqQueue *Q);
Status QueueLength(SqQueue Q);
Status QueueEmpty(SqQueue Q);
Status QueueFull(SqQueue Q);
Status EnQueue(SqQueue *Q, QElemType e);
Status DeQueue(SqQueue *Q, QElemType *e);
Status QueueReverse(SqQueue *Q);
Status Conversion(int m, int n);




// ��ʼջ
Status InitStack(SqStack *S) {
    // �����ʼ�����ݵĿռ� ��߱��������ַ
    S->base = (SElemType *)malloc(sizeof(SElemType) * STACK_INIT_SIZE);
    if(!S->base) {
        return ERROR;
    }
    S->top  = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return OK;
}

// ���ջ
Status ClearStack(SqStack *S) {
    S->top = S->base;
    return OK;
}

// ����
Status StackLength(SqStack S) {
    return S.top-S.base;
}

// ����ջ��Ԫ��ֵ
Status GetTop(SqStack S, SElemType *e) {
    if (S.top == S.base) {
        return ERROR;
    }
    *e = *(S.top - 1);
    return OK;
}

// ������e��ջ
Status Push(SqStack *S, SElemType e) {
    // ���ջ�Ƿ�����
    if (StackLength(*S) >= S->stacksize) {
        // realloc���·���ռ�  Ҫ����һ����ʼ����
        SElemType *newbase = (SElemType *)realloc(S->base, (S->stacksize + STACK_INIT_SIZE) * sizeof(SElemType));
        if (!newbase) {
            // ������ʧ��
            return ERROR;
        }
        // �� S.base ����Ϊ�·�����ڴ�ռ�Ļ���ַ newbase
        S->base = newbase;
        S->top = S->base + S->stacksize;
        S->stacksize += STACK_INIT_SIZE;
    }

    *S->top = e;
    S->top++;
    return OK;
}

// ��ջ��Ԫ�س�ջ e������ֵ
Status Pop(SqStack *S, SElemType *e) {
    if (S->top == S->base) {
        return ERROR;
    }

    *e = *(S->top - 1);
    S->top--;
    return OK;
}

// ��ʼ����
Status InitQueue(SqQueue *Q) {
    Q->base = (QElemType *)malloc(sizeof(QElemType) * MAXQSIZE);
    if (!Q->base ) {
        printf("��ʼ�������ڴ����ʧ��");
        return ERROR;
    }

    Q->front = 0;
    Q->rear = 0;
    return OK;
}

// ��ն���
Status ClearQueue(SqQueue *Q) {
    Q->rear = Q->front = 0;
    return OK;
}

// ����Q������Ԫ�ظ���
Status QueueLength(SqQueue Q) {
    // ע����ѭ������
    return (Q.rear >= Q.front)? (Q.rear - Q.front) : (Q.rear + MAXQSIZE - Q.front);
}

// ����e���
Status EnQueue(SqQueue *Q, QElemType e) {
    // �ж��Ƿ���������������һ���洢��Ԫ���ж������������ (Q->rear + 1) % MAXQSIZE == Q->front ʱΪ����
    if ((Q->rear + 1) % MAXQSIZE == Q->front) {
        printf("�������� �޷����\n");
        return ERROR;
    }
    /*
    if (QueueLength(*Q) >= MAXQSIZE) {
        printf("�������� �޷����\n");
        return ERROR;
    }
    */
    // ��Ԫ�طŵ���β
    Q->base[Q->rear] = e;

    printf("EnQueue: front = %d, rear = %d, MAXQSIZE = %d\n", Q->front, Q->rear, MAXQSIZE);

    Q->rear = (Q->rear + 1) % MAXQSIZE;
    return OK;
}

// ���ݳ��ӣ���e������ֵ
Status DeQueue(SqQueue *Q, QElemType *e) {

    // �ж��Ƿ�Ϊ��
    if (Q->front == Q->rear) {
        printf("����Ϊ�ա�\n");
        return ERROR;
    }

    *e = Q->base[Q->front];

    // ����ͷָ��
    Q->front = (Q->front + 1) % MAXQSIZE;

    // �����Ӻ�����Ƿ�Ϊ��
    if (Q->front == Q->rear) {
        Q->front = 0;
        Q->rear = 0;
    }

    return OK;
}

// �ж�ѭ�������Ƿ�Ϊ��
Status QueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;
}

// �ж�ѭ�������Ƿ�����
Status QueueFull(SqQueue Q) {
    // ѭ���������������ǣ�(rear + 1) % MAXQSIZE == front
    return ((Q.rear + 1) % MAXQSIZE) == Q.front;
}

// ����ת��
Status Conversion(int m, int n) {
    SqStack S;
    if (InitStack(&S)!= OK) {
        printf("ջ��ʼ��ʧ�ܣ�\n");
        return ERROR;
    }

    int re;
    while (m!= 0) {
        re = m % n;
        if (Push(&S, re)!= OK) {
            printf("��ջʧ�ܣ�\n");
            return ERROR;
        }
        m /= n;
    }

    printf("ת�����Ϊ��");

    // ��ʼ��ջ��ӡ
    SElemType e;
    while (Pop(&S, &e) == OK) {
        printf("%d", e);
    }

    printf("\n");

    // �����ͷŻ���ַ
    free(S.base);
    return OK;
}

// ����ջ�������е�Ԫ����������
Status QueueReverse(SqQueue *Q) {
    SqStack S;
    if (InitStack(&S)!= OK) {
        printf("ջ��ʼ��ʧ�ܣ�\n");
        return ERROR;
    }

    QElemType e;
    // �Ƚ������е�Ԫ�����γ��Ӳ�����ջ��
    printf("������ջ: \n");
    while (DeQueue(Q, &e) == OK) {
        if (Push(&S, e)!= OK) {
            printf("��ջ����ʧ�ܣ�\n");
            return ERROR;
        }
        printf("����Ԫ�ؽ�ջ��ֱ��Ϊ�ա���\n");
    }

    printf("��ȡ�����ؽ������: \n");
    while (Pop(&S, &e) == OK) {
        if (EnQueue(Q, e)!= OK) {
            printf("��Ӳ���ʧ�ܣ�\n");
            return ERROR;
        }
    }

    free(S.base);
    return OK;
}

int main() {
    // ����ת������
    int m;
    printf("������һ��ʮ������: ");
    scanf("%d", &m);

    printf("��ʮ������ %d ת���ɶ�������: ", m);
    Conversion(m, 2);

    printf("��ʮ������ %d ת���ɰ˽�����: ", m);
    Conversion(m, 8);


    // ѭ�����в�������
    SqQueue Q;
    if (InitQueue(&Q)!= OK) {
        printf("���г�ʼ��ʧ�ܣ�\n");
        return ERROR;
    }
    printf("��ʼ���������\n");

    // <1>�������9����������ʾ����Q��front��rear��ֵ
    QElemType num;
    for (int i = 1; i <= 9; i++) {
        num = i;
        if (EnQueue(&Q, num)!= OK) {
            printf("��Ӳ���ʧ�ܣ�\n");
            return ERROR;
        }
    }
    printf("<1>�������9�� ���\n");

    printf("�������9�������󣬶���Q��frontֵΪ: %d��rearֵΪ: %d\n", Q.front, Q.rear);

    // <2>�������1��������չʾ��ʱ���е�״̬��Ϣ
    num = 10;
    if (EnQueue(&Q, num) == OK) {
        printf("�����1�������ɹ�����ʱ����״̬: ���г���Ϊ %d��frontֵΪ %d��rearֵΪ %d\n",
            QueueLength(Q), Q.front, Q.rear);
    } else {
        printf("�����1������ʧ�ܣ���ʱ����������\n");
    }

    printf("-------\n");
    printf("���벽��<3>\n");
    // <3>��������5�������������2����������ʾ��ʱ�Ķ���Q�ĳ�����front��rear��ֵ
    for (int i = 0; i < 5; i++) {
        if (DeQueue(&Q, &num)!= OK) {
            printf("���Ӳ���ʧ�ܣ�\n");
            return -1;
        }
    }
    printf("����5��������\n");
    for (int i = 0; i < 2; i++) {
        num = 11 + i;
        if (EnQueue(&Q, num)!= OK) {
            printf("��Ӳ���ʧ�ܣ�\n");
            return -1;
        }
    }
    printf("��������5�������������2����������Q�ĳ���Ϊ: %d��frontֵΪ: %d��rearֵΪ: %d\n",
        QueueLength(Q), Q.front, Q.rear);

    printf("-------\n");
    printf("���벽��<4>\n");
    // <4>���ú���QueueReverse(Q)��������Ԫ���������ţ�Ȼ�󽫶���Ԫ�س��Ӳ����
    if (QueueReverse(&Q)!= OK) {
        printf("�����������ʧ�ܣ�\n");
        return -1;
    }
    printf("��ӡ������������\n");
    while (!QueueEmpty(Q)) {
        if (DeQueue(&Q, &num)!= OK) {
            printf("���Ӳ���ʧ�ܣ�\n");
            return -1;
        }
        printf("%d ", num);
    }
    printf("\n");
    printf("������ϡ�\n");

    return 0;
}