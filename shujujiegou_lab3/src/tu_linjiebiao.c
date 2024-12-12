//
// Created by 13561 on 2024/12/5.
//
#include<stdio.h>
#include<stdlib.h>
#define MAX_VERTEX_NUM  20//��󶥵���

typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //ͼ������
typedef char VertexType; //�������ݵ�����(�ַ���

//��(��)�����������
typedef struct ArcNode {
    int    adjvex ;       //�ñ�(��)������(ָ��)�Ķ����ڶ���������±�
    struct ArcNode *nextarc;       //ָ����һ����(��)��ָ��
    int    weight;        //��(��)��Ȩֵ����Ȩͼ��ֵΪ0
}ArcNode;

//����������������
typedef struct VNode {
    VertexType  data;          //��������
    ArcNode  *firstarc;         //ָ���һ�������ö���ı�(��)��ָ��
}VNode, AdjList[MAX_VERTEX_NUM];

//ͼ���ڽӱ���������
typedef  struct {
    AdjList   vertices;                   //ͼ�Ķ�������
    int         vexnum, arcnum;     //ͼ�Ķ������ͱ�(��)��
    GraphKind kind;                    //ͼ������
}ALGraph;

/*
��������˵�����£�
GraphKind GKind: ͼ������ DG,DN,UDG,UDN
MGraph G: ͼ���������ͼ�Ķ������顢�ڽӱߣ�����������
int vexnum: ͼ�Ķ�����
int arcnum: ͼ�ı߻���
char *vexs: �������飬���ͼ�Ķ���
int *arcs:  ��(��)���飬ÿ���ߣ�����������Ԫ����ʽ���(i, j, w)���ֱ��ʾ�ߵ��ڽӶ����Ȩֵ�򻡵Ļ�β����ͷ��Ȩֵ��������Ȩֵ��ͼ��Ȩֵ=1���ò�����ʵ���ö�ά����洢���β���һά�������ʽ���ʡ�
��ʵ��ʱҪע������ͼ������ÿ����Ҫ���������߽�㣬������ͼ�������ǳ��߱�
*/

// һ���ܹ�����4�ֲ�ͬ����ͼ���ڽӱ�洢��ͨ�ú���
void CreateALGraph(GraphKind GKind, ALGraph *G, int vexnum, int arcnum, char *vexs, int *arcs) {
    G->kind = GKind;
    G->vexnum = vexnum;
    G->arcnum = arcnum;

    int i , j;
    // ��ʼ����������
    // ������Ķ����ʶ��ֵ������������ÿ����������ݳ�Ա��ͬʱ��ÿ������ĵ�һ����ָ���ʼ��ΪNULL
    for(i = 0;i < vexnum;i++) {
        G->vertices[i].data = vexs[i];
        G->vertices[i].firstarc=NULL;
    }

    // ��ʼ������ĸ�����
    ArcNode *s;
    for(int k = 0;k< arcnum;k++) {
        // ��ȡ�ߣ�������Ԫ���еĵ�һ��Ԫ�أ����ߣ�������ʼ�����ڶ��������е��±�
        i = arcs[3*k];
        // ��ȡ�ߣ�������Ԫ���еĵڶ���Ԫ�أ����ߣ�������ֹ�����ڶ��������е��±�
        j = arcs[3*k+1];

        s = (ArcNode *)malloc(sizeof(ArcNode));
        // ���ñߣ������ڵ���ָ����ڽӶ����±�
        s -> adjvex = j;
        // ���½ڵ���뵽��ǰ��ʼ������ڽӱ�ͷ����ʹ���Ϊ��һ���ߣ�����
        s -> nextarc = G->vertices[i].firstarc;

        // Ȩ������Ԫ��ĵ�����
        s -> weight = arcs[3*k+2];
        G -> vertices[i].firstarc = s;

        // ���������ͼ����Ҫ�ٴ���һ������ıߣ������ڵ㣬��Ϊ����ͼ�ı���˫���
        if(GKind == UDN || GKind == UDG) {
            s = (ArcNode *)malloc(sizeof(ArcNode));
            s -> adjvex = i;
            s -> nextarc = G->vertices[j].firstarc;
            s -> weight = arcs[3*k+2];
            G->vertices[j].firstarc = s;
        }
    }
}


