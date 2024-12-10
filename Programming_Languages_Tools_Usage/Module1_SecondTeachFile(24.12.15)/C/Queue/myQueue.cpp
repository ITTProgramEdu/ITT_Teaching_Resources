#include <stdio.h>

void pushQueue(int *queue, int *rear, int n, int val);
void popQueue(int *queue, int *rear);
void printQueue(int *queue, int rear);

int main() {
    int n = 2;
    // scanf("%d", &n);
    int queue[n];
    int rear = 0;
    pushQueue(queue, &rear, n, 1);
    pushQueue(queue, &rear, n, 2);
    pushQueue(queue, &rear, n, 3);
    pushQueue(queue, &rear, n, 4);
    printQueue(queue, rear);
    popQueue(queue, &rear);
    popQueue(queue, &rear);
    printQueue(queue, rear);
}

void pushQueue(int *queue, int *rear, int n, int val) {
    /* ��� */
    if (*rear >= n) {
        printf("��������\n");
        return ;
    } else {
        queue[(*rear)++] = val;
    }
}

void popQueue(int *queue, int *rear) {
    /* ���� */
    if (*rear <= 0) {
        printf("����Ϊ��\n");
        return ;
    } else {
        int i = 0;
        for (; i < (*rear)-1; i++) {
            queue[i] = queue[i+1];
        }
        (*rear)--;
    }
}

void printQueue(int *queue, int rear) {
    /* ��ӡ���� */
    int i = 0;
    for (; i < rear; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}