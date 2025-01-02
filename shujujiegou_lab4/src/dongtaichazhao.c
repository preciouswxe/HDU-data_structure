//
// Created by 13561 on 2024/12/26.
//

#include <stdio.h>
#include <stdlib.h>
#define OK 1
#define ERROR 0
typedef int Status;
typedef int Bool;
// 1. ��̬���ұ�Ĵ洢�ṹ��������ݵĶ���
typedef int KeyType;  // �ؼ��ֵ���������

// ����Ԫ�أ���¼�������Ͷ���
typedef struct {
    KeyType key;  // ѧ��(�ؼ���)
    char name[8]; // ����
    char sex[4];  // �Ա�
    int age;      // ����
} ElemType;

// ��̬���ұ� BST �����Ͷ���
typedef struct BSTNode {
    ElemType data;
    struct BSTNode *lchild, *rchild; // ָ�����Һ��ӽ���ָ��
} BSTNode, *BSTree;

typedef struct {
    ElemType *elem; // ����Ԫ�ش洢�ռ�Ļ�ַ(һά������)
    int length;     // ��ĳ��ȣ�Ԫ�ظ�����
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
static FILE *instream; // ������
static FILE *outstream; // �����
static int lookTimes = 0;

// 2. ��̬���ұ����������ʵ��
// ���ݶ���Ĵ洢�ṹ����д���л������������� C / C++ Դ���룺

// (1) ���뺯�� InsertDSTable_BST������ BST �����ʽ�����Ԫ�� e ���뵽��̬���ұ� DT ��
Status CreateSSTable(SSTable *ST, int n) {
    ST->length = n;
    ElemType *p = (ElemType *)calloc(1 + n, sizeof(ElemType));
    if (!p) {
        fprintf(stderr, "�ѷ���ʧ��\n");
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
    fprintf(outstream, "ѧ��key  ����name    �Ա�sex    ����age \n");
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
        return 1; // ����ɹ�
    }

    BSTree *addPlacePoint = NULL;
    BSTree *nowPoint = DT;
    int standardNum = e.key;
    BSTree tempNodePoint = (BSTree)malloc(sizeof(BSTNode));
    if (!tempNodePoint) {
        fprintf(stderr, "�ѷ���ʧ�ܣ�\n");
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
            return 0; // Ԫ���Ѵ���
        }
    }
    return 1;
}

// (2) ������̬���ұ�ĺ��� CreateDSTable_BST
Status CreateDSTable_BST(BSTree *DT, SSTable *ST) {
    ElemType *tempSheet = ST->elem;
    int lenSheet = ST->length;
    for (int i = 1; i <= lenSheet; i++) {
        if (!InsertDSTable_BST(DT, tempSheet[i])) {
            return 0; // ����ʧ��
        }
    }
    return 1;
}

// (3) ����������� TraversDSTable_InOrder
void TraversDSTable_InOrder(BSTree DT) {
    if (DT == NULL)
        return;
    TraversDSTable_InOrder(DT->lchild);
    printElem(DT->data);
    TraversDSTable_InOrder(DT->rchild);
}

// (4) ���Һ��� SearchDSTable_BST
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

// (5) ɾ������ DeleteDSTable_BST
Bool DeleteDSTable_BST(BSTree *DT, KeyType key) {
    if (*DT == NULL)
        return 0;

    if ((*DT)->data.key == key && (*DT)->lchild == NULL && (*DT)->rchild == NULL) { // ֻ��һ���ڵ�
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

    // ɾ���������ӽڵ�����
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

// 3. ��д������
int main() {
    BSTree DT = NULL;
    SSTable ST;
    instream = stdin;
    outstream = stdout;

    //��1������һ����̬���ұ� ST1�������и�Ԫ�ص�ֵд�� ST1��Ȼ����� ST1 �ĸ�Ԫ�ص�ֵ��
    {
        fprintf(outstream, "para1:\n");
        CreateSSTable(&ST, n_sheet);
        TraversSSTable(&ST);
    }

    //��2������ ST1 �е����ݴ�����̬���ұ� DT1������� DT1 ����������Ԫ�ء�
    {
        fprintf(outstream, "para2:\n");
        CreateDSTable_BST(&DT, &ST);
        TraversDSTable_InOrder(DT);
    }

    //��3�����Ҳ���
    fprintf(outstream, "para3:\n");
    while (1) {
        int key;
        fprintf(outstream, "������һ��Ҫ���ҵ�key: ");
        fscanf(instream, "%d", &key);
        if (key == -1)
            break;

        BSTree *temp = SearchDSTable_BST(&DT, key);
        if (temp != NULL) {
            printElem((*temp)->data);
            fprintf(outstream, "������%d��\n", lookTimes);
        } else {
            fprintf(outstream, "δ�ҵ���key\n");
        }
    }

    //��4�����㲢������ҳɹ��� ASL
    {
        fprintf(outstream, "para4:\n");
        int sumTimes = 0;
        for (int i = 1; i <= ST.length; i++) {
            printElem((*SearchDSTable_BST(&DT, ST.elem[i].key))->data);
            fprintf(outstream, "�Ƚ���%d��\n", lookTimes);
            sumTimes += lookTimes;
        }
        fprintf(outstream, "���������ҳɹ���ASL=%.4lf\n", sumTimes * 1.0 / ST.length);
    }

    //��5������ʧ�ܵ� ASL
    {
        fprintf(outstream, "para5:\n");
        int sumTimes = 0;
        for (int i = 0; i < n_checkKey; i++) {
            SearchDSTable_BST(&DT, checkKey[i]);
            fprintf(outstream, "����%d���Ƚ���%d��\n", checkKey[i], lookTimes);
            sumTimes += lookTimes;
        }
        fprintf(outstream, "����������ʧ�ܵ�ASL=%.4lf\n", sumTimes * 1.0 / n_checkKey);
    }

    //��6���������
    fprintf(outstream, "para6:\n");
    while (1) {
        ElemType e;
        fprintf(outstream, "������һ��Ҫ��ӵļ�¼: ");
        fscanf(instream, "%d", &e.key);
        if (e.key == -1) break;
        fscanf(instream, "%s%s%d", e.name, e.sex, &e.age);
        if (InsertDSTable_BST(&DT, e))
            TraversDSTable_InOrder(DT);
        else
            fprintf(outstream, "��ǰҪ����ļ�¼�Ѵ��ڣ���ֹ���룡\n");
    }

    //��7��ɾ������
    fprintf(outstream, "para7:\n");
    while (1) {
        int key;
        fprintf(outstream, "������һ��Ҫɾ����key: ");
        fscanf(instream, "%d", &key);
        if (key == -1)
            break;
        if (!DeleteDSTable_BST(&DT, key))
            fprintf(outstream, "��ǰҪɾ���ļ�¼�����ڣ���ֹɾ����\n");
        else
            TraversDSTable_InOrder(DT);
    }

    return 0;
}
