//
// Created by 13561 on 2024/12/12.
//

// 如果连通图G的一个子图是一棵包含G的所有顶点的树，则该子图称为G的生成树(SpanningTree)。
// 最小生成树（minimum spanning tree）其实就是一个生成树，不过它不同于一般的生成树，它的边权之和是最小的，即边权和最小的生成树。

#include<stdio.h>
#include<stdlib.h>

#define INFINITY 32767                   //表示最大值∞
#define MAX_VERTEX_NUM  20      //最大顶点数
#define MAX_EDGE_NUM  20         //最大边数
#define OK 1
#define MaxInt 32767
#define SpecialNum 50000


typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //图的枚举类型
typedef int VRType;   //权值
typedef char InfoType;  //指针

// 邻接矩阵
typedef struct ArcCell {
    VRType  adj ;          //值  无权图用1或0表示；对带权图，则为权值类型。
    InfoType *info;       //该边或弧相关信息的指针（字符串）
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

// 顶点数组的类型定义
typedef char VertexType;

// 图
typedef struct{
    VertexType  vexs[MAX_VERTEX_NUM];   //顶点数组，存放顶点信息
    AdjMatrix   arcs;                  //邻接矩阵
    int         vexnum, arcnum;           //图中顶点总数与弧数
    GraphKind   kind;                 //图的种类标志
} MGraph;


// 1. Prim: 创建辅助数组
typedef struct//辅助数组结构体(候选最短边)
{
    VertexType adjvex;       //候选最短边的邻接点
    int lowcost;             //候选最短边的权值
}ShortEdge;

// 2. Kruskal: 边结构体
typedef struct  {
    VertexType start;   //边的起点的下标
    VertexType end;     //边的终点的下标
    int weight;  //边权值
}Edge;

// 并查集结构体，parent数组用于记录每个顶点的父节点，用于判断集合关系
typedef struct UnionFindSet {
    int parent[MAX_VERTEX_NUM];
} UnionFindSet;


/*
typedef struct {
    VertexType Vertex[MAX_VERTEX_NUM];//顶点数组
    Edge edge[MAX_VERTEX_NUM];//边集数组
    int vexnum;//顶点数
    int edgenum;//边数
}EdgeGraph;
*/




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
    printf("-------------------\n");
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
    printf("-------------------\n");
}

// 查找元素v在一维数组 vexs[] 中的下标，并返回下标
int Locate(MGraph G, VertexType v) {
    int i;
    for(i=0;i<G.vexnum;i++) {
        if (v == G.vexs[i]) {
            return i;
        }
    }

    printf("没找到该元素在数组里的下标!\n");
    return -1;
}

// 用于 Prim算法 找权值最短路径
int minimal(MGraph G,ShortEdge *short_edge) {
    int i;
    // min找最小权值
    int min = MaxInt;
    int loc = -1;

    for(i=1 ; i < G.vexnum ;i++) {
        // 已经是最小生成树的specialnum不参与
        if(min > short_edge[i].lowcost && short_edge[i].lowcost!= SpecialNum) {
            // 记下一轮里目前最小的值
            min = short_edge[i].lowcost;
            // 记下最小权值的顶点的下标
            loc = i;
            printf("找到的min权值 : %d ，loc：%d\n",min,loc);
        }
    }
    return loc;
}

// Prim算法
void MiniSpanTree_Prim(MGraph G, VertexType u) {
    // 创建辅助数组
    ShortEdge short_edge[MAX_VERTEX_NUM];

    int i,j,k;

    // 处理起始点u
    k = Locate(G,u);
    for(i=0;i<G.vexnum;i++) {
        // 顶点是谁 存入
        short_edge[i].adjvex = u;
        // 对应接下来的顶点的权值距离
        // 如果是无向图则需要判断0的情况 赋值为无穷大 否则会导致无法判断权值大小（有0的存在）
        // 有向图本身就是MaxInt也就是INFINITY
        if(G.arcs[k][i].adj == 0) {
            short_edge[i].lowcost = MaxInt;
        }else {
            short_edge[i].lowcost = G.arcs[k][i].adj;
        }
    }
    short_edge[k].lowcost = SpecialNum;        //将起始顶点的权值设为specialnum，表示已加入最小生成树

    // 处理后续结点
    for(i=0;i<G.vexnum-1;i++) {
        // 找最短路径的顶点
        k = minimal(G,short_edge);

        printf("最小权值的边是 %c -> %c , 权值是 %d \n",short_edge[k].adjvex, G.vexs[k] , short_edge[k].lowcost);

        // 标记为最小生成树的顶点了 后续找最短路径的minimal函数不能考虑这个结点
        printf(" 当前加入的顶点下标也就是 k 值: %d\n",k);
        short_edge[k].lowcost = SpecialNum;

        // 因为多了新节点，会多很多边，顶点间的最短路径要更新
        for(j=0;j<G.vexnum;j++) {
            // 进行比较 新来的顶点是否有更短的路径
            //printf("正在处理顶点 %c，当前边权值为 %d，当前 short_edge[%d].lowcost 值为 %d\n", G.vexs[j], G.arcs[k][j].adj, j, short_edge[j].lowcost);
            // 防止因为无向图本身权值0干扰判断 所以不等于
            if(G.arcs[k][j].adj < short_edge[j].lowcost && short_edge[j].lowcost!=SpecialNum && G.arcs[k][j].adj != 0) {
                short_edge[j].lowcost = G.arcs[k][j].adj;
                short_edge[j].adjvex = G.vexs[k];
                //printf("正在处理顶点 %c，当前边权值为 %d，当前 short_edge[%d].lowcost 值为 %d\n", G.vexs[j], G.arcs[k][j].adj, j, short_edge[j].lowcost);
            }
        }
        printf("--------------------\n");
    }
}

