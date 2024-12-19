//
// Created by 13561 on 2024/12/12.
//

// �����ͨͼG��һ����ͼ��һ�ð���G�����ж�������������ͼ��ΪG��������(SpanningTree)��
// ��С��������minimum spanning tree����ʵ����һ������������������ͬ��һ��������������ı�Ȩ֮������С�ģ�����Ȩ����С����������

#include<stdio.h>
#include<stdlib.h>

#define INFINITY 32767                   //��ʾ���ֵ��
#define MAX_VERTEX_NUM  20      //��󶥵���
#define MAX_EDGE_NUM  20         //������
#define OK 1
#define MaxInt 32767
#define SpecialNum 50000


typedef int Status;
typedef enum {DG,DN,UDG,UDN} GraphKind;  //ͼ��ö������
typedef int VRType;   //Ȩֵ
typedef char InfoType;  //ָ��

// �ڽӾ���
typedef struct ArcCell {
    VRType  adj ;          //ֵ  ��Ȩͼ��1��0��ʾ���Դ�Ȩͼ����ΪȨֵ���͡�
    InfoType *info;       //�ñ߻������Ϣ��ָ�루�ַ�����
} ArcCell, AdjMatrix[MAX_VERTEX_NUM][MAX_VERTEX_NUM];

// ������������Ͷ���
typedef char VertexType;

// ͼ
typedef struct{
    VertexType  vexs[MAX_VERTEX_NUM];   //�������飬��Ŷ�����Ϣ
    AdjMatrix   arcs;                  //�ڽӾ���
    int         vexnum, arcnum;           //ͼ�ж��������뻡��
    GraphKind   kind;                 //ͼ�������־
} MGraph;


// 1. Prim: ������������
typedef struct//��������ṹ��(��ѡ��̱�)
{
    VertexType adjvex;       //��ѡ��̱ߵ��ڽӵ�
    int lowcost;             //��ѡ��̱ߵ�Ȩֵ
}ShortEdge;

// 2. Kruskal: �߽ṹ��
typedef struct  {
    VertexType start;   //�ߵ������±�
    VertexType end;     //�ߵ��յ���±�
    int weight;  //��Ȩֵ
}Edge;

// ���鼯�ṹ�壬parent�������ڼ�¼ÿ������ĸ��ڵ㣬�����жϼ��Ϲ�ϵ
typedef struct UnionFindSet {
    int parent[MAX_VERTEX_NUM];
} UnionFindSet;


/*
typedef struct {
    VertexType Vertex[MAX_VERTEX_NUM];//��������
    Edge edge[MAX_VERTEX_NUM];//�߼�����
    int vexnum;//������
    int edgenum;//����
}EdgeGraph;
*/




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
    printf("-------------------\n");
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
    printf("-------------------\n");
}

// ����Ԫ��v��һά���� vexs[] �е��±꣬�������±�
int Locate(MGraph G, VertexType v) {
    int i;
    for(i=0;i<G.vexnum;i++) {
        if (v == G.vexs[i]) {
            return i;
        }
    }

    printf("û�ҵ���Ԫ������������±�!\n");
    return -1;
}

// ���� Prim�㷨 ��Ȩֵ���·��
int minimal(MGraph G,ShortEdge *short_edge) {
    int i;
    // min����СȨֵ
    int min = MaxInt;
    int loc = -1;

    for(i=1 ; i < G.vexnum ;i++) {
        // �Ѿ�����С��������specialnum������
        if(min > short_edge[i].lowcost && short_edge[i].lowcost!= SpecialNum) {
            // ����һ����Ŀǰ��С��ֵ
            min = short_edge[i].lowcost;
            // ������СȨֵ�Ķ�����±�
            loc = i;
            printf("�ҵ���minȨֵ : %d ��loc��%d\n",min,loc);
        }
    }
    return loc;
}

