//
// Created by 13561 on 2024/12/5.
//
#include<stdio.h>
#include<stdlib.h>
#define MAX_VERTEX_NUM  20//最大顶点数

typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //图的类型
typedef char VertexType; //顶点数据的类型(字符）

//边(弧)结点数据类型
typedef struct ArcNode {
    int    adjvex ;       //该边(弧)所依附(指向)的顶点在顶点数组的下标
    struct ArcNode *nextarc;       //指向下一条边(弧)的指针
    int    weight;        //边(弧)的权值，无权图其值为0
}ArcNode;

//顶点数组数据类型
typedef struct VNode {
    VertexType  data;          //顶点数据
    ArcNode  *firstarc;         //指向第一条依附该顶点的边(弧)的指针
}VNode, AdjList[MAX_VERTEX_NUM];

//图的邻接表数据类型
typedef  struct {
    AdjList   vertices;                   //图的顶点数组
    int         vexnum, arcnum;     //图的顶点数和边(弧)数
    GraphKind kind;                    //图的类型
}ALGraph;

/*
函数参数说明如下：
GraphKind GKind: 图的类型 DG,DN,UDG,UDN
MGraph G: 图变量，存放图的顶点数组、邻接边（弧）等数据
int vexnum: 图的顶点数
int arcnum: 图的边或弧数
char *vexs: 顶点数组，存放图的顶点
int *arcs:  边(弧)数组，每条边（弧）采用三元组形式存放(i, j, w)，分别表示边的邻接顶点和权值或弧的弧尾、弧头和权值，对于无权值的图，权值=1。该参数的实参用二维数组存储，形参用一维数组的形式访问。
在实现时要注意无向图（网）每条边要产生两个边结点，而有向图（网）是出边表。
*/

// 一个能够创建4种不同类型图的邻接表存储的通用函数
void CreateALGraph(GraphKind GKind, ALGraph *G, int vexnum, int arcnum, char *vexs, int *arcs) {
    G->kind = GKind;
    G->vexnum = vexnum;
    G->arcnum = arcnum;

    int i , j;
    // 初始化顶点数组
    // 将传入的顶点标识赋值给顶点数组中每个顶点的数据成员，同时将每个顶点的第一条边指针初始化为NULL
    for(i = 0;i < vexnum;i++) {
        G->vertices[i].data = vexs[i];
        G->vertices[i].firstarc=NULL;
    }

    // 开始处理顶点的各个边
    ArcNode *s;
    for(int k = 0;k< arcnum;k++) {
        // 获取边（弧）三元组中的第一个元素，即边（弧）起始顶点在顶点数组中的下标
        i = arcs[3*k];
        // 获取边（弧）三元组中的第二个元素，即边（弧）终止顶点在顶点数组中的下标
        j = arcs[3*k+1];

        s = (ArcNode *)malloc(sizeof(ArcNode));
        // 设置边（弧）节点所指向的邻接顶点下标
        s -> adjvex = j;
        // 将新节点插入到当前起始顶点的邻接表头部，使其成为第一条边（弧）
        s -> nextarc = G->vertices[i].firstarc;

        // 权重在三元组的第三个
        s -> weight = arcs[3*k+2];
        G -> vertices[i].firstarc = s;

        // 如果是无向图，需要再创建一个反向的边（弧）节点，因为无向图的边是双向的
        if(GKind == UDN || GKind == UDG) {
            s = (ArcNode *)malloc(sizeof(ArcNode));
            s -> adjvex = i;
            s -> nextarc = G->vertices[j].firstarc;
            s -> weight = arcs[3*k+2];
            G->vertices[j].firstarc = s;
        }
    }
}


// 输出图G的顶点数组元素和邻接表结构数据，对于有权图（网），权值加括号表示。
void OutMGraph(ALGraph G) {
    int i;
    ArcNode *p;

    printf("顶点集：");
    for(i=0;i<G.vexnum;i++) {
        printf("%c ",G.vertices[i].data);
    }
    printf("\n");

    printf("邻接表如下：\n");
    for(i = 0;i<G.vexnum;i++) {
        printf("%c ",G.vertices[i].data);
        p = G.vertices[i].firstarc;
        while(p) {
            printf("->%d",p->adjvex);
            if(G.kind==UDN || G.kind==DN) {
                // 题目要求
                printf("(%d)",p->weight);
            }
            p = p->nextarc;
        }
        printf("\n");
    }

}


// 输出图G中各顶点的度（有向图分入度和出度）
void VerDegree(ALGraph G) {
    int i,j,c;
    ArcNode *p;
    if(G.kind == UDG || G.kind == UDN) {
        printf("无向图各顶点的度：\n");
    }else {
        printf("各顶点的出度：\n");
    }

    for(i = 0;i<G.vexnum;i++) {
        printf("%c:",G.vertices[i].data);
        // 计数
        c = 0;
        p = G.vertices[i].firstarc;
        while(p) {
            c++;
            p = p->nextarc;
        }
        printf("%d\n",c);
    }

    // 有向图分出度入度
    if(G.kind==DG||G.kind==DN) {
        printf("各顶点的入度：\n");
        for(i=0;i<G.vexnum;i++) {
            printf("%c:",G.vertices[i].data);
            c=0;

            for(j=0;j<G.vexnum;j++) {
                // 获取当前顶点的第一条边（弧）指针
                p = G.vertices[j].firstarc;
                // 遍历当前顶点的邻接表，每有一条边（弧）则度（出度）计数加1
                while(p) {
                    if(p->adjvex == i) {
                        //计数
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
    printf("(1)无向图G1\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");

    char DGvex[20]={'A','B','C','D'};
    int DGarcs[20][3]={0,1,1,1,0,1,1,2,1,2,0,1,2,1,1,3,1,1};
    CreateALGraph(DG,&G,4,6,DGvex,&DGarcs[0][0]);
    printf("(2)有向图G2\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");


    char UDNvex[20]={'A','B','C','D'};
    int UDNarcs[20][3]={0,1,56,0,2,34,0,3,78,2,3,25};
    CreateALGraph(UDN,&G,4,4,UDNvex,&UDNarcs[0][0]);
    printf("(3)无向网G3\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");


    char DNvex[20]={'A','B','C'};
    int DNarcs[20][3]={0,2,50,1,2,45,2,0,64};
    CreateALGraph(DN,&G,3,3,DNvex,&DNarcs[0][0]);
    printf("(4)有向图\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");
    printf("-------------------\n");


    /*
    char DNvex1[20]={'A','B','C'};
    int DNarcs1[20][3]={2,0,50,2,1,45,0,2,64};
    CreateALGraph(DN,&G,3,3,DNvex1,&DNarcs1[0][0]);
    printf("(5)有向网G4的逆邻接表\n");
    OutMGraph(G);
    printf("\n");
    */

    return 0;
}



















