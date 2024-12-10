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
    insertItem(n0, 4, 4);
    ListNode *target1 = findItem(n0, 2);
    if (target1) {
        // 注意:这个printf有bug，Item->next可能为NULL
        printf("找到的这个Item的值为%d, 它的下一个值为%d\n", target1->value, target1->next->value);
    }
    ListNode *target2 = findItem(n0, 5);
    if (target2) {
        printf("找到的这个Item的值为%d, 它的下一个值为%d\n", target2->value, target2->next->value);
    }
    removeItem(n0, 3);
    removeItem(n0, 5);
    printLinkedlist(n0);

    return 0;
}

ListNode* newList() {
    /* 初始化一个空结点，并返回该结点的指针 */
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = -1;
    node->next = NULL;
    return node;
}

void insertItem(ListNode* head, int val, int pos) {
    /* 在pos位置插入一个新结点P，并且默认为最开始的位置 */
    // 注意：缺省参数只能在c++中使用，即文件后缀必须为.cpp
    ListNode *node = (ListNode*)malloc(sizeof(ListNode));
    node->value = val;
    node->next = NULL;
    
    ListNode *p = head;
    // 如果pos太大超出限制，则会被存放在最后
    while (--pos > 0 && p->next) {
        p = p->next;
    }
    node->next = p->next;
    p->next = node;
}

void removeItem(ListNode* head, int val) {
    /* 删除值为val的首个结点 */
    ListNode *p = head->next;
    ListNode *q = head;
    while (p && p->value != val) {
        // 这里有一个逻辑短路原理
        p = p->next;
        q = q->next;
    }
    if (p) {
        // 如果p存在,则删除
        q->next = p->next;
        free(p);
    } else {
        printf("%d不存在\n", val);
    }
}

ListNode* findItem(ListNode* head, int val) {
    /* 返回首个值为val的结点 */
    ListNode *p = head->next;
    while (p && p->value != val) {
        p = p->next;
    }
    if (p) {
        return p;
    } else {
        printf("%d不存在\n", val);
        return NULL;
    }
}

void printLinkedlist(ListNode *head) {
    /* 按照顺序打印整个列表的value */
    ListNode *p = head->next;
    while (p) {
        printf("%d ", p->value);
        p = p->next;
    }
    printf("\n");
}