// ���ͼG�Ķ�������Ԫ�غ��ڽӱ�ṹ���ݣ�������Ȩͼ��������Ȩֵ�����ű�ʾ��
void OutMGraph(ALGraph G) {
    int i;
    ArcNode *p;

    printf("���㼯��");
    for(i=0;i<G.vexnum;i++) {
        printf("%c ",G.vertices[i].data);
    }
    printf("\n");

    printf("�ڽӱ����£�\n");
    for(i = 0;i<G.vexnum;i++) {
        printf("%c ",G.vertices[i].data);
        p = G.vertices[i].firstarc;
        while(p) {
            printf("->%d",p->adjvex);
            if(G.kind==UDN || G.kind==DN) {
                // ��ĿҪ��
                printf("(%d)",p->weight);
            }
            p = p->nextarc;
        }
        printf("\n");
    }

}


// ���ͼG�и�����Ķȣ�����ͼ����Ⱥͳ��ȣ�
void VerDegree(ALGraph G) {
    int i,j,c;
    ArcNode *p;
    if(G.kind == UDG || G.kind == UDN) {
        printf("����ͼ������Ķȣ�\n");
    }else {
        printf("������ĳ��ȣ�\n");
    }

    for(i = 0;i<G.vexnum;i++) {
        printf("%c:",G.vertices[i].data);
        // ����
        c = 0;
        p = G.vertices[i].firstarc;
        while(p) {
            c++;
            p = p->nextarc;
        }
        printf("%d\n",c);
    }

    // ����ͼ�ֳ������
    if(G.kind==DG||G.kind==DN) {
        printf("���������ȣ�\n");
        for(i=0;i<G.vexnum;i++) {
            printf("%c:",G.vertices[i].data);
            c=0;

            for(j=0;j<G.vexnum;j++) {
                // ��ȡ��ǰ����ĵ�һ���ߣ�����ָ��
                p = G.vertices[j].firstarc;
                // ������ǰ������ڽӱ�ÿ��һ���ߣ�������ȣ����ȣ�������1
                while(p) {
                    if(p->adjvex == i) {
                        //����
                        c++;
                    }
                    p=p->nextarc;
                }
            }
            printf("%d\n",c);
        }
    }
}



int main() {
    ALGraph G;
    char UDGvex[20]={'A','B','C','D'};
    int UDGarcs[20][3]={0,1,1,0,2,1,0,3,1,1,2,1,2,3,1};
    CreateALGraph(UDG,&G,4,5,UDGvex,&UDGarcs[0][0]);
    printf("(1)����ͼG1\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");

    char DGvex[20]={'A','B','C','D'};
    int DGarcs[20][3]={0,1,1,1,0,1,1,2,1,2,0,1,2,1,1,3,1,1};
    CreateALGraph(DG,&G,4,6,DGvex,&DGarcs[0][0]);
    printf("(2)����ͼG2\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");


    char UDNvex[20]={'A','B','C','D'};
    int UDNarcs[20][3]={0,1,56,0,2,34,0,3,78,2,3,25};
    CreateALGraph(UDN,&G,4,4,UDNvex,&UDNarcs[0][0]);
    printf("(3)������G3\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");


    char DNvex[20]={'A','B','C'};
    int DNarcs[20][3]={0,2,50,1,2,45,2,0,64};
    CreateALGraph(DN,&G,3,3,DNvex,&DNarcs[0][0]);
    printf("(4)����ͼ\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");


    /*
    char DNvex1[20]={'A','B','C'};
    int DNarcs1[20][3]={2,0,50,2,1,45,0,2,64};
    CreateALGraph(DN,&G,3,3,DNvex1,&DNarcs1[0][0]);
    printf("(5)������G4�����ڽӱ�\n");
    OutMGraph(G);
    printf("\n");
    */

    return 0;
}



















