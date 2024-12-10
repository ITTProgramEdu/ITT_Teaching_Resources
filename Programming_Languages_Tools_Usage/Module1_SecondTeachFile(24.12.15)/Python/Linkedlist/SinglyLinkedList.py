class SinglyLinkedList:
    class ListNode:
        def __init__(self, value=-1):
            self.value = value
            self.next = None

    def __init__(self):
        self.head = self.ListNode()  # Dummy head node

    def insert_item(self, val, pos=1):
        """在指定位置插入节点"""
        node = self.ListNode(val)
        p = self.head
        # 如果 pos 太大，超出限制，插入到最后
        while pos > 1 and p.next:
            p = p.next
            pos -= 1
        node.next = p.next
        p.next = node

    def remove_item(self, val):
        """删除第一个值为 val 的节点"""
        p = self.head.next
        q = self.head
        while p and p.value != val:
            p = p.next
            q = q.next
        if p:
            # 删除节点
            q.next = p.next
        else:
            print(f"{val} 不存在")

    def find_item(self, val):
        """查找第一个值为 val 的节点"""
        p = self.head.next
        while p and p.value != val:
            p = p.next
        if p:
            return p
        else:
            print(f"{val} 不存在")
            return None

    def print_linked_list(self):
        """打印链表"""
        p = self.head.next
        while p:
            print(p.value, end=" ")
            p = p.next
        print()

# 示例用法
if __name__ == "__main__":
    sll = SinglyLinkedList()
    sll.insert_item(1)
    sll.insert_item(2, 2)
    sll.insert_item(3, 3)
    sll.insert_item(4, 4)

    target1 = sll.find_item(2)
    if target1:
        print(f"找到的这个Item的值为 {target1.value}", end="")
        if target1.next:
            print(f", 它的下一个值为 {target1.next.value}")
        else:
            print(", 它是最后一个节点。")

    target2 = sll.find_item(5)
    if target2:
        print(f"找到的这个Item的值为 {target2.value}", end="")
        if target2.next:
            print(f", 它的下一个值为 {target2.next.value}")
        else:
            print(", 它是最后一个节点。")

    sll.remove_item(3)
    sll.remove_item(5)
    sll.print_linked_list()
