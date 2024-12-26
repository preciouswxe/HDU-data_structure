//
// Created by 13561 on 2024/12/26.
//
#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define OK 1;
typedef int KeyType;        //�ؼ��ֵ���������
typedef int Status;

//����Ԫ�أ���¼�������Ͷ���
typedef struct {
    KeyType  key;            //ѧ��(�ؼ���)
    const char *name;        //����
    char sex;                //�Ա�
    int  age;                //����
} RecordType;

//��̬���ұ�����Ͷ���
typedef struct {
    RecordType *Record;      //����Ԫ�ش洢�ռ�Ļ�ַ(һά������)
    int    length;             //��ĳ��ȣ�Ԫ�ظ�����
}SSTable;

// ��ĿҪ��
int key[11]={56,19,80,5,21,64,88,13,37,75,92};
const char name[11][11]={"Zhang","Wang","Zhou","Huang","Zheng","Li","Liu","Qian","Sun","Zhao","Chen"};
char sex[11]={'F','F','F','M','M','M','F','F','M','M','M'};               //name ���ַ���ָ�룬�� sex �ǵ����ַ���
int age[11]={19,20,19,20,20,19,18,19,20,20,20};


// 1.������̬���ұ�ĺ���
Status CreateSSTable(SSTable *ST, int n) {
    // ��������ռ�
    ST->Record=(RecordType *)malloc((n+1)* sizeof(RecordType));
    for(int i = 0;i<n;i++) {
        ST->Record[i].key = key[i];
        ST->Record[i].name= name[i];
        ST->Record[i].sex = sex[i];
        ST->Record[i].age = age[i];
    }

    ST->length = n;  // ���ñ�ĳ���
    return OK;
}


// 2.���������̬���ұ�ST����������Ԫ��
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



// 3.˳����Һ���
// ���в���keyΪҪ���ҵĹؼ��֣�����c������ҹ����йؼ��ֱȽϵĴ�����
// �����ҳɹ����������ظùؼ���Ԫ�����ڵ��±꣬���򣬲��Ҳ��ɹ�������0���� ���ڱ�������-1���� ���ڱ�������
int SearchSSTable_Seq(SSTable ST, KeyType key, int *c) {
    // �����һ��Ԫ�ؾ���Ŀ��Ԫ�أ�ֱ�ӷ���
    if (ST.Record[0].key == key) {
        return 0; // �ҵ������
    }

    // �����һ��Ԫ�ص� key�������޸�
    KeyType originalKey = ST.Record[0].key;

    ST.Record[0].key = key; // ʹ���ڱ����ƣ���key�ŵ���һ��Ԫ��
    int i;
    for(i = ST.length - 1;ST.Record[i].key!=key;--i) {
        (*c)++;
    }
    // �ָ���һ��Ԫ�ص�ԭʼ key ֵ
    ST.Record[0].key = originalKey;

    // ���û�ҵ�������-1
    return (ST.Record[i].key == key) ? i : -1;
}




// 4.������
// ����̬���ұ���������Ԫ�ذ��ؼ��ִ�С�������������
Status SortSSTable(SSTable *ST) {

    int t1;
    const char *t2;
    char t3;
    int t4;

    // ð������
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




// 5.�۰���Һ���
// ���в���keyΪҪ���ҵĹؼ��֣�����c������ҹ����йؼ��ֱȽϵĴ�����
// �����ҳɹ����������ظùؼ���Ԫ�����ڵ��±꣬���򣬲��Ҳ��ɹ�������-1��
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

    // ������̬���ұ�ST1
    CreateSSTable(&ST1,n);
    printf("\nѧ������ľ�̬���ұ�\n");

    // ����
    TraversSSTable(ST1);

    // ����
    int times1,times2;

    printf("-------˳�����-------\n");
    printf("\n���� -1 ���˳�\n������Ҫ���ҵ�ѧ�ţ�");
    scanf("%d",&key);
    while (key!=-1){

        times1=0;
        i = SearchSSTable_Seq(ST1,key,&times1);

        if(i == -1) {
            printf("��ǰҪ���ҵļ�¼�����ڣ�\n");
            printf("���Ҵ�����");
            printf("%d\n",times1);
        }
        else {
            printf("%d ",ST1.Record[i].key);
            printf("%s ",ST1.Record[i].name);
            printf("%c ",ST1.Record[i].sex);
            printf("%d \n",ST1.Record[i].age);
            printf("���Ҵ�����");
            printf("%d\n",times1);

            total += times1;  // �ۼӳɹ����ҵıȽϴ���
        }

        printf("����ʱ�ȽϵĴ���=%d\n",n-i+1);
        printf("\n���������Ҫ���ҵ�ѧ�ţ���������-1�˳���\n"); //��һ�β���

        // �ٴλ�ȡ����
        scanf("%d",&key);
    }

    printf("\n˳����Ҹ��ؼ��ֵıȽϴ�����\n");

    // ���򲢱������
    printf("--------�������-------\n");
    SortSSTable(&ST1);
    TraversSSTable(ST1);

    // ����
    printf("---------------\n");
    printf("������ؼ��ֽ��в��ң�");

    while(scanf("%d",&key2)&&key2!=-1){

        times2=0;
        int m=SearchSSTable_Seq(ST1,key2,&times2);
        if(m == -1){
            printf("��ǰҪ���ҵ�Ԫ�ؼ�¼�����ڣ�\n");
            printf("���Ҵ�����");
            printf("%d\n",times2);
        }
        else{
            printf("%d ",ST1.Record[m].key);
            printf("%s ",ST1.Record[m].name);
            printf("%c ",ST1.Record[m].sex);
            printf("%d \n",ST1.Record[m].age);
            printf("���Ҵ�����");
            printf("%d\n",times2);

            total += times2;  // �ۼӳɹ����ҵıȽϴ���

            printf("\n���������Ҫ���ҵĹؼ��֣���������-1�˳���\n"); //��һ�β���
        }

    }
    printf("\n\n���ҳɹ���ƽ�����ҳ���ASL=%f\n",(float)total/ST1.length);
    printf("\n���Ҳ��ɹ���ƽ�����ҳ���ASL=%f\n",(float)ST1.length+1);

}


















