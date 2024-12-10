class BiNode:
    """定义二叉树节点"""
    def __init__(self, value):
        self.value = value
        self.lchild = None
        self.rchild = None


class BinarySearchTree:
    """定义二叉搜索树类"""
    def __init__(self):
        self.root = None

    def insert(self, val):
        """插入节点"""
        def _insert(node, val):
            if not node:
                return BiNode(val)
            if val < node.value:
                node.lchild = _insert(node.lchild, val)
            else:
                node.rchild = _insert(node.rchild, val)
            return node

        self.root = _insert(self.root, val)

    def find_left_max(self, node):
        """查找左子树最大节点的父节点"""
        p = node
        c = node.lchild
        while c and c.rchild:
            p = c
            c = c.rchild
        return p

    def find_right_min(self, node):
        """查找右子树最小节点的父节点"""
        p = node
        c = node.rchild
        while c and c.lchild:
            p = c
            c = c.lchild
        return p

    def remove(self, val):
        """删除节点"""
        def _remove(node, val, parent):
            if not node:
                print(f"{val}节点不存在")
                return None
            if val < node.value:
                node.lchild = _remove(node.lchild, val, node)
            elif val > node.value:
                node.rchild = _remove(node.rchild, val, node)
            else:
                if node.lchild:  # 左子树存在
                    max_parent = self.find_left_max(node)
                    max_node = max_parent.rchild if max_parent != node else max_parent.lchild
                    node.value = max_node.value
                    if max_parent == node:
                        node.lchild = max_node.lchild
                    else:
                        max_parent.rchild = max_node.lchild
                elif node.rchild:  # 右子树存在
                    min_parent = self.find_right_min(node)
                    min_node = min_parent.lchild if min_parent != node else min_parent.rchild
                    node.value = min_node.value
                    if min_parent == node:
                        node.rchild = min_node.rchild
                    else:
                        min_parent.lchild = min_node.rchild
                else:  # 叶子节点
                    if parent and parent.lchild == node:
                        parent.lchild = None
                    elif parent and parent.rchild == node:
                        parent.rchild = None
                    return None
            return node

        self.root = _remove(self.root, val, None)

    def print_tree(self):
        """中序遍历打印树"""
        def _print(node):
            if not node:
                return
            _print(node.lchild)
            print(node.value, end=" ")
            _print(node.rchild)

        _print(self.root)
        print()


# 示例用法
if __name__ == "__main__":
    bst = BinarySearchTree()
    bst.insert(4)
    bst.insert(2)
    bst.insert(3)
    bst.insert(1)
    bst.insert(0)
    bst.insert(6)
    bst.insert(8)
    bst.insert(7)
    bst.print_tree()
    bst.remove(2)
    bst.print_tree()
