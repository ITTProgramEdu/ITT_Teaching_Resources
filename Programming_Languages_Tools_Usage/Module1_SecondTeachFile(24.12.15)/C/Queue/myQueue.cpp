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
    /* 入队 */
    if (*rear >= n) {
        printf("队列已满\n");
        return ;
    } else {
        queue[(*rear)++] = val;
    }
}

void popQueue(int *queue, int *rear) {
    /* 出队 */
    if (*rear <= 0) {
        printf("队列为空\n");
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
    /* 打印队列 */
    int i = 0;
    for (; i < rear; i++) {
        printf("%d ", queue[i]);
    }
    printf("\n");
}