// Prim�㷨
void MiniSpanTree_Prim(MGraph G, VertexType u) {
    // ������������
    ShortEdge short_edge[MAX_VERTEX_NUM];

    int i,j,k;

    // ������ʼ��u
    k = Locate(G,u);
    for(i=0;i<G.vexnum;i++) {
        // ������˭ ����
        short_edge[i].adjvex = u;
        // ��Ӧ�������Ķ����Ȩֵ����
        // ���������ͼ����Ҫ�ж�0����� ��ֵΪ����� ����ᵼ���޷��ж�Ȩֵ��С����0�Ĵ��ڣ�
        // ����ͼ�������MaxIntҲ����INFINITY
        if(G.arcs[k][i].adj == 0) {
            short_edge[i].lowcost = MaxInt;
        }else {
            short_edge[i].lowcost = G.arcs[k][i].adj;
        }
    }
    short_edge[k].lowcost = SpecialNum;        //����ʼ�����Ȩֵ��Ϊspecialnum����ʾ�Ѽ�����С������

    // ����������
    for(i=0;i<G.vexnum-1;i++) {
        // �����·���Ķ���
        k = minimal(G,short_edge);

        printf("��СȨֵ�ı��� %c -> %c , Ȩֵ�� %d \n",short_edge[k].adjvex, G.vexs[k] , short_edge[k].lowcost);

        // ���Ϊ��С�������Ķ����� ���������·����minimal�������ܿ���������
        printf(" ��ǰ����Ķ����±�Ҳ���� k ֵ: %d\n",k);
        short_edge[k].lowcost = SpecialNum;

        // ��Ϊ�����½ڵ㣬���ܶ�ߣ����������·��Ҫ����
        for(j=0;j<G.vexnum;j++) {
            // ���бȽ� �����Ķ����Ƿ��и��̵�·��
            //printf("���ڴ����� %c����ǰ��ȨֵΪ %d����ǰ short_edge[%d].lowcost ֵΪ %d\n", G.vexs[j], G.arcs[k][j].adj, j, short_edge[j].lowcost);
            // ��ֹ��Ϊ����ͼ����Ȩֵ0�����ж� ���Բ�����
            if(G.arcs[k][j].adj < short_edge[j].lowcost && short_edge[j].lowcost!=SpecialNum && G.arcs[k][j].adj != 0) {
                short_edge[j].lowcost = G.arcs[k][j].adj;
                short_edge[j].adjvex = G.vexs[k];
                //printf("���ڴ����� %c����ǰ��ȨֵΪ %d����ǰ short_edge[%d].lowcost ֵΪ %d\n", G.vexs[j], G.arcs[k][j].adj, j, short_edge[j].lowcost);
            }
        }
        printf("--------------------\n");
    }
}

// ��ʼ�����鼯��ÿ������ĸ��ڵ��ʼ��Ϊ�Լ�
void InitUnionFindSet(UnionFindSet *ufs, int n) {
    for (int i = 0; i < n; i++) {
        ufs->parent[i] = i;
    }
}

// ���Ҷ���x���ڼ��ϵĸ��ڵ㣨����Ԫ�أ�������·��ѹ��
int Find(UnionFindSet *ufs, int x) {
    if (ufs->parent[x]!= x) {
        ufs->parent[x] = Find(ufs, ufs->parent[x]);
    }
    return ufs->parent[x];
}

// �ϲ�����x��y���ڵļ���
void Union(UnionFindSet *ufs, int x, int y) {
    int root_x = Find(ufs, x);
    int root_y = Find(ufs, y);
    if (root_x!= root_y) {
        ufs->parent[root_x] = root_y;
    }
}

// ð�������㷨�Դ洢����Ϣ�� edges ���鰴�ձߵ�Ȩֵ��С�����������
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

// Kruskal�㷨
void MiniSpanTree_Kruskal(MGraph G) {
    Edge all_edges[MAX_EDGE_NUM];  // �洢ͼ�����бߵ�����
    int edge_index = 0;

    // ����һ����ȡ���б���Ϣ������all_edges����
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
    int edge_count = edge_index;  // ��¼ʵ�ʵı�����

    // �����б߽�������
    SortEdges(all_edges, edge_count);

    // ��ӡ�����ı�����
    printf("�����ı�������Ϣ���£�\n");
    for (int i = 0; i < edge_count; i++) {
        printf("�ߣ�%c - %c ��Ȩֵ��%d\n", G.vexs[all_edges[i].start], G.vexs[all_edges[i].end], all_edges[i].weight);
    }

    UnionFindSet ufs;
    InitUnionFindSet(&ufs, G.vexnum);  // ��ʼ�����鼯

    Edge mst_edges[MAX_VERTEX_NUM - 1];  // �洢��С�������ı�
    int mst_edge_count = 0;

    // ����������������ıߣ����ò��鼯�ж��Ƿ������С������
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
                break;  // �Ѿ�ѡȡ��n - 1���ߣ���С�������������
            }
        }
    }

    // �����С�������ı���Ϣ
    printf("\nKruskal �㷨��ͬһͼ��������С�������ı���Ϣ���£�\n");
    for (int i = 0; i < mst_edge_count; i++) {
        printf("�ߣ�%c - %c ��Ȩֵ��%d\n", G.vexs[mst_edges[i].start], G.vexs[mst_edges[i].end], mst_edges[i].weight);
    }
}



int main() {
    MGraph G;
    GraphKind GKind = UDG;  // ����Ϊ����ͼ
    int vexnum = 6;  // ��������
    int arcnum = 10;  // ������
    char vexs[] = {'A', 'B', 'C', 'D', 'E', 'F'};  // ��������
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
    };  // �����飬ÿ��Ԫ�����α�ʾ�ߵ���㡢�յ㡢Ȩֵ

    // ����ͼ
    CreateMGraph(GKind, &G, vexnum, arcnum, vexs, arcs);
    // ���ͼ�Ķ�������Ԫ�غ��ڽӾ���
    OutMGraph(G);

    // ʹ��Prim�㷨����С���������Զ���'A'Ϊ��ʼ��
    printf("Prim �㷨��С����������AΪ��ʼ�㣩��\n");
    MiniSpanTree_Prim(G, 'A');

    MiniSpanTree_Kruskal(G);

    return 0;
}