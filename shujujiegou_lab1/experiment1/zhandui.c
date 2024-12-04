//
// Created by 13561 on 2024/11/7.
//
#include<stdlib.h>
#include<stdio.h>

#define STACK_INIT_SIZE    10
#define MAXQSIZE 10    //队列的最大长度
#define STACKINCREMENT  5
#define ERROR   0
#define TRUE    1
#define OK      1



typedef int Status;         //声明函数类型名
typedef int SElemType;  //声明SElemType的类型
typedef int QElemType;  //声明SElemType的类型

typedef struct  {
    SElemType *base;  //顺序栈数据存储空间基址
    SElemType *top;    //顺序栈栈顶指针
    int stacksize;
}SqStack;  // SqStack为用户定义的顺序栈类型


typedef struct {
    QElemType *base;    //初始化队列时动态分配存储空间的地址(数组名)
    int front;                 //头元素的下标变量
    int rear;                   //尾元素的下标变量
} SqQueue;   // SqQueue为用户定义的循环队列类型

// 函数声明部分
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




// 初始栈
Status InitStack(SqStack *S) {
    // 分配初始的数据的空间 左边被分配基地址
    S->base = (SElemType *)malloc(sizeof(SElemType) * STACK_INIT_SIZE);
    if(!S->base) {
        return ERROR;
    }
    S->top  = S->base;
    S->stacksize = STACK_INIT_SIZE;
    return OK;
}

// 清空栈
Status ClearStack(SqStack *S) {
    S->top = S->base;
    return OK;
}

// 长度
Status StackLength(SqStack S) {
    return S.top-S.base;
}

// 返回栈顶元素值
Status GetTop(SqStack S, SElemType *e) {
    if (S.top == S.base) {
        return ERROR;
    }
    *e = *(S.top - 1);
    return OK;
}

// 将数据e进栈
Status Push(SqStack *S, SElemType e) {
    // 检查栈是否已满
    if (StackLength(*S) >= S->stacksize) {
        // realloc重新分配空间  要增加一个初始长度
        SElemType *newbase = (SElemType *)realloc(S->base, (S->stacksize + STACK_INIT_SIZE) * sizeof(SElemType));
        if (!newbase) {
            // 若分配失败
            return ERROR;
        }
        // 将 S.base 更新为新分配的内存空间的基地址 newbase
        S->base = newbase;
        S->top = S->base + S->stacksize;
        S->stacksize += STACK_INIT_SIZE;
    }

    *S->top = e;
    S->top++;
    return OK;
}

// 将栈顶元素出栈 e返回其值
Status Pop(SqStack *S, SElemType *e) {
    if (S->top == S->base) {
        return ERROR;
    }

    *e = *(S->top - 1);
    S->top--;
    return OK;
}

// 初始队列
Status InitQueue(SqQueue *Q) {
    Q->base = (QElemType *)malloc(sizeof(QElemType) * MAXQSIZE);
    if (!Q->base ) {
        printf("初始化队列内存分配失败");
        return ERROR;
    }

    Q->front = 0;
    Q->rear = 0;
    return OK;
}

// 清空队列
Status ClearQueue(SqQueue *Q) {
    Q->rear = Q->front = 0;
    return OK;
}

// 返回Q中数据元素个数
Status QueueLength(SqQueue Q) {
    // 注意是循环队列
    return (Q.rear >= Q.front)? (Q.rear - Q.front) : (Q.rear + MAXQSIZE - Q.front);
}

// 数据e入队
Status EnQueue(SqQueue *Q, QElemType e) {
    // 判断是否已满（采用牺牲一个存储单元来判断满的情况，即 (Q->rear + 1) % MAXQSIZE == Q->front 时为满）
    if ((Q->rear + 1) % MAXQSIZE == Q->front) {
        printf("队列已满 无法入队\n");
        return ERROR;
    }
    /*
    if (QueueLength(*Q) >= MAXQSIZE) {
        printf("队列已满 无法入队\n");
        return ERROR;
    }
    */
    // 将元素放到队尾
    Q->base[Q->rear] = e;

    printf("EnQueue: front = %d, rear = %d, MAXQSIZE = %d\n", Q->front, Q->rear, MAXQSIZE);

    Q->rear = (Q->rear + 1) % MAXQSIZE;
    return OK;
}

// 数据出队，用e返回其值
Status DeQueue(SqQueue *Q, QElemType *e) {

    // 判断是否为空
    if (Q->front == Q->rear) {
        printf("队列为空。\n");
        return ERROR;
    }

    *e = Q->base[Q->front];

    // 更新头指针
    Q->front = (Q->front + 1) % MAXQSIZE;

    // 检查出队后队列是否为空
    if (Q->front == Q->rear) {
        Q->front = 0;
        Q->rear = 0;
    }

    return OK;
}

