#include <stdio.h>
#include <stdlib.h>

typedef struct myRecurringQueue {
    int *queue;
    int front;
    int rear;
    int n;
} myRecurringQueue;

myRecurringQueue* initQueue(int n);
bool isEmpty(myRecurringQueue *q);
bool isFull(myRecurringQueue *q);
int countQueue(myRecurringQueue *q);
void pushQueue(myRecurringQueue *q, int val);
void popQueue(myRecurringQueue *q);
void printQueue(myRecurringQueue *q);

int main() {
    int n = 4;
    // scanf("%d", &n);
    myRecurringQueue* q = initQueue(n);
    popQueue(q);
    pushQueue(q, 1);
    pushQueue(q, 2);
    pushQueue(q, 3);
    pushQueue(q, 4);
    printQueue(q);
    pushQueue(q, 5);
    printQueue(q);
    popQueue(q);
    popQueue(q);
    pushQueue(q, 6);
    printQueue(q);
    popQueue(q);
    printQueue(q);
}

bool isEmpty(myRecurringQueue *q) {
    return q->front == q->rear;
}

bool isFull(myRecurringQueue *q) {
    return (q->rear+1)%q->n == q->front;
}

int countQueue(myRecurringQueue *q) {
    return (q->rear - q->front < 0) ? (q->rear - q->front + q->n) : (q->rear - q->front);
}

myRecurringQueue* initQueue(int n) {
    myRecurringQueue *p = (myRecurringQueue*)malloc(sizeof(myRecurringQueue));
    // 队列多一个空间用来区分满队列和空队列
    p->queue = (int*)malloc(sizeof(int)*(n+1));
    p->front = 0;
    p->rear = 0;
    p->n = n+1;
    return p;
}

void pushQueue(myRecurringQueue *q, int val) {
    /* 入队 */
    if (isFull(q)) {
        printf("The queue is full. You can't re-enter the queue.\n");
        return ;
    }
    q->queue[(q->rear) % q->n] = val;
    q->rear = (q->rear + 1) % q->n;
}

void popQueue(myRecurringQueue *q) {
    /* 出队 */
    if (isEmpty(q)) {
        printf("The queue is empty. You can't pop the queue.\n");
        return ;
    }
    q->front = (q->front + 1) % q->n;
}

void printQueue(myRecurringQueue *q) {
    /* 打印队列 */
    int i = 0, num = countQueue(q);
    for (; i < num; i++) {
        printf("%d ", q->queue[(q->front + i)%q->n]);
    }
    printf("\n");
}