class CircularLinkedList:
    class ListNode:
        def __init__(self, value=-1):
            self.value = value
            self.next = self  # 循环链表的空节点指向自己

    def __init__(self):
        self.head = self.ListNode()  # 初始化循环链表的头节点

    def insert_item(self, val, pos=1):
        """在指定位置插入节点，默认插入到起始位置"""
        node = self.ListNode(val)
        p = self.head
        # 如果 pos 太大，则插入到最后
        while pos > 1 and p.next != self.head:
            p = p.next
            pos -= 1
        node.next = p.next
        p.next = node

    def remove_item(self, val):
        """删除第一个值为 val 的节点"""
        p = self.head.next
        q = self.head
        while p != self.head and p.value != val:
            p = p.next
            q = q.next
        if p != self.head:
            # 删除节点
            q.next = p.next
        else:
            print(f"删除 {val} 不存在")

    def find_item(self, val):
        """查找第一个值为 val 的节点"""
        p = self.head.next
        while p != self.head and p.value != val:
            p = p.next
        if p != self.head:
            return p
        else:
            print(f"寻找 {val} 不存在")
            return None

    def print_linked_list(self):
        """打印链表中的所有节点值"""
        p = self.head.next
        while p != self.head:
            print(p.value, end=" ")
            p = p.next
        print()

# 示例用法
if __name__ == "__main__":
    cll = CircularLinkedList()
    cll.insert_item(1)
    cll.insert_item(2, 2)
    cll.insert_item(3, 3)
    cll.insert_item(4, 6)  # 插入到链表尾部
    cll.print_linked_list()

    target1 = cll.find_item(2)
    if target1:
        print(f"找到的这个Item的值为 {target1.value}", end="")
        if target1.next != cll.head:
            print(f", 它的下一个值为 {target1.next.value}")
        else:
            print(", 它是最后一个节点。")

    target2 = cll.find_item(5)
    if target2:
        print(f"找到的这个Item的值为 {target2.value}", end="")
        if target2.next != cll.head:
            print(f", 它的下一个值为 {target2.next.value}")
        else:
            print(", 它是最后一个节点。")

    cll.remove_item(3)
    cll.remove_item(5)
    cll.print_linked_list()
