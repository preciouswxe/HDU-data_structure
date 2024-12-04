#include <stdio.h>
#include <stdlib.h>

#define OK         1
#define ERROR   0
#define TRUE    1
#define FALSE    0

typedef int Status;  //声明函数类型名
typedef int ElemType;  //数据域类型


typedef struct LNode {  //结点类型定义
    ElemType       data;    // 数据域
    struct LNode  *next;   // 明确指定next是一个指向LNode结构体类型的指针
}LNode, *LinkList;    //节点  指针*LinkList定义为指向LNode的指针类型

// 通常用一个LinkList类型的指针来表示整个链表，这个指针指向链表的第一个节点（头节点）。
// 通过操作这个指针以及节点中的next指针，可以实现对整个链表的各种操作，如遍历、插入节点、删除节点等。

Status InitList(LinkList *L) {
    // 初始化头节点的内存 里面没有东西
    *L = (LinkList)malloc(sizeof(LNode));
    if((*L) == NULL) {
        return ERROR;
    }
    // 指向的下个节点为空
    (*L)->next = NULL;
    return OK;
}

Status ClearList(LinkList *L) {
    LinkList p,q;
    p = (*L)->next;
    while(p) {
        q = p;
        p = p->next;
        free(q);
    }
    (*L)->next = NULL;
    return OK;
}

int Listlength(LinkList L) {
    int len = 0;
    LinkList p;
    p = L->next;
    while(p) {
        len++;
        p = p->next;
    }
    return len;
}

Status GetElem(LinkList L,int i,ElemType *e){
    // 检查是否超出边界
    int len = Listlength(L);
    if (i < 1 || i > len){
        return ERROR;
    }

    LinkList p;
    p = L->next;
    for(int j = 1;j<i;j++) {
        p = p->next;
    }
    *e = p->data;
    return OK;
}

LNode* LocateList(LinkList L,ElemType e) {
    LinkList p = L->next;
    while(p){
        if(p->data == e) {
            return p;
        }
        p = p->next;
    }
    return NULL;
}

Status LinkInsert(LinkList *L,int i,ElemType e) {

    LinkList s = (LinkList)malloc(sizeof(LNode));
    if (!s) {
        return ERROR;
    }
    s ->data = e;
    if(i==1) {
        // 插入在头节点
        s->next = *L;
        *L = s;

    }else {
        // 数字和节点一起后移
        int j = 1;
        LinkList p = *L;
        while(p && j < i-1) {
            //printf("Current node data: %d, j value: %d\n", p->data, j);
            p = p->next;
            j++;
        }
        if (!p && j < i-1) {
            return ERROR;
        }

        s -> next = p -> next;
        p -> next = s;
    }
    //(*L) ->next =NULL;
    return OK;
}

Status ListDelete(LinkList *L,int i,ElemType *e) {
    LinkList p = (*L)->next;
    if(i==1) {
        *e = (*L)->next->data;
        LinkList temp = (*L)->next;
        (*L)->next = (*L)->next->next;
        free(temp);
        return OK;
    }

    for(int j=1;j<i;j++) {
        p = p->next;
    }
    LinkList temp = p->next;
    *e = temp->data;
    p->next = p->next->next;
    free(temp);
    return OK;
}

Status ListTraverse(LinkList L) {
    LinkList p = L;
    int count = 0;
    while (p && p->next!= NULL) {
        printf("%d ", p->data);
        p = p->next;

    }
    printf("\n");
    return OK;

}

// 并
Status Union(LinkList *La,LinkList Lb) {
    // 头节点也有值
    LinkList pa = *La;
    LinkList pb = Lb;
    LinkList prev = *La;
    while(pa && pb) {
        if(pa->data == pb->data) {
            prev = pa;
            pa = pa->next;
            pb = pb->next;
        }else if(pa->data < pb->data) {
            prev = pa;
            pa = pa->next;
        }else {
            // > pb插进来
            LinkList new = (LinkList)malloc(sizeof(LNode));
            new -> data = pb -> data;
            new -> next = prev -> next;
            // 先处理好前一个 再后移
            prev -> next = new;
            prev = prev->next;
            pb = pb->next;
        }
    }
    // 如果pb还有多余 都放到pa最后
    while(pb) {
        LinkList new = (LinkList)malloc(sizeof(LNode));
        new -> data = pb -> data;
        new -> next = prev -> next;
        // 先处理好前一个 再后移
        prev -> next = new;
        prev = prev->next;
        pb = pb->next;
    }
    return OK;
}

// 交
Status Intersection(LinkList *La,LinkList Lb) {
    // 头节点也有值
    LinkList pa = *La;
    LinkList pb = Lb;
    LinkList prev = *La;
    while(pa && pb) {
        if(pa->data < pb->data) {
            LinkList left = pa;
            // pa继续后移 prev的指针指着新的pa 释放掉之前的pa
            pa = pa->next;
            prev->next = pa;
            free(left);
        }else if(pa->data > pb->data) {
            pb = pb->next;
        }else {
            // prev变成这个条件成立的pa节点 papb一起后移
            prev = pa;
            pa = pa->next;
            //printf("%d\n",pa->data);
            pb = pb->next;
            //printf("pb:%d\n",pb->data);
        }
    }

    //pa = pa->next;
    // 多余释放
    /*
     while(pa) {
        LinkList temp = pa;
        pa = pa->next;
        prev->next = pa;
        free(temp);
    }
    */

    return OK;
}

