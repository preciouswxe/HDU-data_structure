//
// Created by 13561 on 2024/11/28.
//

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ERROR   0
#define TRUE    1
#define OK      1
#define MAXSIZE 100

typedef int Status;            //��������������
typedef  char TElemType;       //�������Ԫ��ֵ������

//����������������
typedef  struct BiTNode {

    TElemType  		data;
    struct BiTNode  *lchild, *rchild;  	    //ָ�����Һ��ӽ���ָ��

} BiTNode, *BiTree;

// ջ
typedef struct {
    BiTree data[MAXSIZE];
    int top;
}SqStack;



// ��������������ַ����У�����һ�ð���������ṹ�洢�Ķ�������ָ�����Tָ��������ĸ��ڵ�
void CreateBiTree(BiTree *T) {

    TElemType ch;
    scanf(" %c",&ch);
    printf("Read character: %c\n", ch);

    // # ����ս��
    if(ch == '#') {
        printf("#����Ϊ�ս��\n");
        *T = NULL;
    } else {

        *T = (BiTree)malloc(sizeof(BiTNode));
        if(!*T) {
            exit(0);
        }
        (*T)->data = ch;

        // ������������
        CreateBiTree(&(*T)->lchild);
        CreateBiTree(&(*T)->rchild);
    }

}

