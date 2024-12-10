class CircularQueue:
    def __init__(self, n):
        """初始化循环队列，队列容量为 n+1"""
        self.queue = [None] * (n + 1)
        self.front = 0
        self.rear = 0
        self.size = n + 1  # 队列多一个空间用来区分满队列和空队列

    def is_empty(self):
        """判断队列是否为空"""
        return self.front == self.rear

    def is_full(self):
        """判断队列是否为满"""
        return (self.rear + 1) % self.size == self.front

    def count(self):
        """计算队列中元素个数"""
        return (self.rear - self.front + self.size) % self.size

    def push(self, val):
        """入队操作"""
        if self.is_full():
            print("The queue is full. You can't re-enter the queue.")
            return
        self.queue[self.rear] = val
        self.rear = (self.rear + 1) % self.size

    def pop(self):
        """出队操作"""
        if self.is_empty():
            print("The queue is empty. You can't pop the queue.")
            return
        self.front = (self.front + 1) % self.size

    def print_queue(self):
        """打印队列中的元素"""
        num = self.count()
        for i in range(num):
            print(self.queue[(self.front + i) % self.size], end=" ")
        print()

# 示例用法
if __name__ == "__main__":
    n = 4
    q = CircularQueue(n)
    q.pop()  # 队列为空
    q.push(1)
    q.push(2)
    q.push(3)
    q.push(4)
    q.print_queue()
    q.push(5)  # 队列已满
    q.print_queue()
    q.pop()
    q.pop()
    q.push(6)
    q.print_queue()
    q.pop()
    q.print_queue()