// 初始化并查集，每个顶点的父节点初始化为自己
void InitUnionFindSet(UnionFindSet *ufs, int n) {
    for (int i = 0; i < n; i++) {
        ufs->parent[i] = i;
    }
}

// 查找顶点x所在集合的根节点（代表元素），采用路径压缩
int Find(UnionFindSet *ufs, int x) {
    if (ufs->parent[x]!= x) {
        ufs->parent[x] = Find(ufs, ufs->parent[x]);
    }
    return ufs->parent[x];
}

// 合并顶点x和y所在的集合
void Union(UnionFindSet *ufs, int x, int y) {
    int root_x = Find(ufs, x);
    int root_y = Find(ufs, y);
    if (root_x!= root_y) {
        ufs->parent[root_x] = root_y;
    }
}

// 冒泡排序算法对存储边信息的 edges 数组按照边的权值从小到大进行排序
void SortEdges(Edge edges[], int edge_num) {
    for (int i = 0; i < edge_num - 1; i++) {
        for (int j = 0; j < edge_num - i - 1; j++) {
            if (edges[j].weight > edges[j + 1].weight) {
                Edge temp = edges[j];
                edges[j] = edges[j + 1];
                edges[j + 1] = temp;
            }
        }
    }
}

// Kruskal算法
void MiniSpanTree_Kruskal(MGraph G) {
    Edge all_edges[MAX_EDGE_NUM];  // 存储图中所有边的数组
    int edge_index = 0;

    // 步骤一：提取所有边信息并存入all_edges数组
    for (int i = 0; i < G.vexnum; i++) {
        for (int j = i + 1; j < G.vexnum; j++) {
            if (G.arcs[i][j].adj != 0 ) {
                all_edges[edge_index].start = i;
                all_edges[edge_index].end = j;
                all_edges[edge_index].weight = G.arcs[i][j].adj;
                edge_index++;
            }
        }
    }
    int edge_count = edge_index;  // 记录实际的边数量

    // 对所有边进行排序
    SortEdges(all_edges, edge_count);

    // 打印排序后的边数组
    printf("排序后的边数组信息如下：\n");
    for (int i = 0; i < edge_count; i++) {
        printf("边：%c - %c ，权值：%d\n", G.vexs[all_edges[i].start], G.vexs[all_edges[i].end], all_edges[i].weight);
    }

    UnionFindSet ufs;
    InitUnionFindSet(&ufs, G.vexnum);  // 初始化并查集

    Edge mst_edges[MAX_VERTEX_NUM - 1];  // 存储最小生成树的边
    int mst_edge_count = 0;

    // 步骤二：遍历排序后的边，利用并查集判断是否加入最小生成树
    for (int i = 0; i < edge_count; i++) {
        int start = all_edges[i].start;
        int end = all_edges[i].end;
        if (Find(&ufs, start) != Find(&ufs, end)) {
            mst_edges[mst_edge_count].start = start;
            mst_edges[mst_edge_count].end = end;
            mst_edges[mst_edge_count].weight = all_edges[i].weight;
            mst_edge_count++;

            Union(&ufs, start, end);

            if (mst_edge_count == G.vexnum - 1) {
                break;  // 已经选取了n - 1条边，最小生成树构建完成
            }
        }
    }

    // 输出最小生成树的边信息
    printf("\nKruskal 算法对同一图构建的最小生成树的边信息如下：\n");
    for (int i = 0; i < mst_edge_count; i++) {
        printf("边：%c - %c ，权值：%d\n", G.vexs[mst_edges[i].start], G.vexs[mst_edges[i].end], mst_edges[i].weight);
    }
}



int main() {
    MGraph G;
    GraphKind GKind = UDG;  // 定义为无向图
    int vexnum = 6;  // 顶点数量
    int arcnum = 10;  // 边数量
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};  // 顶点数组
    int arcs[] = {
        0, 1, 10,
        0, 2, 12,
        0, 4, 15,
        1, 2, 7,
        1, 3, 5,
        1, 5, 6,
        2, 4, 12,
        2, 5, 8,
        3, 5, 6,
        4, 5, 10
    };  // 边数组，每个元素依次表示边的起点、终点、权值

    // 创建图
    CreateMGraph(GKind, &G, vexnum, arcnum, vexs, arcs);
    // 输出图的顶点数组元素和邻接矩阵
    OutMGraph(G);

    // 使用Prim算法求最小生成树，以顶点'A'为起始点
    printf("Prim 算法最小生成树（以A为起始点）：\n");
    MiniSpanTree_Prim(G, 'A');

    MiniSpanTree_Kruskal(G);

    return 0;
}