#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR   0
#define TRUE    1
#define FALSE    0
#define INFEASIBLE -1
#define OVERFLOW -2
#define MAXSIZE    100

typedef int ElemType;
typedef int Status ;
typedef struct SqList {
    ElemType *elem;
    int length;
}SqList;

Status InitList(SqList *L) {
    // 修改后分配了MAXSIZE个ElemType大小的内存空间，确保有足够的空间来存储元素
    L->elem = (ElemType *)malloc(MAXSIZE * sizeof(ElemType));
    if (!L->elem) {
        printf("defeat！\n");
        exit(OVERFLOW);
    }
    L->length = 0;
    return OK;
}

Status ClearList(SqList *L) {
    L->length = 0;
    return OK;
}

int Listlength(SqList L) {
    return L.length;
}

Status GetElem(SqList *L, int i, ElemType *e) {
    if (i < 1 || i > L->length) {
        return ERROR;
    }
    *e = L->elem[i - 1];
    return OK;
}

int LocateList(SqList L, ElemType e) {
    for (int i = 0; i < L.length; i++) {
        if (L.elem[i] == e) {
            return i + 1;
        }
    }
    return 0;
}

Status ListInsert(SqList *L, int i, ElemType e) {
    if (i < 1 || i > L->length + 1 || L->length == MAXSIZE) {
        return ERROR;
    }
    for (int j = L->length; j >= i; j--) {
        L->elem[j] = L->elem[j - 1];
    }
    L->elem[i - 1] = e;
    L->length++;
    return OK;
}

Status ListDelete(SqList *L, int i, ElemType *e) {
    if (i < 1 || i > L->length) {
        return ERROR;
    }
    *e = L->elem[i - 1];
    for (int j = i; j < L->length; j++) {
        L->elem[j - 1] = L->elem[j];
    }
    L->length--;
    return OK;
}

Status ListTraverse(SqList L) {
    for (int i = 0; i < L.length; i++) {
        printf("%d ", L.elem[i]);
    }
    return OK;
}

Status Union(SqList *La, SqList Lb) {
    ElemType e;
    for (int i = 0; i < Lb.length; i++) {
        e = Lb.elem[i];
        int j;
        for (j = 0; j < La->length; j++) {
            if (e == La->elem[j]) {
                break;
            }
        }
        if (j == La->length) {
            if (La->length >= MAXSIZE) return OVERFLOW;
            La->elem[La->length++] = e;
        }
    }
    return OK;
}

Status Intersection(SqList *La, SqList Lb) {
    SqList temp;
    temp.elem = (ElemType *)malloc(MAXSIZE * sizeof(ElemType));
    if (!temp.elem) return OVERFLOW;
    temp.length = 0;
    ElemType e;
    for (int i = 0; i < La->length; i++) {
        e = La->elem[i];
        for (int j = 0; j < Lb.length; j++) {
            if (e == Lb.elem[j]) {
                if (temp.length >= MAXSIZE) return OVERFLOW;
                temp.elem[temp.length] = e;
                temp.length++;
                break;
            }
        }
    }
    free(La->elem);
    La->elem = temp.elem;
    La->length = temp.length;
    return OK;
}

Status Difference(SqList *La, SqList Lb) {
    SqList temp;
    temp.elem = (ElemType *)malloc(MAXSIZE * sizeof(ElemType));
    if (!temp.elem) return OVERFLOW;
    temp.length = 0;
    ElemType e;
    int i, j;
    for (i = 0; i < La->length; i++) {
        e = La->elem[i];
        int found = 0;
        for (j = 0; j < Lb.length; j++) {
            if (Lb.elem[j] == e) {
                found = 1;
                break;
            }
        }
        if (!found) {
            if (temp.length >= MAXSIZE) return OVERFLOW;
            temp.elem[temp.length] = e;
            temp.length++;
        }
    }
    free(La->elem);
    La->elem = temp.elem;
    La->length = temp.length;
    return OK;
}

Status MergeList(SqList La, SqList Lb, SqList *Lc) {
    Lc->elem = (ElemType *)malloc((La.length + Lb.length) * sizeof(ElemType));
    if (!Lc->elem) return OVERFLOW;
    Lc->length = 0;

    int i = 0, j = 0, k = 0;
    while (i < La.length && j < Lb.length) {
        if (La.elem[i] <= Lb.elem[j]) {
            Lc->elem[k] = La.elem[i];
            i++;
        } else {
            Lc->elem[k] = Lb.elem[j];
            j++;
        }
        k++;
    }

    while (i < La.length) {
        Lc->elem[k] = La.elem[i];
        i++;
        k++;
    }

    while (j < Lb.length) {
        Lc->elem[k] = Lb.elem[j];
        j++;
        k++;
    }

    Lc->length = k;
    return OK;
}

Status Purge(SqList *Lc) {
    if (Lc->length == 0) {
        return OK;
    }

    int i, j, k;
    for (i = 0, k = 0; i < Lc->length; i++) {
        for (j = 0; j < k; j++) {
            if (Lc->elem[j] == Lc->elem[i]) break;
        }
        if (j == k) {
            Lc->elem[k] = Lc->elem[i];
            k++;
        }
    }
    Lc->length = k;
    return OK;
}

int main() {
    SqList LA, LB, LC;
    InitList(&LA);
    InitList(&LB);

    // 生成 LA
    int LA_data[] = {2, 8, 27, 39, 66, 77, 89};
    for (int i = 0; i < sizeof(LA_data) / sizeof(LA_data[0]); i++) {
        ListInsert(&LA, i + 1, LA_data[i]);
    }
    // 生成 LB
    int LB_data[] = {6, 18, 27, 59, 65, 77, 89, 120, 140};
    for (int i = 0; i < sizeof(LB_data) / sizeof(LB_data[0]); i++) {
        ListInsert(&LB, i + 1, LB_data[i]);
    }

    // 输出 LA
    printf("LA = ");
    ListTraverse(LA);

    // 输出 LB
    printf("\n LB = ");
    ListTraverse(LB);

    // 并
    Union(&LA, LB);
    printf("\n Union, LA = ");
    ListTraverse(LA);

    // 重新初始化 LA
    free(LA.elem);
    InitList(&LA);
    for (int i = 0; i < sizeof(LA_data) / sizeof(LA_data[0]); i++) {
        ListInsert(&LA, i + 1, LA_data[i]);
    }

    // 输出 LA
    printf("LA = ");
    ListTraverse(LA);

    // 交
    Intersection(&LA, LB);
    printf("\n Intersection, LA = ");
    ListTraverse(LA);

    // 先释放 再 重新初始化 LA
    free(LA.elem);
    InitList(&LA);
    for (int i = 0; i < sizeof(LA_data) / sizeof(LA_data[0]); i++) {
        ListInsert(&LA, i + 1, LA_data[i]);
    }

    // 差
    Difference(&LA, LB);
    printf("\n Difference, LA = ");
    ListTraverse(LA);

    // 排序产生 LC
    InitList(&LC);
    MergeList(LA, LB, &LC);
    printf("\n Mergelist, LC = ");
    ListTraverse(LC);

    // 去掉重复元素
    Purge(&LC);
    printf("\n Purge, LC = ");
    ListTraverse(LC);

    return 0;
}