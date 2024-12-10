#include <stdio.h>
#include <stdlib.h>

typedef struct myBinarylist {
    int value;
    struct myBinarylist *next;
    struct myBinarylist *prev;
} ListNode;

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
    insertItem(n0, 4, 4);

    ListNode *target1 = findItem(n0, 2);
    if (target1) {
        printf("The value of this Item found is%d", target1->value);
        if (target1->next) {
            printf(", Its next value is%d\n", target1->next->value);
        } else {
            printf(", It's the last node.\n");
        }
    }

    ListNode *target2 = findItem(n0, 5);
    if (target2) {
        printf("The value of this Item found is%d", target2->value);
        if (target2->next) {
            printf(", Its next value is%d\n", target2->next->value);
        } else {
            printf(", It's the last node.\n");
        }
    }

    removeItem(n0, 3);
    removeItem(n0, 5);
    printLinkedlist(n0);

    return 0;
}

ListNode* newList() {
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = 0;
    node->next = NULL;
    node->prev = NULL;
    return node;
}

void insertItem(ListNode* head, int val, int pos) {
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = val;
    node->next = NULL;
    node->prev = NULL;

    ListNode *p = head;
    while (--pos > 0 && p->next) {
        p = p->next;
    }
    node->next = p->next;
    node->prev = p;
    if (p->next) {
        p->next->prev = node;
    }
    p->next = node;
}

void removeItem(ListNode* head, int val) {
    ListNode *p = head->next;
    while (p && p->value != val) {
        p = p->next;
    }
    if (p) {
        if (p->next) {
            p->next->prev = p->prev;
        }
        p->prev->next = p->next;
        free(p);
    } else {
        printf("%d is not found\n", val);
    }
}

ListNode* findItem(ListNode* head, int val) {
    ListNode *p = head->next;
    while (p && p->value != val) {
        p = p->next;
    }
    if (p) {
        return p;
    } else {
        printf("%d is not found\n", val);
        return NULL;
    }
}

void printLinkedlist(ListNode *head) {
    ListNode *p = head->next;
    while (p) {
        printf("%d ", p->value);
        p = p->next;
    }
    printf("\n");
}