// �ݹ�������������� T ��������ʵĽ���ַ�����
Status PreOrderTraverse(BiTree T) {
    // �� - �� - ��
    if(T) {
        printf("%c ",T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
    return OK;
}

// �ݹ�������������� T ��������ʵĽ���ַ�����
Status InOrderTraverse(BiTree T) {
    // �� - �� - ��
    if(T) {
        InOrderTraverse(T->lchild);
        printf("%c ",T->data);
        InOrderTraverse(T->rchild);
    }
    return OK;
}

// �ݹ������������� T ��������ʵĽ���ַ�����
Status PostOrderTraverse(BiTree T) {
    // �� - �� - ��
    if(T) {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        printf("%c ",T->data);
    }
    return OK;
}

// ջ��������
void InitStack(SqStack *S) {
    S->top = -1;
}

// �ж��Ƿ��
int StackEmpty(SqStack S) {
    return S.top == -1;
}

// ��ջ
void Push(SqStack *S, BiTree e) {
    if (S->top == MAXSIZE - 1) {
        printf("ջ��\n");
        return;
    }
    S->top++;
    S->data[S->top] = e;
}

// ��ջ
BiTree Pop(SqStack *S) {
    if (StackEmpty(*S)) {
        printf("ջ��\n");
        return NULL;
    }
    BiTree e = S->data[S->top];
    S->top--;
    return e;
}

BiTree GetTop(SqStack S) {
    if (StackEmpty(S)) {
        printf("ջ��\n");
        return NULL;
    }
    return S.data[S.top];
}


// �ǵݹ�������������� T������ջʵ��
// Ҫ���ڱ���������������ʵĽ���ַ���ͬʱ���������ջ/��ջ�Ĺ��� �� ջ��ָ����ָ�Ľ���ַ�
Status NRPreOrderTraverse(BiTree T) {
    SqStack S;
    InitStack(&S);
    if(T!=NULL) {
        BiTree p ;
        S.data[++(S.top)] = T;
        while(S.top != -1) {
            // ���ڵ��ջ������������
            p = S.data[(S.top)--];
            printf("��ջ %c \n",p->data);
            // ������ѹ�������
            if(p->rchild != NULL) {
                S.data[++(S.top)] = p->rchild;
            }
            if(p->lchild != NULL) {
                S.data[++(S.top)] = p->lchild;
            }
        }
    }
    return OK;
}

// �ǵݹ�������������� T  �����
Status NRInOrderTraverse(BiTree T) {
    SqStack S;
    InitStack(&S);
    BiTree p = T;
    while (p ||!StackEmpty(S)) {
        while (p) {
            //printf("��� %c ׼����ջ \n", p->data);
            Push(&S, p);
            //printf("��ǰջ��: %c \n", GetTop(S)->data);
            //printf("���ڷ��ʽ�� %c \n", p->data);
            // �������
            p = p->lchild;
        }
        // ջ���վͳ�ջ
        if (!StackEmpty(S)) {
            p = Pop(&S);
            printf("��� %c ��ջ \n", p->data);
            // �����ұ�
            p = p->rchild;
        }
    }
    return OK;
}



// �ǵݹ������������� T  ���Ҹ�
Status NRPostOrderTraverse(BiTree T) {
    SqStack S;
    InitStack(&S);
    BiTree p = T;
    BiTree lastVisited = NULL;
    while (p ||!StackEmpty(S)) {
        if(p) {
            S.data[++(S.top)] = p;
            // ��������
            p = p->lchild;
        }
        else {
            p = S.data[S.top];
            // �����ų�ջ���� �����ҽڵ��Ƿ�����Լ��Ƿ񱻷��ʹ� ѹ��ջ��
            if (p->rchild != NULL && p->rchild != lastVisited) {
                p = p->rchild;
                S.data[++(S.top)] = p;
                // �鿴�ҽ�������
                p = p ->lchild;
            }
            else {
                p = S.data[(S.top)--];
                printf("��ջ %c \n",p->data);
                lastVisited = p;
                // ��һ������ջ�󣬽�������һ������ջ������pֱ���ÿ�
                p = NULL;
            }
        }
    }
    return OK;
}

// Ӧ��ʵ��1�����ض�����T�ȷֱ�Ϊ 0 , 1 , 2�Ľ����
void TNodes(BiTree T, int d, int *count) {
    if (T) {
        int degree = (T->lchild!= NULL) + (T->rchild!= NULL);
        if (degree == d) {
            (*count)++;
        }
        TNodes(T->lchild, d, count);
        TNodes(T->rchild, d, count);
    }
}

// Ӧ��ʵ��2��������� T �ĸ߶�
int High(BiTree T) {
    if (T == NULL) return 0;
    else {
        int leftHeight = High(T->lchild);
        int rightHeight = High(T->rchild);
        return (leftHeight > rightHeight)? (leftHeight + 1) : (rightHeight + 1);
    }
}

// Ӧ��ʵ��3�����ݸ������ַ��������ɶ���������
void CreateBST(BiTree *T, const char *chars) {
    *T = NULL;
    int len = strlen(chars);
    for (int i = 0; i < len; i++) {
        BiTree p = *T;
        BiTree q = NULL;
        while (p!= NULL) {
            q = p;
            if (chars[i] < p->data) {
                // С�ķ����
                p = p->lchild;
            } else {
                // ����ұ�
                p = p->rchild;
            }
        }
        BiTree newNode = (BiTree)malloc(sizeof(BiTNode));
        newNode->data = chars[i];
        newNode->lchild = newNode->rchild = NULL;
        if (q == NULL) {
            *T = newNode;
        } else if (chars[i] < q->data) {
            q->lchild = newNode;
        } else {
            q->rchild = newNode;
        }
    }
}


int main() {
    // (1) ����CreateBiTree(T)��������һ�ö�����T
    BiTree T;
    printf("�����������T������������У�#��ʾ�սڵ㣩��\n");
    CreateBiTree(&T);
    printf("��������ɹ���\n");

    printf("------------------------------\n");


    // (2) �ֱ���������������������ͺ�������ĵݹ麯�������Ӧ�ı������
    printf("�ݹ�������������\n");
    PreOrderTraverse(T);
    printf("\n");
    printf("�ݹ�������������\n");
    InOrderTraverse(T);
    printf("\n");
    printf("�ݹ������������\n");
    PostOrderTraverse(T);
    printf("\n");

    printf("------------------------------\n");

    // (3) �ֱ���������������������ͺ�������ķǵݹ麯�������Ӧ�ı��������ջԪ�صı仯����
    printf("�ǵݹ�������������ջ�仯��\n");
    NRPreOrderTraverse(T);
    printf("------------------------------\n");
    printf("�ǵݹ�������������ջ�仯��\n");
    NRInOrderTraverse(T);
    printf("------------------------------\n");
    printf("�ǵݹ������������ջ�仯��\n");
    NRPostOrderTraverse(T);
    printf("------------------------------\n");

    // (4) ����TNodes(T)���������������T�ȷֱ�Ϊ0��1��2�Ľ����
    int count0 = 0, count1 = 0, count2 = 0;
    TNodes(T, 0, &count0);
    TNodes(T, 1, &count1);
    TNodes(T, 2, &count2);
    printf("��Ϊ0�Ľڵ������%d\n", count0);
    printf("��Ϊ1�Ľڵ������%d\n", count1);
    printf("��Ϊ2�Ľڵ������%d\n", count2);

    printf("------------------------------\n");


    // (5) ����CreateBST(T1,"DBFCAEG")��CreateBST(T2,"ABCDEFG")���������ö������������ǽ������������������High()���������߶�
    BiTree T1, T2;
    CreateBST(&T1, "DBFCAEG");
    CreateBST(&T2, "ABCDEFG");
    printf("T1������������");
    InOrderTraverse(T1);
    printf("\nT1�߶ȣ�%d\n", High(T1));
    printf("T2������������");
    InOrderTraverse(T2);
    printf("\nT2�߶ȣ�%d\n", High(T2));


    return 0;
}