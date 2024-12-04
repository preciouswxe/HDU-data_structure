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

typedef int Status;            //声明函数类型名
typedef  char TElemType;       //声明结点元素值的类型

//定义二叉树结点类型
typedef  struct BiTNode {

    TElemType  		data;
    struct BiTNode  *lchild, *rchild;  	    //指向左右孩子结点的指针

} BiTNode, *BiTree;

// 栈
typedef struct {
    BiTree data[MAXSIZE];
    int top;
}SqStack;



// 根据先序遍历的字符序列，创建一棵按二叉链表结构存储的二叉树，指针变量T指向二叉树的根节点
void CreateBiTree(BiTree *T) {

    TElemType ch;
    scanf(" %c",&ch);
    printf("Read character: %c\n", ch);

    // # 代表空结点
    if(ch == '#') {
        printf("#设置为空结点\n");
        *T = NULL;
    } else {

        *T = (BiTree)malloc(sizeof(BiTNode));
        if(!*T) {
            exit(0);
        }
        (*T)->data = ch;

        // 创建左、右子树
        CreateBiTree(&(*T)->lchild);
        CreateBiTree(&(*T)->rchild);
    }

}

// 递归先序遍历二叉树 T ，输出访问的结点字符序列
Status PreOrderTraverse(BiTree T) {
    // 根 - 左 - 右
    if(T) {
        printf("%c ",T->data);
        PreOrderTraverse(T->lchild);
        PreOrderTraverse(T->rchild);
    }
    return OK;
}

// 递归中序遍历二叉树 T ，输出访问的结点字符序列
Status InOrderTraverse(BiTree T) {
    // 左 - 根 - 右
    if(T) {
        InOrderTraverse(T->lchild);
        printf("%c ",T->data);
        InOrderTraverse(T->rchild);
    }
    return OK;
}

// 递归后序遍历二叉树 T ，输出访问的结点字符序列
Status PostOrderTraverse(BiTree T) {
    // 左 - 右 - 根
    if(T) {
        PostOrderTraverse(T->lchild);
        PostOrderTraverse(T->rchild);
        printf("%c ",T->data);
    }
    return OK;
}

// 栈基本操作
void InitStack(SqStack *S) {
    S->top = -1;
}

// 判断是否空
int StackEmpty(SqStack S) {
    return S.top == -1;
}

// 进栈
void Push(SqStack *S, BiTree e) {
    if (S->top == MAXSIZE - 1) {
        printf("栈满\n");
        return;
    }
    S->top++;
    S->data[S->top] = e;
}

// 出栈
BiTree Pop(SqStack *S) {
    if (StackEmpty(*S)) {
        printf("栈空\n");
        return NULL;
    }
    BiTree e = S->data[S->top];
    S->top--;
    return e;
}

BiTree GetTop(SqStack S) {
    if (StackEmpty(S)) {
        printf("栈空\n");
        return NULL;
    }
    return S.data[S.top];
}


