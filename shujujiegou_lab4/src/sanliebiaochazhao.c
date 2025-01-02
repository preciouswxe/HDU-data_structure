//
// Created by 13561 on 2024/12/26.
//
#include<stdio.h>
#include<stdlib.h>

#define SUCCESS 1
#define UNSUCCESS 0
#define ERROR 0
#define OK 1

typedef int KeyType;
typedef int Status;

// 哈希表的结构体
typedef struct{
    KeyType *key;
    int count;
    int size;
} HashTable;

// 链式哈希中的单链表节点
typedef struct KeyNode{
    KeyType key;
    struct KeyNode *next;
} KeyNode, *KeyLink;

// 链式哈希表的结构体
typedef struct{
    KeyLink *elem;
    int count;
    int size;
} HashLink;

// 哈希函数
int Hash(KeyType key){
    return key % 13;
}

// 创建哈希表（线性探测法）
Status CreateHashTable(HashTable *HT, int HT_Length, KeyType key[], int KeyNum) {
    HT->key = (KeyType *)malloc(HT_Length * sizeof(KeyType));
    HT->count = KeyNum;
    HT->size = HT_Length;
    for (int i = 0; i < HT->size; i++) {
        HT->key[i] = -1;
    }

    for (int i = 0; i < HT->count; i++) {
        int k = Hash(key[i]);
        if (HT->key[k] == -1) {
            HT->key[k] = key[i];
        } else {
            int j = (k + 1) % HT_Length;
            while (j != k && HT->key[j] != -1) {
                j = (j + 1) % HT_Length;
            }
            if (HT->key[j] == -1) {
                HT->key[j] = key[i];
            } else {
                return ERROR;
            }
        }
    }
    return OK;
}

// 在哈希表中查找关键字
Status SearchHashTable(HashTable HT, KeyType key, int *p, int *c) {
    *p = Hash(key);
    *c = 0;
    while (HT.key[*p] != -1 && ((*p + 1) % HT.size) != Hash(key) && HT.key[*p] != key) {
        (*c)++;
        *p = (*p + 1) % HT.size;
    }
    if (HT.key[*p] == key) {
        (*c)++;
        return SUCCESS;
    } else {
        *p = -1;
        return UNSUCCESS;
    }
}

// 创建链式哈希表
Status CreateHashLink(HashLink *HL, int HL_Length, KeyType key[], int KeyNum) {
    KeyLink s;
    HL->elem = (KeyLink *)malloc(HL_Length * sizeof(KeyLink));
    HL->count = KeyNum;
    HL->size = HL_Length;
    for (int i = 0; i < HL->size; i++) {
        HL->elem[i] = NULL;
    }

    for (int i = 0; i < HL->count; i++) {
        s = (KeyLink)malloc(sizeof(KeyNode));
        s->key = key[i];
        int k = Hash(key[i]);
        s->next = HL->elem[k];
        HL->elem[k] = s;
    }
    return OK;
}

// 在链式哈希表中查找关键字
Status SearchHashLink(HashLink HL, KeyType key, KeyLink *p, int *c) {
    *p = HL.elem[Hash(key)];
    *c = 0;
    while (*p && (*p)->key != key) {
        (*c)++;
        *p = (*p)->next;
    }
    if (*p) {
        (*c)++;
        return SUCCESS;
    } else {
        return UNSUCCESS;
    }
}

// 输出哈希表（线性探测法）
Status OutHashTable(HashTable HT) {
    for (int i = 0; i < HT.size; i++) {
        printf("%d\n", HT.key[i]);
    }
    return OK;
}

// 输出链式哈希表
Status OutHashLink(HashLink HL) {
    for (int i = 0; i < HL.size; i++) {
        if (HL.elem[i] != NULL) {
            KeyLink temp = HL.elem[i];
            while (temp) {
                printf("%d ", temp->key);
                temp = temp->next;
            }
            printf("\n");
        }
    }
    return OK;
}

int main() {
    int keys[12] = {19, 14, 23, 1, 68, 20, 84, 27, 55, 11, 10, 79};
    HashTable HT;
    HashLink HL;

    printf("关键字：\n");
    for (int i = 0; i < 12; i++) {
        printf("%d ", keys[i]);
    }
    printf("\n");

    // 创建并输出哈希表
    CreateHashTable(&HT, 16, keys, 12);
    OutHashTable(HT);

    int p, c;
    double sum = 0;
    for (int i = 0; i < 12; i++) {
        c = 0;
        SearchHashTable(HT, keys[i], &p, &c);
        sum += c;
        printf("关键字：%d  ", keys[i]);
        printf("下标：%d  ", p);
        printf("比较次数：%d\n", c);
    }
    printf("查找成功的ASL：%lf\n", sum / 12);

    // 创建链式哈希表并输出
    CreateHashLink(&HL, 13, keys, 12);
    OutHashLink(HL);

    sum = 0;
    for (int i = 0; i < 12; i++) {
        c = 0;
        KeyLink q;
        SearchHashLink(HL, keys[i], &q, &c);
        sum += c;
        printf("关键字：%d  ", keys[i]);
        printf("比较次数：%d\n", c);
    }
    printf("查找成功的ASL：%lf\n", sum / 12);

    // 线性探测法查找不成功的案例
    int key1[13] = {26, 40, 15, 29, 30, 18, 32, 46, 60, 74, 36, 24, 38};
    sum = 0;
    for (int i = 0; i < 13; i++) {
        c = 0;
        SearchHashTable(HT, key1[i], &p, &c);
        sum += c;
        printf("关键字：%d  ", key1[i]);
        printf("比较次数：%d\n", c);
    }
    printf("HT查找不成功的ASL：%lf\n", sum / 13);

    sum = 0;
    for (int i = 0; i < 13; i++) {
        c = 0;
        KeyLink q;
        SearchHashLink(HL, key1[i], &q, &c);
        sum += c;
        printf("关键字：%d  ", key1[i]);
        printf("比较次数：%d\n", c);
    }
    printf("HL查找不成功的ASL：%lf", sum / 13);

    return 0;
}
