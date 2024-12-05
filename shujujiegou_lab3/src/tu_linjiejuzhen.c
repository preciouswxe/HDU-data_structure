//
// Created by 13561 on 2024/12/5.
//
#include<stdio.h>

#define INFINITY 32767                   //表示最大值∞
#define MAX_VERTEX_NUM  20       //最大顶点数
#define OK 1

typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //图的枚举类型   带U的是无向图，不带的是有向图,G是图，N是网
typedef int VRType;
typedef char InfoType;
typedef char VertexType;

// 邻接矩阵
typedef struct ArcCell {

    VRType adj  ;          //对无权图用1或0表示；对带权图，则为权值类型。
    InfoType *info;       //该边或弧相关信息的指针（字符串）

} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

// 图 含矩阵 顶点 定点数和弧数 图类别
typedef struct{

    VertexType vexs[MAX_VERTEX_NUM];   //顶点数组，存放顶点信息
    AdjMatrix   arcs;                  //邻接矩阵
    int vexnum, arcnum;           //图中顶点总数与弧数
    GraphKind  kind;                 //图的种类标志

} MGraph;

/*
 * GraphKind GKind: 图的类型 DG,DN,UDG,UDN
 * MGraph G: 图变量，存放图的顶点数组、邻接矩阵等数据
 * int vexnum: 图的顶点数
 * int arcnum: 图的边或弧数
 * char *vexs: 顶点数组，存放图的顶点
 * int *arcs: 边(弧)数组，每条边（弧）采用三元组形式存放(i, j, w)，分别表示边的邻接顶点和权值或弧的弧尾、 弧头和权值。对于无权值的图，权值=1。
 * 该参数的实参用二维数组存储，形参用一维数组的形式访问。
 * 在实现时要注意无向图（网）是对称矩阵，有向图（网）是非对称矩阵。
 */

// 一个能够创建4种不同类型图的邻接矩阵存储的通用函数
Status CreateMGraph(GraphKind GKind, MGraph *G, int vexnum, int arcnum, char *vexs, int *arcs) {
    G->kind = GKind;
    G->vexnum = vexnum;
    G->arcnum = arcnum;

    // 初始化顶点数组
    for(int i = 0; i < vexnum ;i++) {
        G->vexs[i] = vexs[i];
    }

    // 初始化邻接矩阵
    for(int i = 0; i < vexnum;i++) {
        for(int j = 0;j < vexnum;j++) {
            // 网 初始化无穷大
            if(GKind == DN || GKind == UDN) {
                G->arcs[i][j].adj = INFINITY;
            }else {
                // 图 初始化0
                G->arcs[i][j].adj = 0;
            }
            G->arcs[i][j].info = NULL;
        }
    }

    // 根据边（弧）信息填充邻接矩阵
    // 解析三元组并赋值 从一维的边（弧）数组arcs中依次提取出每条边（弧）对应的两个顶点索引（i和j）以及权值（w）
    for (int k = 0; k < arcnum; ++k) {
        int i = arcs[3 * k];
        int j = arcs[3 * k + 1];
        int w = arcs[3 * k + 2];

        G->arcs[i][j].adj = w;
        if (GKind == UDG || GKind == UDN) {
            // 对于无向图（网），对称位置也要赋值
            G->arcs[j][i].adj = w;
        }
    }

    return OK;
}

// 输出图G的顶点数组元素和邻接矩阵，对于无权图，元素用1和0表示，对于有权图（网），边的权值正整数表示，其他用∞表示。
void OutMGraph(MGraph G) {
    // 输出顶点数组元素
    printf("顶点数组元素：");
    for (int i = 0; i < G.vexnum; ++i) {
        printf("%c ", G.vexs[i]);
    }
    printf("\n");

    // 输出邻接矩阵
    printf("邻接矩阵：\n");
    for (int i = 0; i < G.vexnum; ++i) {
        for (int j = 0; j < G.vexnum; ++j) {
            if (G.arcs[i][j].adj == INFINITY) {
                printf("∞ ");
            } else {
                printf("%d ", G.arcs[i][j].adj);
            }
        }
        printf("\n");
    }
}


// 输出图G中各顶点的度（有向图分入度和出度）
Status VerDegree(MGraph G) {
    // 有向图
    // 竖列是出 横列是入
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

        printf("各顶点的入度和出度情况如下：\n");
        for (int k = 0; k < G.vexnum; ++k) {
            printf("顶点 %c 的入度：%d，出度：%d\n", G.vexs[k], indegree[k], outdegree[k]);
        }

    }else {
        // 无向图
        int degree[MAX_VERTEX_NUM] = {0};
        for (int i = 0; i < G.vexnum; ++i) {
            for (int j = 0; j < G.vexnum; ++j) {
                if (G.arcs[i][j].adj != 0) {
                    degree[i]++;
                }
            }
        }

        printf("各顶点的度情况如下：\n");
        for (int k = 0; k < G.vexnum; ++k) {
            printf("顶点 %c 的度：%d\n", G.vexs[k], degree[k]);
        }


    }
    return OK;
}


int main() {
    MGraph G;
    //输出并创建无向图
    char UDGvex[20]={'A','B','C','D'};
    int UDGarcs[20][3]={0,1,1,0,2,1,0,3,1,1,2,1,2,3,1};
    CreateMGraph(UDG,&G,4,5,UDGvex,&UDGarcs[0][0]);
    printf("(1)无向图G\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");

    //输出并创建有向图
    char DGvex[20]={'A','B','C','D'};
    int DGarcs[20][3]={0,1,1,1,0,1,1,2,1,2,0,1,2,1,1,3,1,1};
    CreateMGraph(DG,&G,4,6,DGvex,&DGarcs[0][0]);
    printf("(2)有向图");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");

    //创建并输出无向图
    char UDNvex[20]={'A','B','C','D'};
    int UDNarcs[20][3]={0,1,56,0,2,34,0,3,78,2,3,25};
    CreateMGraph(UDN,&G,4,4,UDNvex,&UDNarcs[0][0]);
    printf("(3)无向网");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    printf("\n");

    //创建并输出有向网
    char DNvex[20]={'A','B','C'};
    int DNarcs[20][3]={0,2,50,1,2,45,2,0,64};
    CreateMGraph(DN,&G,3,3,DNvex,&DNarcs[0][0]);
    printf("(4)有向网G\n");
    OutMGraph(G);
    printf("\n");
    VerDegree(G);
    return 0;
}
