// 非递归先序遍历二叉树 T，依靠栈实现
// 要求在遍历过程中输出访问的结点字符的同时，输出结点进栈/出栈的过程 和 栈中指针所指的结点字符
Status NRPreOrderTraverse(BiTree T) {
    SqStack S;
    InitStack(&S);
    if(T!=NULL) {
        BiTree p ;
        S.data[++(S.top)] = T;
        while(S.top != -1) {
            // 根节点出栈再找它的子树
            p = S.data[(S.top)--];
            printf("出栈 %c \n",p->data);
            // 右子树压在最底下
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

// 非递归中序遍历二叉树 T  左根右
Status NRInOrderTraverse(BiTree T) {
    SqStack S;
    InitStack(&S);
    BiTree p = T;
    while (p ||!StackEmpty(S)) {
        while (p) {
            //printf("结点 %c 准备进栈 \n", p->data);
            Push(&S, p);
            //printf("当前栈顶: %c \n", GetTop(S)->data);
            //printf("正在访问结点 %c \n", p->data);
            // 访问左边
            p = p->lchild;
        }
        // 栈不空就出栈
        if (!StackEmpty(S)) {
            p = Pop(&S);
            printf("结点 %c 出栈 \n", p->data);
            // 访问右边
            p = p->rchild;
        }
    }
    return OK;
}



// 非递归后序遍历二叉树 T  左右根
Status NRPostOrderTraverse(BiTree T) {
    SqStack S;
    InitStack(&S);
    BiTree p = T;
    BiTree lastVisited = NULL;
    while (p ||!StackEmpty(S)) {
        if(p) {
            S.data[++(S.top)] = p;
            // 访问左结点
            p = p->lchild;
        }
        else {
            p = S.data[S.top];
            // 不急着出栈左结点 看看右节点是否存在以及是否被访问过 压入栈中
            if (p->rchild != NULL && p->rchild != lastVisited) {
                p = p->rchild;
                S.data[++(S.top)] = p;
                // 查看右结点的左结点
                p = p ->lchild;
            }
            else {
                p = S.data[(S.top)--];
                printf("出栈 %c \n",p->data);
                lastVisited = p;
                // 在一个结点出栈后，紧接着下一个结点出栈，所以p直接置空
                p = NULL;
            }
        }
    }
    return OK;
}

// 应用实例1：返回二叉树T度分别为 0 , 1 , 2的结点数
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

// 应用实例2：求二叉树 T 的高度
int High(BiTree T) {
    if (T == NULL) return 0;
    else {
        int leftHeight = High(T->lchild);
        int rightHeight = High(T->rchild);
        return (leftHeight > rightHeight)? (leftHeight + 1) : (rightHeight + 1);
    }
}

// 应用实例3：根据给定的字符序列生成二叉排序树
void CreateBST(BiTree *T, const char *chars) {
    *T = NULL;
    int len = strlen(chars);
    for (int i = 0; i < len; i++) {
        BiTree p = *T;
        BiTree q = NULL;
        while (p!= NULL) {
            q = p;
            if (chars[i] < p->data) {
                // 小的放左边
                p = p->lchild;
            } else {
                // 大的右边
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
    // (1) 调用CreateBiTree(T)函数生成一棵二叉树T
    BiTree T;
    printf("请输入二叉树T的先序遍历序列（#表示空节点）：\n");
    CreateBiTree(&T);
    printf("先序遍历成功！\n");

    printf("------------------------------\n");


    // (2) 分别调用先序遍历、中序遍历和后序遍历的递归函数输出相应的遍历结果
    printf("递归先序遍历结果：\n");
    PreOrderTraverse(T);
    printf("\n");
    printf("递归中序遍历结果：\n");
    InOrderTraverse(T);
    printf("\n");
    printf("递归后序遍历结果：\n");
    PostOrderTraverse(T);
    printf("\n");

    printf("------------------------------\n");

    // (3) 分别调用先序遍历、中序遍历和后序遍历的非递归函数输出相应的遍历结果和栈元素的变化过程
    printf("非递归先序遍历结果及栈变化：\n");
    NRPreOrderTraverse(T);
    printf("------------------------------\n");
    printf("非递归中序遍历结果及栈变化：\n");
    NRInOrderTraverse(T);
    printf("------------------------------\n");
    printf("非递归后序遍历结果及栈变化：\n");
    NRPostOrderTraverse(T);
    printf("------------------------------\n");

    // (4) 调用TNodes(T)函数，输出二叉树T度分别为0、1、2的结点数
    int count0 = 0, count1 = 0, count2 = 0;
    TNodes(T, 0, &count0);
    TNodes(T, 1, &count1);
    TNodes(T, 2, &count2);
    printf("度为0的节点个数：%d\n", count0);
    printf("度为1的节点个数：%d\n", count1);
    printf("度为2的节点个数：%d\n", count2);

    printf("------------------------------\n");


    // (5) 调用CreateBST(T1,"DBFCAEG")，CreateBST(T2,"ABCDEFG")，创建两棵二叉树，对它们进行中序遍历，并调用High()函数输出其高度
    BiTree T1, T2;
    CreateBST(&T1, "DBFCAEG");
    CreateBST(&T2, "ABCDEFG");
    printf("T1中序遍历结果：");
    InOrderTraverse(T1);
    printf("\nT1高度：%d\n", High(T1));
    printf("T2中序遍历结果：");
    InOrderTraverse(T2);
    printf("\nT2高度：%d\n", High(T2));


    return 0;
}