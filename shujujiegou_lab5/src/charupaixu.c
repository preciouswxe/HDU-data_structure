//
// Created by 13561 on 2025/1/2.
//
#include<stdio.h>

#define Status int
#define OK 1
#define MAXSIZE 20         //��¼���������ֵ

typedef int  KeyType;          //����ؼ�������Ϊ��������
int  ccount = 0;       //�ؼ��ֵıȽϴ��� ȫ�ֱ���
int  mcount = 0;       //��¼���ƶ�����


//��¼����
typedef struct  {
    KeyType  key;         //ѧ�ţ���¼�Ĺؼ��֣�
    const char *name;     //����
    const char *sex;      //�Ա�
    int  age;             //����
} RecordType;

//��¼�������
typedef struct{
    RecordType  r[MAXSIZE+1];           //r[0]���û��������ڱ�����Ԫ
    int length;                         //��¼�ĸ���
}RecordTable;

Status CreateRecordTable(RecordTable *L) {	//����˳���

    int keys[]={56,19,80, 5,21,64,88,13,37,75,92};

    //int keys[]={25,12,9,20,7,31,24,35,17,10,5};

    const char *names[]={"Zhang","Wang","ZHou","Huang","Zheng","Li","Liu","Qian","Sun","Zhao","Chen"};

    const char *sexs[]={"F","F","F","F","M","M","M","M","M","M","M"};

    int ages[]={19,18,19,18,19,20,20,19,18,19,18};

    int i,n=11;

    // �����ڱ� ��1�±꿪ʼ��
    for(i=1;i<=n;i++){
        L->r[i].key=keys[i-1];
        L->r[i].name=names[i-1];
        L->r[i].sex=sexs[i-1];
        L->r[i].age=ages[i-1];
    }

    L->length=n;

    return OK;

}



Status OutRecordTable(RecordTable L){ 	//���˳���ĸ�����¼

    int i;

    printf("ѧ�� ���� �Ա� ����\n");

    for(i=1;i<=L.length;i++){
        printf(" %2d ",L.r[i].key);
        printf("%5s   ",L.r[i].name);
        printf("%1s   ",L.r[i].sex);
        printf("%2d\n",L.r[i].age);
    }
    return OK;
}

// ֱ�Ӳ�������
Status InsertSort(RecordTable *RT) {
    // ��һ��Ԫ�����±� 1
    for(int i = 2;i<RT->length;i++) {
        // ��ʱ����temp
        RecordType temp = RT->r[i];
        int j = i-1;
        // �Ƚ�
        while(j >= 1 && RT->r[j].key > temp.key) {
            // ��С������ ��Ҫ�������С �����Ѿ��źõĺ���
            RT->r[j+1] = RT->r[j];
            j--;
            // �ƶ�����
            mcount++;
        }
        // ������temp Ҳ����Ҫ���������
        RT->r[j+1] = temp;
        // �Ƚϴ���
        ccount++;
    }
    return OK;
}

// ϣ������
Status ShellSort(RecordTable *RT) {
    int d = RT->length / 2;
    while(d>=1) {
        for(int i = d + 1 ;i<RT->length;i++) {
            // ��ʱ����temp
            RecordType temp = RT->r[i];
            int j = i - d;
            // �Ƚ�
            while(j >= d && RT->r[j].key > temp.key) {
                // ��С������ ��Ҫ�������С �����Ѿ��źõĺ���
                RT->r[j + d] = RT->r[j];
                j--;
                // �ƶ�����
                mcount++;
            }
            // ������temp Ҳ����Ҫ���������
            RT->r[j + d] = temp;
            // �Ƚϴ���
            ccount++;
        }
        d /= 2;  // ÿ�μ���
        printf("\n d = %d, �����ļ�¼��: \n", d);
        OutRecordTable(*RT);  // ���ÿ�������ļ�¼��
    }
    return OK;
}

int main() {
    RecordTable L;
    CreateRecordTable(&L);
    printf("------���˳���------\n");
    OutRecordTable(L);

    InsertSort(&L);
    printf("------���ֱ�Ӳ���������------\n");
    OutRecordTable(L);

    printf("�ؼ��ֱȽϴ���ccount = %d\n",ccount);
    printf("�ƶ�����mcount = %d\n",mcount);

    // ����ȽϺ��ƶ�����
    ccount = 0;
    mcount = 0;

    // ���´���
    CreateRecordTable(&L);
    printf("------���ϣ��������------\n");
    ShellSort(&L);


    printf("�ؼ��ֱȽϴ���ccount = %d\n",ccount);
    printf("�ƶ�����mcount = %d\n",mcount);


}