// 判断循环队列是否为空
Status QueueEmpty(SqQueue Q) {
    return Q.front == Q.rear;
}

// 判断循环队列是否已满
Status QueueFull(SqQueue Q) {
    // 循环队列满的条件是：(rear + 1) % MAXQSIZE == front
    return ((Q.rear + 1) % MAXQSIZE) == Q.front;
}

// 进制转换
Status Conversion(int m, int n) {
    SqStack S;
    if (InitStack(&S)!= OK) {
        printf("栈初始化失败！\n");
        return ERROR;
    }

    int re;
    while (m!= 0) {
        re = m % n;
        if (Push(&S, re)!= OK) {
            printf("入栈失败！\n");
            return ERROR;
        }
        m /= n;
    }

    printf("转换结果为：");

    // 开始出栈打印
    SElemType e;
    while (Pop(&S, &e) == OK) {
        printf("%d", e);
    }

    printf("\n");

    // 用完释放基地址
    free(S.base);
    return OK;
}

// 利用栈将队列中的元素逆序重排
Status QueueReverse(SqQueue *Q) {
    SqStack S;
    if (InitStack(&S)!= OK) {
        printf("栈初始化失败！\n");
        return ERROR;
    }

    QElemType e;
    // 先将队列中的元素依次出队并存入栈中
    printf("队列入栈: \n");
    while (DeQueue(Q, &e) == OK) {
        if (Push(&S, e)!= OK) {
            printf("进栈操作失败！\n");
            return ERROR;
        }
        printf("队列元素进栈中直至为空……\n");
    }

    printf("再取出返回进入队列: \n");
    while (Pop(&S, &e) == OK) {
        if (EnQueue(Q, e)!= OK) {
            printf("入队操作失败！\n");
            return ERROR;
        }
    }

    free(S.base);
    return OK;
}

int main() {
    // 数制转换部分
    int m;
    printf("请输入一个十进制数: ");
    scanf("%d", &m);

    printf("将十进制数 %d 转换成二进制数: ", m);
    Conversion(m, 2);

    printf("将十进制数 %d 转换成八进制数: ", m);
    Conversion(m, 8);


    // 循环队列操作部分
    SqQueue Q;
    if (InitQueue(&Q)!= OK) {
        printf("队列初始化失败！\n");
        return ERROR;
    }
    printf("初始化队列完成\n");

    // <1>连续入队9个整数后，显示队列Q的front和rear的值
    QElemType num;
    for (int i = 1; i <= 9; i++) {
        num = i;
        if (EnQueue(&Q, num)!= OK) {
            printf("入队操作失败！\n");
            return ERROR;
        }
    }
    printf("<1>连续入队9个 完成\n");

    printf("连续入队9个整数后，队列Q的front值为: %d，rear值为: %d\n", Q.front, Q.rear);

    // <2>若再入队1个整数，展示此时队列的状态信息
    num = 10;
    if (EnQueue(&Q, num) == OK) {
        printf("再入队1个整数成功，此时队列状态: 队列长度为 %d，front值为 %d，rear值为 %d\n",
            QueueLength(Q), Q.front, Q.rear);
    } else {
        printf("再入队1个整数失败，此时队列已满！\n");
    }

    printf("-------\n");
    printf("进入步骤<3>\n");
    // <3>连续出队5个整数，再入队2个整数，显示此时的队列Q的长度与front和rear的值
    for (int i = 0; i < 5; i++) {
        if (DeQueue(&Q, &num)!= OK) {
            printf("出队操作失败！\n");
            return -1;
        }
    }
    printf("出队5个整数。\n");
    for (int i = 0; i < 2; i++) {
        num = 11 + i;
        if (EnQueue(&Q, num)!= OK) {
            printf("入队操作失败！\n");
            return -1;
        }
    }
    printf("连续出队5个整数，再入队2个数，队列Q的长度为: %d，front值为: %d，rear值为: %d\n",
        QueueLength(Q), Q.front, Q.rear);

    printf("-------\n");
    printf("进入步骤<4>\n");
    // <4>调用函数QueueReverse(Q)，将队列元素逆序重排，然后将队列元素出队并输出
    if (QueueReverse(&Q)!= OK) {
        printf("队列逆序操作失败！\n");
        return -1;
    }
    printf("打印队列逆序结果：\n");
    while (!QueueEmpty(Q)) {
        if (DeQueue(&Q, &num)!= OK) {
            printf("出队操作失败！\n");
            return -1;
        }
        printf("%d ", num);
    }
    printf("\n");
    printf("程序完毕。\n");

    return 0;
}