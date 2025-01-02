//
// Created by 13561 on 2025/1/2.
//
#include<stdio.h>

#define Status int
#define OK 1
#define MAXSIZE 20         //记录个数的最大值

typedef int  KeyType;          //定义关键字类型为整数类型
int  ccount = 0;       //关键字的比较次数 全局变量
int  mcount = 0;       //记录的移动次数


//记录类型
typedef struct  {
    KeyType  key;         //学号（记录的关键字）
    const char *name;     //姓名
    const char *sex;      //性别
    int  age;             //年龄
} RecordType;

//记录表的类型
typedef struct{
    RecordType  r[MAXSIZE+1];           //r[0]闲置或用作“哨兵”单元
    int length;                         //记录的个数
}RecordTable;

Status CreateRecordTable(RecordTable *L) {	//创建顺序表

    int keys[]={56,19,80, 5,21,64,88,13,37,75,92};

    //int keys[]={25,12,9,20,7,31,24,35,17,10,5};

    const char *names[]={"Zhang","Wang","ZHou","Huang","Zheng","Li","Liu","Qian","Sun","Zhao","Chen"};

    const char *sexs[]={"F","F","F","F","M","M","M","M","M","M","M"};

    int ages[]={19,18,19,18,19,20,20,19,18,19,18};

    int i,n=11;

    // 保留哨兵 从1下标开始放
    for(i=1;i<=n;i++){
        L->r[i].key=keys[i-1];
        L->r[i].name=names[i-1];
        L->r[i].sex=sexs[i-1];
        L->r[i].age=ages[i-1];
    }

    L->length=n;

    return OK;

}



Status OutRecordTable(RecordTable L){ 	//输出顺序表的各个记录

    int i;

    printf("学号 姓名 性别 年龄\n");

    for(i=1;i<=L.length;i++){
        printf(" %2d ",L.r[i].key);
        printf("%5s   ",L.r[i].name);
        printf("%1s   ",L.r[i].sex);
        printf("%2d\n",L.r[i].age);
    }
    return OK;
}

// 直接插入排序
Status InsertSort(RecordTable *RT) {
    // 第一个元素在下标 1
    for(int i = 2;i<RT->length;i++) {
        // 临时变量temp
        RecordType temp = RT->r[i];
        int j = i-1;
        // 比较
        while(j >= 1 && RT->r[j].key > temp.key) {
            // 从小到大排 若要插入的数小 则让已经排好的后移
            RT->r[j+1] = RT->r[j];
            j--;
            // 移动次数
            mcount++;
        }
        // 最后放下temp 也就是要来插入的数
        RT->r[j+1] = temp;
        // 比较次数
        ccount++;
    }
    return OK;
}

// 希尔排序
Status ShellSort(RecordTable *RT) {
    int d = RT->length / 2;
    while(d>=1) {
        for(int i = d + 1 ;i<RT->length;i++) {
            // 临时变量temp
            RecordType temp = RT->r[i];
            int j = i - d;
            // 比较
            while(j >= d && RT->r[j].key > temp.key) {
                // 从小到大排 若要插入的数小 则让已经排好的后移
                RT->r[j + d] = RT->r[j];
                j--;
                // 移动次数
                mcount++;
            }
            // 最后放下temp 也就是要来插入的数
            RT->r[j + d] = temp;
            // 比较次数
            ccount++;
        }
        d /= 2;  // 每次减半
        printf("\n d = %d, 排序后的记录表: \n", d);
        OutRecordTable(*RT);  // 输出每趟排序后的记录表
    }
    return OK;
}

int main() {
    RecordTable L;
    CreateRecordTable(&L);
    printf("------输出顺序表------\n");
    OutRecordTable(L);

    InsertSort(&L);
    printf("------输出直接插入排序结果------\n");
    OutRecordTable(L);

    printf("关键字比较次数ccount = %d\n",ccount);
    printf("移动次数mcount = %d\n",mcount);

    // 清零比较和移动次数
    ccount = 0;
    mcount = 0;

    // 重新创建
    CreateRecordTable(&L);
    printf("------输出希尔排序结果------\n");
    ShellSort(&L);


    printf("关键字比较次数ccount = %d\n",ccount);
    printf("移动次数mcount = %d\n",mcount);


}