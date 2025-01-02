//
// Created by 13561 on 2025/1/2.
//
#include<stdio.h>
#define Status int
#define MAXSIZE 20            //文件中记录个数的最大值
#define OK 1
typedef int KeyType;          //定义关键字类型为整数类型

//记录类型
typedef struct  {

    KeyType  key;             //学号（记录的关键字）
    const char *name;         //姓名
    const char *sex;          //性别
    int  age;                 //年龄
} RecordType;

//记录表的类型
typedef struct{

    RecordType  r[MAXSIZE+1];      //r[0]闲置或用作 “哨兵” 单元
    int length;                    //记录的个数

}RecordTable;

int ccount = 0,mcount = 0,pcount = 0;     //全局变量，ccount关键字比较次数，mcount记录移到次数


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


// 冒泡排序
Status BubbleSort(RecordTable *RT) {
    for(int i = 1;i<RT->length;i++) {
        for(int j = 1; j <= RT->length - i;j++) {
            ccount++; // 每次比较都增加一次
            if(RT->r[j].key > RT->r[j+1].key) {
                RecordType temp = RT->r[j];
                RT->r[j] = RT->r[j+1];
                RT->r[j+1] = temp;
                mcount++;    // 移动次数加1
            }
        }
    }
}




// 快速排序
Status QuickSort(RecordTable *RT, int low, int high) {
    if (low >= high) return;

    // 快速排序的划分过程
    int pivot = RT->r[low].key;  // 选取基准
    int left = low + 1;
    int right = high;

    while (1) {
        // 从左到右，找到大于基准的元素
        while (left <= right && RT->r[left].key <= pivot) {
            left++;
            ccount++;
        }

        // 从右到左，找到小于基准的元素
        while (left <= right && RT->r[right].key >= pivot) {
            right--;
            ccount++;
        }

        if (left > right) break;  // 如果两指针交叉，则退出
        // 交换左指针和右指针所指向的记录
        RecordType temp = RT->r[left];
        RT->r[left] = RT->r[right];
        RT->r[right] = temp;
        mcount++;
    }

    // 将基准放到正确的位置
    RecordType temp = RT->r[low];
    RT->r[low] = RT->r[right];
    RT->r[right] = temp;
    mcount++;

    // 输出当前划分后的记录表
    printf("\n划分%d后的记录表：\n", ++pcount);
    OutRecordTable(*RT);

    // 递归排序基准左右两侧的部分
    QuickSort(RT, low, right - 1);
    QuickSort(RT, right + 1, high);
}









int main() {
    RecordTable RT1;

    // 创建记录表
    CreateRecordTable(&RT1);
    printf("------输出顺序表------\n");
    OutRecordTable(RT1);

    // 进行冒泡排序
    BubbleSort(&RT1);
    printf("\n-------冒泡排序后的记录表-------\n");
    OutRecordTable(RT1);
    printf("关键字比较次数：%d\n", ccount);
    printf("记录交换次数：%d\n", mcount);

    // 重置记录表
    CreateRecordTable(&RT1);

    printf("\n--------开始快速排序----------\n");

    // 进行快速排序
    QuickSort(&RT1, 1, RT1.length);
    printf("\n-------快速排序后的记录表------\n");
    OutRecordTable(RT1);
    printf("关键字比较次数：%d\n", ccount);
    printf("记录交换次数：%d\n", mcount);

    return 0;
}