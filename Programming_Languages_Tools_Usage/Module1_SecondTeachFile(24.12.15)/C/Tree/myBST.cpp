#include <stdio.h>
#include <stdlib.h>

typedef struct myBinarySearchTree {
    int value;
    struct myBinarySearchTree *lchild, *rchild;
} BiNode, *BiTree;

BiNode* newTreeNode(int val);
BiNode* findLeftMax(BiTree root);
BiNode* findRightMin(BiTree root);
void insertBiNode(BiTree *root, int val);
void removeBiNode(BiTree root, int val, BiNode *parent);
void printTree(BiTree root);

int main() {
    BiTree root = NULL;
    insertBiNode(&root, 4);
    insertBiNode(&root, 2);
    insertBiNode(&root, 3);
    insertBiNode(&root, 1);
    insertBiNode(&root, 0);
    insertBiNode(&root, 6);
    insertBiNode(&root, 8);
    insertBiNode(&root, 7);
    printTree(root);
    printf("\n");
    removeBiNode(root, 2, root);
    printTree(root);
    printf("\n");

    return 0;
}

BiNode *newTreeNode(int val) {
    BiNode *node = (BiNode *)malloc(sizeof(BiNode));
    node->value = val;
    node->lchild = NULL;
    node->rchild = NULL;
    return node;
}

BiNode* findLeftMax(BiTree root) {
    BiNode *p = root;
    BiNode *c = root->lchild;
    while (c->rchild) {
        p = c;
        c = c->rchild;
    }
    return p;
}

BiNode* findRightMin(BiTree root) {
    BiNode *p = root;
    BiNode *c = root->rchild;
    while (c->lchild) {
        p = c;
        c = c->lchild;
    }
    return p;
}

void insertBiNode(BiTree *root, int val) {
    if (!(*root)) {
        *root = newTreeNode(val);
        return;
    }
    if (val < (*root)->value) {
        insertBiNode(&((*root)->lchild), val);
    } else {
        insertBiNode(&((*root)->rchild), val);
    }
}

void removeBiNode(BiTree root, int val, BiNode *parent) {
    if (!root) {
        printf("%d��㲻����\n", val);
        return ;
    }
    if (val < root->value) {
        removeBiNode(root->lchild, val, root);
    } else if (val > root->value) {
        removeBiNode(root->rchild, val, root);
    } else {
        // �����������������ѡ�����������(����������С)
        if (root->lchild) {
            BiNode *p = findLeftMax(root);
            root->value = (p == root) ? (p->lchild->value) : (p->rchild->value);
            BiNode *tmp = p->lchild->lchild;
            free(p->lchild); 
            p->lchild = tmp;
        } else if (root->rchild) {
            // ��������������ڣ�������������
            BiNode *p = findRightMin(root);
            root->value = (p == root) ? (p->rchild->value) : (p->lchild->value);
            BiNode *tmp = p->rchild->rchild;
            free(p->rchild); 
            p->rchild = tmp;
        } else {
            // Ҷ�ӽ��
            if (parent->value < root->value) {
                parent->rchild = NULL;
            } else {
                parent->lchild = NULL;
            }
            free(root);
        }
    }
}

void printTree(BiTree root) {
    /* ������� */
    if (!root) {
        return ;
    }
    printTree(root->lchild);
    printf("%d ", root->value);
    printTree(root->rchild);
}