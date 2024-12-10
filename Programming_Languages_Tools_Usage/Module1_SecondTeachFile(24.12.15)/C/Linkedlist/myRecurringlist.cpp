#include <stdio.h>
#include <stdlib.h>

typedef struct myLinkedlist {
    int value;
    struct myLinkedlist *next;
}ListNode;

ListNode* newList();
void insertItem(ListNode* head, int val, int pos = 1);
void removeItem(ListNode* head, int val);
ListNode* findItem(ListNode* head, int val);
void printLinkedlist(ListNode *head);

int main() {
    ListNode *n0 = newList();
    insertItem(n0, 1);
    insertItem(n0, 2, 2);
    insertItem(n0, 3, 3);
    insertItem(n0, 4, 6);
    printLinkedlist(n0);
    ListNode *target1 = findItem(n0, 2);
    if (target1) {
        // ע��:���printf��bug��Item->next����ΪNULL
        printf("�ҵ������Item��ֵΪ%d, ������һ��ֵΪ%d\n", target1->value, target1->next->value);
    }
    ListNode *target2 = findItem(n0, 5);
    if (target2) {
        printf("�ҵ������Item��ֵΪ%d, ������һ��ֵΪ%d\n", target2->value, target2->next->value);
    }
    removeItem(n0, 3);
    removeItem(n0, 5);
    printLinkedlist(n0);

    return 0;
}

ListNode* newList() {
    /* ��ʼ��һ���ս�㣬�����ظý���ָ�� */
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = -1;
    // ѭ������Ŀ�����ָ���Լ�
    node->next = node;
    return node;
}

void insertItem(ListNode* head, int val, int pos) {
    /* ��posλ�ò���һ���½��P������Ĭ��Ϊ�ʼ��λ�� */
    // ע�⣺ȱʡ����ֻ����c++��ʹ�ã����ļ���׺����Ϊ.cpp
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = val;
    node->next = head;
    
    ListNode *p = head;
    // ���pos̫�󳬳����ƣ���ᱻ��������
    while (--pos > 0 && p->next != head) {
        p = p->next;
    }
    node->next = p->next;
    p->next = node;
}

void removeItem(ListNode* head, int val) {
    /* ɾ��ֵΪval���׸���� */
    ListNode *p = head->next;
    ListNode *q = head;
    while (p != head && p->value != val) {
        // ������һ���߼���·ԭ��
        p = p->next;
        q = q->next;
    }
    if (p != head) {
        // ���p����,��ɾ��
        q->next = p->next;
        free(p);
    } else {
        printf("ɾ��%d������\n", val);
    }
}

ListNode* findItem(ListNode* head, int val) {
    /* �����׸�ֵΪval�Ľ�� */
    ListNode *p = head->next;
    while (p != head && p->value != val) {
        p = p->next;
    }
    if (p != head) {
        return p;
    } else {
        printf("Ѱ��%d������\n", val);
        return NULL;
    }
}

void printLinkedlist(ListNode *head) {
    /* ����˳���ӡ�����б��value */
    ListNode *p = head->next;
    while (p != head) {
        printf("%d ", p->value);
        p = p->next;
    }
    printf("\n");
}