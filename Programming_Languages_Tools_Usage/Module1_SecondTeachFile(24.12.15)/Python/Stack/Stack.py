class Stack:
    def __init__(self, n):
        """初始化栈，指定容量"""
        self.stack = [None] * n
        self.rear = 0
        self.capacity = n

    def push(self, val):
        """入栈操作"""
        if self.rear == self.capacity:
            print("栈空间已满，无法push")
            return
        self.stack[self.rear] = val
        self.rear += 1

    def pop(self):
        """出栈操作"""
        if self.rear <= 0:
            print("栈空间为空，无法pop")
            return
        self.rear -= 1

    def print_stack(self):
        """打印栈内容"""
        for i in range(self.rear):
            print(self.stack[i], end=" ")
        print()

# 示例用法
if __name__ == "__main__":
    n = 4
    stack = Stack(n)
    stack.push(1)
    stack.push(2)
    stack.push(3)
    stack.push(4)
    stack.print_stack()  # 输出: 1 2 3 4
    stack.push(5)        # 输出: 栈空间已满，无法push
    stack.pop()
    stack.pop()
    stack.print_stack()  # 输出: 1 2
    stack.push(6)
    stack.print_stack()  # 输出: 1 2 6
