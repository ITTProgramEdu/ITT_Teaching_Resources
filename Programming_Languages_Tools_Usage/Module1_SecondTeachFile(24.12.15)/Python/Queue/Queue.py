class Queue:
    def __init__(self, size):
        self.queue = [None] * size
        self.rear = 0
        self.size = size

    def push(self, val):
        """入队操作"""
        if self.rear >= self.size:
            print("队列已满")
            return
        self.queue[self.rear] = val
        self.rear += 1

    def pop(self):
        """出队操作"""
        if self.rear <= 0:
            print("队列为空")
            return
        for i in range(self.rear - 1):
            self.queue[i] = self.queue[i + 1]
        self.queue[self.rear - 1] = None
        self.rear -= 1

    def print_queue(self):
        """打印队列"""
        print(" ".join(str(self.queue[i]) for i in range(self.rear) if self.queue[i] is not None))

# 示例用法
if __name__ == "__main__":
    n = 2
    q = Queue(n)
    q.push(1)
    q.push(2)
    q.push(3)  # 队列已满
    q.push(4)  # 队列已满
    q.print_queue()
    q.pop()
    q.pop()
    q.print_queue()
