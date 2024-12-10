class DoublyLinkedList:
    class ListNode:
        def __init__(self, value=0):
            self.value = value
            self.next = None
            self.prev = None

    def __init__(self):
        self.head = self.ListNode()  # Create a dummy head node

    def insert_item(self, val, pos=1):
        node = self.ListNode(val)
        p = self.head
        while pos > 1 and p.next:
            p = p.next
            pos -= 1
        node.next = p.next
        node.prev = p
        if p.next:
            p.next.prev = node
        p.next = node

    def remove_item(self, val):
        p = self.head.next
        while p and p.value != val:
            p = p.next
        if p:
            if p.next:
                p.next.prev = p.prev
            p.prev.next = p.next
        else:
            print(f"{val} is not found")

    def find_item(self, val):
        p = self.head.next
        while p and p.value != val:
            p = p.next
        if p:
            return p
        else:
            print(f"{val} is not found")
            return None

    def print_linked_list(self):
        p = self.head.next
        while p:
            print(p.value, end=" ")
            p = p.next
        print()

# Example usage
if __name__ == "__main__":
    dll = DoublyLinkedList()
    dll.insert_item(1)
    dll.insert_item(2, 2)
    dll.insert_item(3, 3)
    dll.insert_item(4, 4)

    target1 = dll.find_item(2)
    if target1:
        print(f"The value of this item found is {target1.value}", end="")
        if target1.next:
            print(f", Its next value is {target1.next.value}")
        else:
            print(", It's the last node.")

    target2 = dll.find_item(5)
    if target2:
        print(f"The value of this item found is {target2.value}", end="")
        if target2.next:
            print(f", Its next value is {target2.next.value}")
        else:
            print(", It's the last node.")

    dll.remove_item(3)
    dll.remove_item(5)
    dll.print_linked_list()
