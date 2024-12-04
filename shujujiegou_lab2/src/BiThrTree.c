//
// Created by 13561 on 2024/11/28.
//
#include<stdio.h>
#include<stdlib.h>

#define ERROR   0
#define TRUE    1
#define OK      1
#define MAXSIZE 100

typedef int Status;              //声明函数类型名

typedef char TElemType;    //声明结点元素值得类型

typedef struct BiThrNode {  //定义线索二叉链表的结点结构和类型
    TElemType data;
    struct BiThrNode *lchild, *rchild;
	int  LTag, RTag;
}BiThrNode,*BiThrTree;

// 先序进行二叉树创建
Status InitBiThrTree(BiThrTree *T) {
	TElemType ch;
	scanf(" %c",&ch);

	if(ch == '#') {
		*T = NULL;
	} else {
		*T = (BiThrNode *)malloc(sizeof(BiThrNode));
		if((*T) == NULL) {
			return -1;
		}

		(*T)->data = ch;
		(*T)->LTag = 0;
		(*T)->RTag = 0;

		// 初始化左孩子
		InitBiThrTree(&(*T)->lchild);
		// 初始化右孩子
		InitBiThrTree(&(*T)->rchild);
	}
	return OK;
}

// 按中序遍历进行线索化
Status InOrderThreading(BiThrTree *Thrt, BiThrTree T) {
	// pre 是一个辅助指针变量  用于在中序遍历线索化的过程中记录当前节点的前驱结点
	BiThrTree pre;

	// *Thrt 所指向的是中序线索二叉树的头结点
	// 头结点的data域存放字符'@'，指针Thrt指向该头结点
	// 头结点的左指针指向二叉树的根结点（LTag=0），右指针指向该二叉树中序遍历的最后一个结点（RTag=1）
	*Thrt = (BiThrNode *)malloc(sizeof(BiThrNode));
	(*Thrt)->data = '@';
	(*Thrt)->LTag = 0;
	(*Thrt)->RTag = 1;
	if (!T) {
		// 处理空树情况
		(*Thrt)->lchild = *Thrt;
	} else {
		// T 是传入的原始二叉树的根结点指针。从头结点Thrt出发，通过左指针找到根结点
		(*Thrt)->lchild = T;
		// pre变成头结点
		pre = *Thrt;

		// 开始非递归中序遍历进行线索化，也就是借助栈
		BiThrTree p = T;
		BiThrTree Stack[100];
		int top = -1;
		while(p || top!= -1) {
			// 找到这一轮最左的结点
			while(p) {
				Stack[++top] = p;
				p = p->lchild;
			}
			if(top!=-1) {
				// 出栈
				p = Stack[top--];
				// 没有左孩子就写1和前驱
				if(!p->lchild) {
					printf("p!\n");
					p->LTag = 1;
					p->lchild = pre;
					printf("P node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p \n\n", p->data, p->LTag, p->RTag,p->lchild,p->rchild);
				}
				// 前驱节点 pre 的右子树为空，将 pre 的右指针指向当前节点 p。因为pre本来就是p的前驱
				if(!pre->rchild) {
					printf("pre!\n");
					pre->RTag = 1;
					pre->rchild = p;
					printf("Pre node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p\n\n", pre->data, pre->LTag, pre->RTag,pre->lchild,pre->rchild);
				}
				// 更新 pre 为当前节点 p，以便在下一次循环中正确记录新的前驱节点
				pre = p;
				// 往右边找
				p = p->rchild;
			}
		}
		// 将最后一个节点（此时 pre 指向最后一个节点）的右指针指向头结点 *Thrt，使头结点的右指针指向中序遍历的最后一个节点
		pre->rchild = *Thrt;
		pre->RTag = 1;
		(*Thrt)->rchild = pre;

		printf("LAST node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p\n\n", pre->data, pre->LTag, pre->RTag,pre->lchild,pre->rchild);
		printf("Thrt node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p\n\n", (*Thrt)->data, (*Thrt)->LTag, (*Thrt)->RTag,(*Thrt)->lchild,(*Thrt)->rchild);

	}
	return OK;
}


