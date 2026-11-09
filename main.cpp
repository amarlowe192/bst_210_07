// Assignment 1: Binary Search Tree (C++17, no std::set / std::map)
#include <algorithm>
#include <iostream>
#include <vector>

struct Node {
    int key;
    Node* left;
    Node* right;
    explicit Node(int k) : key(k), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
public:
    BinarySearchTree() : root_(nullptr) {}
    ~BinarySearchTree() { destroy(root_); }
    BinarySearchTree(const BinarySearchTree&) = delete;
    BinarySearchTree& operator=(const BinarySearchTree&) = delete;

    // Returns false if the key already exists (duplicates are ignored).
    bool insert(int key) {
        bool inserted = false;
        root_ = insert(root_, key, inserted);
        return inserted;
    }

    bool search(int key) const {
        const Node* cur = root_;
        while (cur) {
            if (key == cur->key) return true;
            cur = (key < cur->key) ? cur->left : cur->right;
        }
        return false;
    }

    // Returns true if the key was found and removed.
    bool remove(int key) {
        bool removed = false;
        root_ = remove(root_, key, removed);
        return removed;
    }

    void inorder() const { print(root_, 0); }
    void preorder() const { print(root_, 1); }
    void postorder() const { print(root_, 2); }
    int height() const { return height(root_); }  // empty tree = 0, single node = 1

private:
    Node* root_;

    static void destroy(Node* n) {
        if (!n) return;
        destroy(n->left);
        destroy(n->right);
        delete n;
    }

    static Node* insert(Node* n, int key, bool& inserted) {
        if (!n) { inserted = true; return new Node(key); }
        if (key < n->key) n->left = insert(n->left, key, inserted);
        else if (key > n->key) n->right = insert(n->right, key, inserted);
        return n;
    }

    static Node* minNode(Node* n) {
        while (n->left) n = n->left;
        return n;
    }

    static Node* remove(Node* n, int key, bool& removed) {
        if (!n) return nullptr;
        if (key < n->key) {
            n->left = remove(n->left, key, removed);
        } else if (key > n->key) {
            n->right = remove(n->right, key, removed);
        } else {
            removed = true;
            if (!n->left) {            // case 1 (leaf) and case 2 (only right child)
                Node* r = n->right;
                delete n;
                return r;
            }
            if (!n->right) {           // case 2 (only left child)
                Node* l = n->left;
                delete n;
                return l;
            }
            // case 3: two children -> copy inorder successor, delete it from right subtree
            Node* succ = minNode(n->right);
            n->key = succ->key;
            bool dummy = false;
            n->right = remove(n->right, succ->key, dummy);
        }
        return n;
    }

    static int height(const Node* n) {
        if (!n) return 0;
        return 1 + std::max(height(n->left), height(n->right));
    }

    static void print(const Node* n, int order) {
        if (!n) return;
        if (order == 1) std::cout << n->key << ' ';
        print(n->left, order);
        if (order == 0) std::cout << n->key << ' ';
        print(n->right, order);
        if (order == 2) std::cout << n->key << ' ';
    }
};

static void showAll(const BinarySearchTree& t) {
    std::cout << "  inorder   : "; t.inorder();   std::cout << '\n';
    std::cout << "  preorder  : "; t.preorder();  std::cout << '\n';
    std::cout << "  postorder : "; t.postorder(); std::cout << '\n';
    std::cout << "  height    : " << t.height() << '\n';
}

int main() {
    BinarySearchTree t;
    std::vector<int> keys = {50, 30, 70, 20, 40, 60, 80, 35, 45, 65, 10, 25};

    std::cout << "=== Inserting " << keys.size() << " distinct integers ===\n  ";
    for (int k : keys) { t.insert(k); std::cout << k << ' '; }
    std::cout << "\n";
    showAll(t);

    std::cout << "\n=== Search ===\n";
    std::cout << "  search(60) -> " << (t.search(60) ? "FOUND" : "NOT FOUND") << '\n';
    std::cout << "  search(99) -> " << (t.search(99) ? "FOUND" : "NOT FOUND") << '\n';

    std::cout << "\n=== Removal case 1: leaf (remove 10) ===\n";
    t.remove(10);
    showAll(t);

    std::cout << "\n=== Removal case 2: one child (remove 20, which now has only right child 25) ===\n";
    t.remove(20);
    showAll(t);

    std::cout << "\n=== Removal case 3: two children (remove 30, successor 35 replaces it) ===\n";
    t.remove(30);
    showAll(t);

    std::cout << "\n=== Removal case 3 on the root (remove 50, successor 60 replaces it) ===\n";
    t.remove(50);
    showAll(t);

    std::cout << "\n=== Removing an absent key (remove 99) ===\n";
    std::cout << "  remove(99) -> " << (t.remove(99) ? "removed" : "not found, tree unchanged") << '\n';
    return 0;
}
