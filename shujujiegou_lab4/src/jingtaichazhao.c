//
// Created by 13561 on 2024/12/26.
//
#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define OK 1;
typedef int KeyType;        //关键字的数据类型
typedef int Status;

//数据元素（记录）的类型定义
typedef struct {
    KeyType  key;            //学号(关键字)
    const char *name;        //姓名
    char sex;                //性别
    int  age;                //年龄
} RecordType;

//静态查找表的类型定义
typedef struct {
    RecordType *Record;      //数据元素存储空间的基址(一维数组名)
    int    length;             //表的长度（元素个数）
}SSTable;

// 题目要求
int key[11]={56,19,80,5,21,64,88,13,37,75,92};
const char name[11][11]={"Zhang","Wang","Zhou","Huang","Zheng","Li","Liu","Qian","Sun","Zhao","Chen"};
char sex[11]={'F','F','F','M','M','M','F','F','M','M','M'};               //name 是字符串指针，而 sex 是单个字符。
int age[11]={19,20,19,20,20,19,18,19,20,20,20};


// 1.创建静态查找表的函数
Status CreateSSTable(SSTable *ST, int n) {
    // 分配整体空间
    ST->Record=(RecordType *)malloc((n+1)* sizeof(RecordType));
    for(int i = 0;i<n;i++) {
        ST->Record[i].key = key[i];
        ST->Record[i].name= name[i];
        ST->Record[i].sex = sex[i];
        ST->Record[i].age = age[i];
    }

    ST->length = n;  // 设置表的长度
    return OK;
}


// 2.遍历输出静态查找表ST的所有数据元素
Status TraversSSTable(SSTable ST) {
    printf("key  name  sex  age\n");
    for(int i = 0;i<ST.length;i++) {
        printf("%d ",ST.Record[i].key);
        printf("%s ",ST.Record[i].name);
        printf("%c ",ST.Record[i].sex);
        printf("%d \n",ST.Record[i].age);
    }
    return OK;
}



// 3.顺序查找函数
// 其中参数key为要查找的关键字，参数c保存查找过程中关键字比较的次数。
// 若查找成功，函数返回该关键字元素所在的下标，否则，查找不成功，返回0（有 “哨兵”）或-1（无 “哨兵”）。
int SearchSSTable_Seq(SSTable ST, KeyType key, int *c) {
    // 如果第一个元素就是目标元素，直接返回
    if (ST.Record[0].key == key) {
        return 0; // 找到的情况
    }

    // 保存第一个元素的 key，避免修改
    KeyType originalKey = ST.Record[0].key;

    ST.Record[0].key = key; // 使用哨兵机制，将key放到第一个元素
    int i;
    for(i = ST.length - 1;ST.Record[i].key!=key;--i) {
        (*c)++;
    }
    // 恢复第一个元素的原始 key 值
    ST.Record[0].key = originalKey;

    // 如果没找到，返回-1
    return (ST.Record[i].key == key) ? i : -1;
}




// 4.排序函数
// 将静态查找表所有数据元素按关键字大小递增排序（有序表）
Status SortSSTable(SSTable *ST) {

    int t1;
    const char *t2;
    char t3;
    int t4;

    // 冒泡排序
    for(int j=0;j<ST->length-1;j++){
        for(int i=0;i<ST->length-1-j;i++){
            if(ST->Record[i].key>ST->Record[i+1].key){
                t1=ST->Record[i+1].key;
                t2=ST->Record[i+1].name;
                t3=ST->Record[i+1].sex;
                t4=ST->Record[i+1].age;
                ST->Record[i+1].key=ST->Record[i].key;
                ST->Record[i+1].name=ST->Record[i].name;
                ST->Record[i+1].sex=ST->Record[i].sex;
                ST->Record[i+1].age=ST->Record[i].age;
                ST->Record[i].key=t1;
                ST->Record[i].name=t2;
                ST->Record[i].sex=t3;
                ST->Record[i].age=t4;
            }
        }
    }
    return OK;
}




// 5.折半查找函数
// 其中参数key为要查找的关键字，参数c保存查找过程中关键字比较的次数。
// 若查找成功，函数返回该关键字元素所在的下标，否则，查找不成功，返回-1。
int SearchSSTable_Bin(SSTable ST, KeyType key, int *c) {
    int low=1;
    int mid;
    int high=ST.length;
    while(low<=high){
        mid=(low+high)/2;
        c++;
        if(key==ST.Record[mid].key){
            return mid;
        }
        else if(key<ST.Record[mid].key) {
            high=mid-1;
        }
        else low=mid+1;
    }

    return -1;
}




int main() {
    int i,key,key2;

    int n=10,total=0;


    SSTable ST1;
    ST1.Record=NULL;

    // 创建静态查找表ST1
    CreateSSTable(&ST1,n);
    printf("\n学号无序的静态查找表\n");

    // 遍历
    TraversSSTable(ST1);

    // 查找
    int times1,times2;

    printf("-------顺序查找-------\n");
    printf("\n输入 -1 可退出\n请输入要查找的学号：");
    scanf("%d",&key);
    while (key!=-1){

        times1=0;
        i = SearchSSTable_Seq(ST1,key,&times1);

        if(i == -1) {
            printf("当前要查找的记录不存在！\n");
            printf("查找次数：");
            printf("%d\n",times1);
        }
        else {
            printf("%d ",ST1.Record[i].key);
            printf("%s ",ST1.Record[i].name);
            printf("%c ",ST1.Record[i].sex);
            printf("%d \n",ST1.Record[i].age);
            printf("查找次数：");
            printf("%d\n",times1);

            total += times1;  // 累加成功查找的比较次数
        }

        printf("查找时比较的次数=%d\n",n-i+1);
        printf("\n请继续输入要查找的学号，或者输入-1退出：\n"); //下一次查找

        // 再次获取输入
        scanf("%d",&key);
    }

    printf("\n顺序查找各关键字的比较次数：\n");

    // 排序并遍历输出
    printf("--------排序并输出-------\n");
    SortSSTable(&ST1);
    TraversSSTable(ST1);

    // 查找
    printf("---------------\n");
    printf("请输入关键字进行查找：");

    while(scanf("%d",&key2)&&key2!=-1){

        times2=0;
        int m=SearchSSTable_Seq(ST1,key2,&times2);
        if(m == -1){
            printf("当前要查找的元素记录不存在！\n");
            printf("查找次数：");
            printf("%d\n",times2);
        }
        else{
            printf("%d ",ST1.Record[m].key);
            printf("%s ",ST1.Record[m].name);
            printf("%c ",ST1.Record[m].sex);
            printf("%d \n",ST1.Record[m].age);
            printf("查找次数：");
            printf("%d\n",times2);

            total += times2;  // 累加成功查找的比较次数

            printf("\n请继续输入要查找的关键字，或者输入-1退出：\n"); //下一次查找
        }

    }
    printf("\n\n查找成功的平均查找长度ASL=%f\n",(float)total/ST1.length);
    printf("\n查找不成功的平均查找长度ASL=%f\n",(float)ST1.length+1);

}


