// 按孩子中序遍历
// 打印 | LTag | 左指针所指元素 | 本结点的值 | 右指针所指元素 | RTag |
Status InOrderTraverse(BiThrTree T) {

	if (T) {
		// 递归地遍历二叉树 T 的左子树，会一直深入到最左边的叶子节点，然后逐步向上返回
		printf("Current node: %c, lchild pointer: %p,rchild pointer: %p\n", T->data, T->lchild,T->rchild);

		// 警告！！如果没有这个判断条件则会陷入死循环！！因为会导致一直找lchild然后反复回到根节点
		if(T->LTag == 0) {
			InOrderTraverse(T->lchild);
		}

		// 输出当前结点信息
		printf("| %d |",T->LTag);

		if(T->LTag == 1) {
			printf("^ | ");
		} else if (T->lchild != NULL) {
			printf("%c |",T->lchild->data);
		} else {
			printf("NULL |");
		}

		printf("%c | ",T->data);

		if(T->RTag == 1) {
			printf("^ | ");
		}else if (T->rchild!=NULL){
			printf("%c | ",T->rchild->data);
		}else {
			printf("NULL |");
		}

		printf("%d |\n",T->RTag);

		// 递归遍历右子树 这里同理！
		if(T->RTag == 0) {
			InOrderTraverse(T->rchild);
		}
	}
	return OK;
}



// 按线索和孩子遍历（非递归，格式同上）
Status InOrderTraverse_Thr(BiThrTree T) {
	BiThrTree p = T;
	// 先找有最左边的左结点的根结点
	while (p->LTag == 0) {
		p = p->lchild;
	}

	// 根据是不是遍历到头结点来判断是否循环完毕
	while (p->data!='@') {
		// 左
		printf("| %d | ", p->LTag);
		if (p->LTag == 1) {
			printf("^ | ");
		} else {
			printf("%c | ", p->lchild->data);
		}

		// 值 注意这时候输出的是根节点的值 因为必须是有左子树的p才进入这个循环
		printf("%c | ", p->data);

		// 右
		if (p->RTag == 1) {
			printf("^ | ");
		} else {
			printf("%c | ", p->rchild->data);
		}
		printf("%d |\n", p->RTag);

		// 往后找 如果有右孩子就遍历看看有没有左子树
		if (p->RTag == 0) {
			p = p->rchild;
			while (p->LTag == 0) {
				p = p->lchild;
			}
		} else {
			p = p->rchild;
		}

	}

	// 把头结点单独再输出一下 ，可以省略因为@不是二叉树的内容
	printf("| %d | ", p->LTag);
	if (p->LTag == 1) {
		printf("^ | ");
	} else {
		printf("%c | ", p->lchild->data);
	}

	// 值 注意这时候输出的是根节点的值 因为必须是有左子树的p才进入这个循环
	printf("%c | ", p->data);

	// 右
	if (p->RTag == 1) {
		printf("^ | ");
	} else {
		printf("%c | ", p->rchild->data);
	}
	printf("%d |\n", p->RTag);


	return OK;

}

int main() {
	BiThrTree T,Thrt;

	// （1）调用InitBiThrTree(T)函数，创建一棵按线索二叉链表结构存储的尚未线索化的二叉树
	printf("请输入先序遍历的字符序列（#表示空节点）：\n");
	InitBiThrTree(&T);
	printf("T初始化完成。\n");

	printf("-------------------------------\n");


	// （2）调用InOrderTraverse(T)函数，输出每个结点的数据
	printf("未线索化二叉树按孩子中序遍历结果：\n");
	InOrderTraverse(T);

	printf("-------------------------------\n");

	// （3）调用InOrderThreading(Thrt, T)函数，将T线索化成一棵中序线索二叉树
	InOrderThreading(&Thrt, T);
	printf("Thrt线索化完成。\n");


	printf("-------------------------------\n");
	// （4）调用InOrderTraverse(Thrt)函数，输出每个结点的数据
	printf("中序线索二叉树（递归遍历）结果：\n");
	InOrderTraverse(Thrt->lchild);   //Thrt是头结点 指向根结点

	printf("-------------------------------\n");
	// （5）调用InOrderTraverse_Thr(Thrt)函数，输出每个结点的数据
	printf("中序线索二叉树（非递归遍历）结果：\n");
	InOrderTraverse_Thr(Thrt->lchild);

}