// 差
Status Difference(LinkList *La,LinkList Lb) {
    // 头节点也有值
    LinkList pa = *La;
    LinkList pb = Lb;
    LinkList prev = *La;
    while(pa && pb) {
        if(pa->data < pb->data) {
            prev = pa;
            pa = pa->next;
        }else if(pa->data > pb->data) {
            pb = pb->next;
        }else {
            //删除相同的
            LinkList temp = pa;
            pa = pa->next;
            prev->next = pa;
            free(temp);
        }
    }
    return OK;
}

// 两个有序表的合并
Status MergeList(LinkList La,LinkList Lb,LinkList *Lc) {
    *Lc = (LinkList)malloc(sizeof(LNode));
    (*Lc)->data = La->data; // 将头节点数据初始化为 La 的头节点数据
    (*Lc)->next = NULL;
    LinkList pc = *Lc;
    LinkList pa = La->next;
    LinkList pb = Lb->next;
    while(pa && pb){
        if(pa->data < pb->data) {
            LinkList new = (LinkList)malloc(sizeof(LNode));
            new->data = pa->data;
            new->next = NULL;
            pc->next = new;
            pc = pc->next;
            pa = pa->next;
        }else {
            LinkList new = (LinkList)malloc(sizeof(LNode));
            new->data = pb->data;
            new->next = NULL;
            pc->next = new;
            pc = pc->next;
            pb = pb->next;
        }
    }
    while(pa) {
        LinkList new = (LinkList)malloc(sizeof(LNode));
        new->data = pa->data;
        new->next = NULL;
        pc->next = new;
        pc = pc->next;
        pa = pa->next;
    }
    while(pb) {
        LinkList new = (LinkList)malloc(sizeof(LNode));
        new->data = pb->data;
        new->next = NULL;
        pc->next = new;
        pc = pc->next;
        pb = pb->next;
    }

    // 对新链表进行排序
    LinkList current = *Lc;
    LinkList index = NULL;
    ElemType temp;

    while (current) {
        index = current->next;
        while (index) {
            if (current->data > index->data) {
                temp = current->data;
                current->data = index->data;
                index->data = temp;
            }
            index = index->next;
        }
        current = current->next;
    }

    return OK;
}

// 去掉单链表中的重复元素
void Purge(LinkList *Lc) {
    LinkList p = (*Lc)->next;
    LinkList prev = *Lc;
    while (p) {
        LinkList q = p;
        LinkList qprev = prev;
        while (q->next) {
            if (q->next->data == p->data) {
                LinkList temp = q->next;
                q->next = q->next->next;
                free(temp);
            } else {
                q = q->next;
            }
        }
        prev = p;
        p = p->next;
    }
}

int main(){
    LNode LA,LB;
    LinkList la=&LA, lb=&LB;    // la lb是指针变量 单la是变量 &la是传地址

    int La_data[] = {2,8,27,39,66,77,89};
    int Lb_data[] = {6,18,27,59,65,77,89,120,140};

    // 初始化
    InitList(&la);
    InitList(&lb);

    // 插入数据
    for(int i = 0;i<sizeof(La_data)/sizeof(La_data[0]);i++) {
        //printf("Inserting value: %d\n", La_data[i]);
        LinkInsert(&la,i+1,La_data[i]);
    }

    for(int i = 0;i<sizeof(Lb_data)/sizeof(Lb_data[0]);i++) {
        LinkInsert(&lb,i+1,Lb_data[i]);
    }

    // 初始化输出
    printf("LA = \n");
    ListTraverse(la);
    printf("LB = \n");
    ListTraverse(lb);

    printf("-----------------\n");

    // 求并集
    Union(&la,lb);
    printf("After Union:\n");
    ListTraverse(la);

    // 清空la重新赋值
    ClearList(&la);
    InitList(&la);
    for(int i = 0;i<sizeof(La_data)/sizeof(La_data[0]);i++) {
        //printf("Inserting value: %d\n", La_data[i]);
        LinkInsert(&la,i+1,La_data[i]);
    }

    printf("-----------------\n");

    //求交集
    Intersection(&la,lb);
    printf("After Intersection:\n");
    ListTraverse(la);

    // 清空la重新赋值
    ClearList(&la);
    InitList(&la);
    for(int i = 0;i<sizeof(La_data)/sizeof(La_data[0]);i++) {
        //printf("Inserting value: %d\n", La_data[i]);
        LinkInsert(&la,i+1,La_data[i]);
    }

    printf("-----------------\n");

    // 差集操作
    Difference(&la, lb);
    printf("After Difference:\n");
    ListTraverse(la);

    // 清空la重新赋值
    ClearList(&la);
    InitList(&la);
    for(int i = 0;i<sizeof(La_data)/sizeof(La_data[0]);i++) {
        //printf("Inserting value: %d\n", La_data[i]);
        LinkInsert(&la,i+1,La_data[i]);
    }

    printf("-----------------\n");

    // 有序表合并
    LinkList lc;
    MergeList(la, lb, &lc);
    printf("After MergeList(LA, LB), LC = \n");
    ListTraverse(lc);

    // 去掉重复多余的元素
    Purge(&lc);
    printf("After Purge(LC), LC = \n");
    ListTraverse(lc);

    // 释放内存
    ClearList(&la);
    ClearList(&lb);
    ClearList(&lc);

    return 0;

}

















