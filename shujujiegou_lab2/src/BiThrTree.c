//
// Created by 13561 on 2024/11/28.
//
#include<stdio.h>
#include<stdlib.h>

#define ERROR   0
#define TRUE    1
#define OK      1
#define MAXSIZE 100

typedef int Status;              //��������������

typedef char TElemType;    //�������Ԫ��ֵ������

typedef struct BiThrNode {  //����������������Ľ��ṹ������
    TElemType data;
    struct BiThrNode *lchild, *rchild;
	int  LTag, RTag;
}BiThrNode,*BiThrTree;

// ������ж���������
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

		// ��ʼ������
		InitBiThrTree(&(*T)->lchild);
		// ��ʼ���Һ���
		InitBiThrTree(&(*T)->rchild);
	}
	return OK;
}

// �������������������
Status InOrderThreading(BiThrTree *Thrt, BiThrTree T) {
	// pre ��һ������ָ�����  ��������������������Ĺ����м�¼��ǰ�ڵ��ǰ�����
	BiThrTree pre;

	// *Thrt ��ָ���������������������ͷ���
	// ͷ����data�����ַ�'@'��ָ��Thrtָ���ͷ���
	// ͷ������ָ��ָ��������ĸ���㣨LTag=0������ָ��ָ��ö�����������������һ����㣨RTag=1��
	*Thrt = (BiThrNode *)malloc(sizeof(BiThrNode));
	(*Thrt)->data = '@';
	(*Thrt)->LTag = 0;
	(*Thrt)->RTag = 1;
	if (!T) {
		// ����������
		(*Thrt)->lchild = *Thrt;
	} else {
		// T �Ǵ����ԭʼ�������ĸ����ָ�롣��ͷ���Thrt������ͨ����ָ���ҵ������
		(*Thrt)->lchild = T;
		// pre���ͷ���
		pre = *Thrt;

		// ��ʼ�ǵݹ��������������������Ҳ���ǽ���ջ
		BiThrTree p = T;
		BiThrTree Stack[100];
		int top = -1;
		while(p || top!= -1) {
			// �ҵ���һ������Ľ��
			while(p) {
				Stack[++top] = p;
				p = p->lchild;
			}
			if(top!=-1) {
				// ��ջ
				p = Stack[top--];
				// û�����Ӿ�д1��ǰ��
				if(!p->lchild) {
					printf("p!\n");
					p->LTag = 1;
					p->lchild = pre;
					printf("P node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p \n\n", p->data, p->LTag, p->RTag,p->lchild,p->rchild);
				}
				// ǰ���ڵ� pre ��������Ϊ�գ��� pre ����ָ��ָ��ǰ�ڵ� p����Ϊpre��������p��ǰ��
				if(!pre->rchild) {
					printf("pre!\n");
					pre->RTag = 1;
					pre->rchild = p;
					printf("Pre node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p\n\n", pre->data, pre->LTag, pre->RTag,pre->lchild,pre->rchild);
				}
				// ���� pre Ϊ��ǰ�ڵ� p���Ա�����һ��ѭ������ȷ��¼�µ�ǰ���ڵ�
				pre = p;
				// ���ұ���
				p = p->rchild;
			}
		}
		// �����һ���ڵ㣨��ʱ pre ָ�����һ���ڵ㣩����ָ��ָ��ͷ��� *Thrt��ʹͷ������ָ��ָ��������������һ���ڵ�
		pre->rchild = *Thrt;
		pre->RTag = 1;
		(*Thrt)->rchild = pre;

		printf("LAST node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p\n\n", pre->data, pre->LTag, pre->RTag,pre->lchild,pre->rchild);
		printf("Thrt node: %c, LTag: %d, RTag: %d,lchild:%p,rchild:%p\n\n", (*Thrt)->data, (*Thrt)->LTag, (*Thrt)->RTag,(*Thrt)->lchild,(*Thrt)->rchild);

	}
	return OK;
}


// �������������
// ��ӡ | LTag | ��ָ����ָԪ�� | ������ֵ | ��ָ����ָԪ�� | RTag |
Status InOrderTraverse(BiThrTree T) {

	if (T) {
		// �ݹ�ر��������� T ������������һֱ���뵽����ߵ�Ҷ�ӽڵ㣬Ȼ�������Ϸ���
		printf("Current node: %c, lchild pointer: %p,rchild pointer: %p\n", T->data, T->lchild,T->rchild);

		// ���棡�����û������ж��������������ѭ��������Ϊ�ᵼ��һֱ��lchildȻ�󷴸��ص����ڵ�
		if(T->LTag == 0) {
			InOrderTraverse(T->lchild);
		}

		// �����ǰ�����Ϣ
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

		// �ݹ���������� ����ͬ��
		if(T->RTag == 0) {
			InOrderTraverse(T->rchild);
		}
	}
	return OK;
}



// �������ͺ��ӱ������ǵݹ飬��ʽͬ�ϣ�
Status InOrderTraverse_Thr(BiThrTree T) {
	BiThrTree p = T;
	// ����������ߵ�����ĸ����
	while (p->LTag == 0) {
		p = p->lchild;
	}

	// �����ǲ��Ǳ�����ͷ������ж��Ƿ�ѭ�����
	while (p->data!='@') {
		// ��
		printf("| %d | ", p->LTag);
		if (p->LTag == 1) {
			printf("^ | ");
		} else {
			printf("%c | ", p->lchild->data);
		}

		// ֵ ע����ʱ��������Ǹ��ڵ��ֵ ��Ϊ����������������p�Ž������ѭ��
		printf("%c | ", p->data);

		// ��
		if (p->RTag == 1) {
			printf("^ | ");
		} else {
			printf("%c | ", p->rchild->data);
		}
		printf("%d |\n", p->RTag);

		// ������ ������Һ��Ӿͱ���������û��������
		if (p->RTag == 0) {
			p = p->rchild;
			while (p->LTag == 0) {
				p = p->lchild;
			}
		} else {
			p = p->rchild;
		}

	}

	// ��ͷ��㵥�������һ�� ������ʡ����Ϊ@���Ƕ�����������
	printf("| %d | ", p->LTag);
	if (p->LTag == 1) {
		printf("^ | ");
	} else {
		printf("%c | ", p->lchild->data);
	}

	// ֵ ע����ʱ��������Ǹ��ڵ��ֵ ��Ϊ����������������p�Ž������ѭ��
	printf("%c | ", p->data);

	// ��
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

	// ��1������InitBiThrTree(T)����������һ�ð�������������ṹ�洢����δ�������Ķ�����
	printf("����������������ַ����У�#��ʾ�սڵ㣩��\n");
	InitBiThrTree(&T);
	printf("T��ʼ����ɡ�\n");

	printf("-------------------------------\n");


	// ��2������InOrderTraverse(T)���������ÿ����������
	printf("δ������������������������������\n");
	InOrderTraverse(T);

	printf("-------------------------------\n");

	// ��3������InOrderThreading(Thrt, T)��������T��������һ����������������
	InOrderThreading(&Thrt, T);
	printf("Thrt��������ɡ�\n");


	printf("-------------------------------\n");
	// ��4������InOrderTraverse(Thrt)���������ÿ����������
	printf("�����������������ݹ�����������\n");
	InOrderTraverse(Thrt->lchild);   //Thrt��ͷ��� ָ������

	printf("-------------------------------\n");
	// ��5������InOrderTraverse_Thr(Thrt)���������ÿ����������
	printf("�����������������ǵݹ�����������\n");
	InOrderTraverse_Thr(Thrt->lchild);

}















