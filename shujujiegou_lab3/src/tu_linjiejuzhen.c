//
// Created by 13561 on 2024/12/5.
//
#include<stdio.h>

#define INFINITY 32767                   //��ʾ���ֵ��
#define MAX_VERTEX_NUM  20       //��󶥵���
#define OK 1

typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //ͼ��ö������   ��U��������ͼ��������������ͼ,G��ͼ��N����
typedef int VRType;
typedef char InfoType;
typedef char VertexType;

// �ڽӾ���
typedef struct ArcCell {

    VRType adj  ;          //����Ȩͼ��1��0��ʾ���Դ�Ȩͼ����ΪȨֵ���͡�
    InfoType *info;       //�ñ߻������Ϣ��ָ�루�ַ�����

} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

// ͼ ������ ���� �������ͻ��� ͼ���
typedef struct{

    VertexType vexs[MAX_VERTEX_NUM];   //�������飬��Ŷ�����Ϣ
    AdjMatrix   arcs;                  //�ڽӾ���
    int vexnum, arcnum;           //ͼ�ж��������뻡��
    GraphKind  kind;                 //ͼ�������־

} MGraph;

/*
 * GraphKind GKind: ͼ������ DG,DN,UDG,UDN
 * MGraph G: ͼ���������ͼ�Ķ������顢�ڽӾ��������
 * int vexnum: ͼ�Ķ�����
 * int arcnum: ͼ�ı߻���
 * char *vexs: �������飬���ͼ�Ķ���
 * int *arcs: ��(��)���飬ÿ���ߣ�����������Ԫ����ʽ���(i, j, w)���ֱ��ʾ�ߵ��ڽӶ����Ȩֵ�򻡵Ļ�β�� ��ͷ��Ȩֵ��������Ȩֵ��ͼ��Ȩֵ=1��
 * �ò�����ʵ���ö�ά����洢���β���һά�������ʽ���ʡ�
 * ��ʵ��ʱҪע������ͼ�������ǶԳƾ�������ͼ�������ǷǶԳƾ���
 */

// һ���ܹ�����4�ֲ�ͬ����ͼ���ڽӾ���洢��ͨ�ú���
Status CreateMGraph(GraphKind GKind, MGraph *G, int vexnum, int arcnum, char *vexs, int *arcs) {
    G->kind = GKind;
    G->vexnum = vexnum;
    G->arcnum = arcnum;

    // ��ʼ����������
    for(int i = 0; i < vexnum ;i++) {
        G->vexs[i] = vexs[i];
    }

    // ��ʼ���ڽӾ���
    for(int i = 0; i < vexnum;i++) {
        for(int j = 0;j < vexnum;j++) {
            // �� ��ʼ�������
            if(GKind == DN || GKind == UDN) {
                G->arcs[i][j].adj = INFINITY;
            }else {
                // ͼ ��ʼ��0
                G->arcs[i][j].adj = 0;
            }
            G->arcs[i][j].info = NULL;
        }
    }

    // ���ݱߣ�������Ϣ����ڽӾ���
    // ������Ԫ�鲢��ֵ ��һά�ıߣ���������arcs��������ȡ��ÿ���ߣ�������Ӧ����������������i��j���Լ�Ȩֵ��w��
    for (int k = 0; k < arcnum; ++k) {
        int i = arcs[3 * k];
        int j = arcs[3 * k + 1];
        int w = arcs[3 * k + 2];

        G->arcs[i][j].adj = w;
        if (GKind == UDG || GKind == UDN) {
            // ��������ͼ���������Գ�λ��ҲҪ��ֵ
            G->arcs[j][i].adj = w;
        }
    }

    return OK;
}

// ���ͼG�Ķ�������Ԫ�غ��ڽӾ��󣬶�����Ȩͼ��Ԫ����1��0��ʾ��������Ȩͼ���������ߵ�Ȩֵ��������ʾ�������áޱ�ʾ��
void OutMGraph(MGraph G) {
    // �����������Ԫ��
    printf("��������Ԫ�أ�");
    for (int i = 0; i < G.vexnum; ++i) {
        printf("%c ", G.vexs[i]);
    }
    printf("\n");

    // ����ڽӾ���
    printf("�ڽӾ���\n");
    for (int i = 0; i < G.vexnum; ++i) {
        for (int j = 0; j < G.vexnum; ++j) {
            if (G.arcs[i][j].adj == INFINITY) {
                printf("�� ");
            } else {
                printf("%d ", G.arcs[i][j].adj);
            }
        }
        printf("\n");
    }
}


// ���ͼG�и�����Ķȣ�����ͼ����Ⱥͳ��ȣ�
Status VerDegree(MGraph G) {
    // ����ͼ
    // �����ǳ� ��������
    if(G.kind == DG || G.kind == DN) {
        int indegree[MAX_VERTEX_NUM] = {0};
        int outdegree[MAX_VERTEX_NUM] = {0};

        for(int j = 0;j < G.vexnum;j++) {
            for(int i = 0; i < G.vexnum;i++) {
                if(G.arcs[i][j].adj != INFINITY) {
                    indegree[j]++;
                }
                if(G.arcs[j][i].adj != INFINITY) {
                    outdegree[j]++;
                }
            }
        }

        printf("���������Ⱥͳ���������£�\n");
        for (int k = 0; k < G.vexnum; ++k) {
            printf("���� %c ����ȣ�%d�����ȣ�%d\n", G.vexs[k], indegree[k], outdegree[k]);
        }

    }else {
        // ����ͼ
        int degree[MAX_VERTEX_NUM] = {0};
        for (int i = 0; i < G.vexnum; ++i) {
            for (int j = 0; j < G.vexnum; ++j) {
                if (G.arcs[i][j].adj != 0) {
                    degree[i]++;
                }
            }
        }

        printf("������Ķ�������£�\n");
        for (int k = 0; k < G.vexnum; ++k) {
            printf("���� %c �Ķȣ�%d\n", G.vexs[k], degree[k]);
        }


    }
    return OK;
}


int main() {
    MGraph G;
    //�������������ͼ
    char UDGvex[20]={'A','B','C','D'};
    int UDGarcs[20][3]={0,1,1,0,2,1,0,3,1,1,2,1,2,3,1};
    CreateMGraph(UDG,&G,4,5,UDGvex,&UDGarcs[0][0]);
    printf("(1)����ͼG\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");

    //�������������ͼ
    char DGvex[20]={'A','B','C','D'};
    int DGarcs[20][3]={0,1,1,1,0,1,1,2,1,2,0,1,2,1,1,3,1,1};
    CreateMGraph(DG,&G,4,6,DGvex,&DGarcs[0][0]);
    printf("(2)����ͼ");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");

    //�������������ͼ
    char UDNvex[20]={'A','B','C','D'};
    int UDNarcs[20][3]={0,1,56,0,2,34,0,3,78,2,3,25};
    CreateMGraph(UDN,&G,4,4,UDNvex,&UDNarcs[0][0]);
    printf("(3)������");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");

    //���������������
    char DNvex[20]={'A','B','C'};
    int DNarcs[20][3]={0,2,50,1,2,45,2,0,64};
    CreateMGraph(DN,&G,3,3,DNvex,&DNarcs[0][0]);
    printf("(4)������G\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    return 0;
}
































