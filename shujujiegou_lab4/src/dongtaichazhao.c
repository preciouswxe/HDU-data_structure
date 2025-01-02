//
// Created by 13561 on 2024/12/26.
//

#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define ERROR 0
typedef int Status;
typedef int Bool;
// 1. 动态查找表的存储结构和相关内容的定义
typedef int KeyType;  // 关键字的数据类型

// 数据元素（记录）的类型定义
typedef struct {
    KeyType key;  // 学号(关键字)
    char name[8]; // 姓名
    char sex[4];  // 性别
    int age;      // 年龄
} ElemType;

// 动态查找表 BST 的类型定义
typedef struct BSTNode {
    ElemType data;
    struct BSTNode *lchild, *rchild; // 指向左右孩子结点的指针
} BSTNode, *BSTree;

typedef struct {
    ElemType *elem; // 数据元素存储空间的基址(一维数组名)
    int length;     // 表的长度（元素个数）
} SSTable;

static ElemType sheet[] = {{56, "Zhang\0", "F\0", 19},
                            {19, "Wang\0", "F\0", 20},
                            {80, "Zhou\0", "F\0", 19},
                            {5, "Huang\0", "M\0", 20},
                            {21, "Zheng\0", "M\0", 20},
                            {64, "Li\0", "M\0", 19},
                            {88, "Liu\0", "F\0", 18},
                            {1, "Qian\0", "F\0", 19},
                            {7, "Sun\0", "M\0", 20},
                            {75, "Zhao\0", "M\0", 20},
                            {92, "Chen\0", "M\0", 20}};

KeyType checkKey[] = {0, 3, 6, 10, 20, 23, 61, 73, 78, 81, 89, 99};
static int n_checkKey = sizeof(checkKey) / sizeof(KeyType);
static int n_sheet = sizeof(sheet) / sizeof(ElemType);
static FILE *instream; // 输入流
static FILE *outstream; // 输出流
static int lookTimes = 0;

// 2. 动态查找表基本操作的实现
// 根据定义的存储结构，编写下列基本操作函数的 C / C++ 源代码：

// (1) 插入函数 InsertDSTable_BST，根据 BST 的性质将数据元素 e 插入到动态查找表 DT 中
Status CreateSSTable(SSTable *ST, int n) {
    ST->length = n;
    ElemType *p = (ElemType *)calloc(1 + n, sizeof(ElemType));
    if (!p) {
        fprintf(stderr, "堆分配失败\n");
        exit(ERROR);
        return ERROR;
    }
    ST->elem = p;
    for (int i = 1; i <= n; i++) {
        p[i] = sheet[i - 1];
    }
    return OK;
}

void printElem(ElemType e) {
    fprintf(outstream, "  %2d      %5s        %2s          %2d\n", e.key, e.name, e.sex, e.age);
}

void TraversSSTable(SSTable *ST) {
    fprintf(outstream, "学号key  姓名name    性别sex    年龄age \n");
    ElemType *outPoint = ST->elem;
    int outNum = ST->length;
    for (int i = 1; i <= outNum; i++) {
        printElem(outPoint[i]);
    }
}

Bool InsertDSTable_BST(BSTree *DT, ElemType e) {
    if (*DT == NULL) {
        *DT = (BSTree)malloc(sizeof(BSTNode));
        (*DT)->data = e;
        (*DT)->lchild = (*DT)->rchild = NULL;
        return 1; // 插入成功
    }

    BSTree *addPlacePoint = NULL;
    BSTree *nowPoint = DT;
    int standardNum = e.key;
    BSTree tempNodePoint = (BSTree)malloc(sizeof(BSTNode));
    if (!tempNodePoint) {
        fprintf(stderr, "堆分配失败！\n");
        exit(0);
    }
    tempNodePoint->data = e;
    tempNodePoint->lchild = tempNodePoint->rchild = NULL;

    while (addPlacePoint == NULL) {
        if (*nowPoint == NULL) {
            addPlacePoint = nowPoint;
            *addPlacePoint = tempNodePoint;
            break;
        }
        int nowKey = (*nowPoint)->data.key;
        if (nowKey > standardNum) {
            nowPoint = &((*nowPoint)->lchild);
        } else if (nowKey < standardNum) {
            nowPoint = &((*nowPoint)->rchild);
        } else {
            return 0; // 元素已存在
        }
    }
    return 1;
}

// (2) 创建动态查找表的函数 CreateDSTable_BST
Status CreateDSTable_BST(BSTree *DT, SSTable *ST) {
    ElemType *tempSheet = ST->elem;
    int lenSheet = ST->length;
    for (int i = 1; i <= lenSheet; i++) {
        if (!InsertDSTable_BST(DT, tempSheet[i])) {
            return 0; // 插入失败
        }
    }
    return 1;
}

// (3) 中序遍历函数 TraversDSTable_InOrder
void TraversDSTable_InOrder(BSTree DT) {
    if (DT == NULL)
        return;
    TraversDSTable_InOrder(DT->lchild);
    printElem(DT->data);
    TraversDSTable_InOrder(DT->rchild);
}

