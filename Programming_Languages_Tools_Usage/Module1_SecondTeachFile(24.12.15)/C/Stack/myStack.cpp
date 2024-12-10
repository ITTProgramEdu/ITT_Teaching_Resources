#include <stdio.h>
#include <stdlib.h>

void pushStack(int *stack, int *rear, int n, int val);
void popStack(int *stack, int *rear);
void printStack(int *stack, int rear);

int main() {
    int n = 4;
    // scanf("%d", &n);
    int stack[n];
    int rear = 0; 
    pushStack(stack, &rear, n, 1);
    pushStack(stack, &rear, n, 2);
    pushStack(stack, &rear, n, 3);
    pushStack(stack, &rear, n, 4);
    printStack(stack, rear);
    pushStack(stack, &rear, n, 5);
    popStack(stack, &rear);
    popStack(stack, &rear);
    printStack(stack, rear);
    pushStack(stack, &rear, n, 6);
    printStack(stack, rear);

    return 0;
}

void pushStack(int *stack, int *rear, int n, int val) {
    if (*rear == n) {
        printf("栈空间已满, 无法push\n");
        return ;
    }
    stack[(*rear)++] = val;
}

void popStack(int *stack, int *rear) {
    if (rear <= 0) {
        printf("栈空间为空, 无法pop\n");
        return ;
    }
    (*rear)--;
}

void printStack(int *stack, int rear) {
    int i = 0;
    for (; i < rear; i++) {
        printf("%d ", stack[i]);
    }
    printf("\n");
}