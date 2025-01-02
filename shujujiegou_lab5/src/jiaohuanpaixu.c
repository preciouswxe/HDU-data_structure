//
// Created by 13561 on 2025/1/2.
//
#include<stdio.h>
#define Status int
#define MAXSIZE 20            //�ļ��м�¼���������ֵ
#define OK 1
typedef int KeyType;          //����ؼ�������Ϊ��������

//��¼����
typedef struct  {

    KeyType  key;             //ѧ�ţ���¼�Ĺؼ��֣�
    const char *name;         //����
    const char *sex;          //�Ա�
    int  age;                 //����
} RecordType;

//��¼�������
typedef struct{

    RecordType  r[MAXSIZE+1];      //r[0]���û����� ���ڱ��� ��Ԫ
    int length;                    //��¼�ĸ���

}RecordTable;

int ccount = 0,mcount = 0,pcount = 0;     //ȫ�ֱ�����ccount�ؼ��ֱȽϴ�����mcount��¼�Ƶ�����


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


// ð������
Status BubbleSort(RecordTable *RT) {
    for(int i = 1;i<RT->length;i++) {
        for(int j = 1; j <= RT->length - i;j++) {
            ccount++; // ÿ�αȽ϶�����һ��
            if(RT->r[j].key > RT->r[j+1].key) {
                RecordType temp = RT->r[j];
                RT->r[j] = RT->r[j+1];
                RT->r[j+1] = temp;
                mcount++;    // �ƶ�������1
            }
        }
    }
}




// ��������
Status QuickSort(RecordTable *RT, int low, int high) {
    if (low >= high) return;

    // ��������Ļ��ֹ���
    int pivot = RT->r[low].key;  // ѡȡ��׼
    int left = low + 1;
    int right = high;

    while (1) {
        // �����ң��ҵ����ڻ�׼��Ԫ��
        while (left <= right && RT->r[left].key <= pivot) {
            left++;
            ccount++;
        }

        // ���ҵ����ҵ�С�ڻ�׼��Ԫ��
        while (left <= right && RT->r[right].key >= pivot) {
            right--;
            ccount++;
        }

        if (left > right) break;  // �����ָ�뽻�棬���˳�
        // ������ָ�����ָ����ָ��ļ�¼
        RecordType temp = RT->r[left];
        RT->r[left] = RT->r[right];
        RT->r[right] = temp;
        mcount++;
    }

    // ����׼�ŵ���ȷ��λ��
    RecordType temp = RT->r[low];
    RT->r[low] = RT->r[right];
    RT->r[right] = temp;
    mcount++;

    // �����ǰ���ֺ�ļ�¼��
    printf("\n����%d��ļ�¼��\n", ++pcount);
    OutRecordTable(*RT);

    // �ݹ������׼��������Ĳ���
    QuickSort(RT, low, right - 1);
    QuickSort(RT, right + 1, high);
}









int main() {
    RecordTable RT1;

    // ������¼��
    CreateRecordTable(&RT1);
    printf("------���˳���------\n");
    OutRecordTable(RT1);

    // ����ð������
    BubbleSort(&RT1);
    printf("\n-------ð�������ļ�¼��-------\n");
    OutRecordTable(RT1);
    printf("�ؼ��ֱȽϴ�����%d\n", ccount);
    printf("��¼����������%d\n", mcount);

    // ���ü�¼��
    CreateRecordTable(&RT1);

    printf("\n--------��ʼ��������----------\n");

    // ���п�������
    QuickSort(&RT1, 1, RT1.length);
    printf("\n-------���������ļ�¼��------\n");
    OutRecordTable(RT1);
    printf("�ؼ��ֱȽϴ�����%d\n", ccount);
    printf("��¼����������%d\n", mcount);

    return 0;
}