// (4) 查找函数 SearchDSTable_BST
BSTree *SearchDSTable_BST(BSTree *DT, KeyType key) {
    lookTimes = 0;
    BSTree *nowPoint = DT;
    while (*nowPoint != NULL) {
        int nowKey = (*nowPoint)->data.key;
        lookTimes++;
        if (nowKey > key) {
            nowPoint = &((*nowPoint)->lchild);
        } else if (nowKey < key) {
            nowPoint = &((*nowPoint)->rchild);
        } else {
            return nowPoint;
        }
    }
    return NULL;
}

// (5) 删除函数 DeleteDSTable_BST
Bool DeleteDSTable_BST(BSTree *DT, KeyType key) {
    if (*DT == NULL)
        return 0;

    if ((*DT)->data.key == key && (*DT)->lchild == NULL && (*DT)->rchild == NULL) { // 只有一个节点
        free(*DT);
        *DT = NULL;
        return 1;
    }

    if ((*DT)->data.key == key && (*DT)->lchild == NULL) {
        BSTree tempPoint = *DT;
        *DT = (*DT)->rchild;
        free(tempPoint);
        return 1;
    }

    if ((*DT)->data.key == key && (*DT)->rchild == NULL) {
        BSTree tempPoint = *DT;
        *DT = (*DT)->lchild;
        free(tempPoint);
        return 1;
    }

    // 删除有两个子节点的情况
    BSTree tempPoint = *DT;
    BSTree *findPoint = &((*DT)->lchild);
    while ((*findPoint)->rchild != NULL) {
        findPoint = &((*findPoint)->rchild);
    }
    (*findPoint)->rchild = (*DT)->rchild;
    *DT = (*DT)->lchild;
    free(tempPoint);
    return 1;
}

// 3. 编写主函数
int main() {
    BSTree DT = NULL;
    SSTable ST;
    instream = stdin;
    outstream = stdout;

    //（1）定义一个静态查找表 ST1，将下列各元素的值写入 ST1，然后输出 ST1 的各元素的值。
    {
        fprintf(outstream, "para1:\n");
        CreateSSTable(&ST, n_sheet);
        TraversSSTable(&ST);
    }

    //（2）根据 ST1 中的数据创建动态查找表 DT1，并输出 DT1 的所有数据元素。
    {
        fprintf(outstream, "para2:\n");
        CreateDSTable_BST(&DT, &ST);
        TraversDSTable_InOrder(DT);
    }

    //（3）查找操作
    fprintf(outstream, "para3:\n");
    while (1) {
        int key;
        fprintf(outstream, "请输入一个要查找的key: ");
        fscanf(instream, "%d", &key);
        if (key == -1)
            break;

        BSTree *temp = SearchDSTable_BST(&DT, key);
        if (temp != NULL) {
            printElem((*temp)->data);
            fprintf(outstream, "查找了%d次\n", lookTimes);
        } else {
            fprintf(outstream, "未找到该key\n");
        }
    }

    //（4）计算并输出查找成功的 ASL
    {
        fprintf(outstream, "para4:\n");
        int sumTimes = 0;
        for (int i = 1; i <= ST.length; i++) {
            printElem((*SearchDSTable_BST(&DT, ST.elem[i].key))->data);
            fprintf(outstream, "比较了%d次\n", lookTimes);
            sumTimes += lookTimes;
        }
        fprintf(outstream, "二叉树查找成功的ASL=%.4lf\n", sumTimes * 1.0 / ST.length);
    }

    //（5）查找失败的 ASL
    {
        fprintf(outstream, "para5:\n");
        int sumTimes = 0;
        for (int i = 0; i < n_checkKey; i++) {
            SearchDSTable_BST(&DT, checkKey[i]);
            fprintf(outstream, "查找%d，比较了%d次\n", checkKey[i], lookTimes);
            sumTimes += lookTimes;
        }
        fprintf(outstream, "二叉树查找失败的ASL=%.4lf\n", sumTimes * 1.0 / n_checkKey);
    }

    //（6）插入操作
    fprintf(outstream, "para6:\n");
    while (1) {
        ElemType e;
        fprintf(outstream, "请输入一条要添加的记录: ");
        fscanf(instream, "%d", &e.key);
        if (e.key == -1) break;
        fscanf(instream, "%s%s%d", e.name, e.sex, &e.age);
        if (InsertDSTable_BST(&DT, e))
            TraversDSTable_InOrder(DT);
        else
            fprintf(outstream, "当前要插入的记录已存在，禁止插入！\n");
    }

    //（7）删除操作
    fprintf(outstream, "para7:\n");
    while (1) {
        int key;
        fprintf(outstream, "请输入一个要删除的key: ");
        fscanf(instream, "%d", &key);
        if (key == -1)
            break;
        if (!DeleteDSTable_BST(&DT, key))
            fprintf(outstream, "当前要删除的记录不存在，禁止删除！\n");
        else
            TraversDSTable_InOrder(DT);
    }

    return 0;
}
