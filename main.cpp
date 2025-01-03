#include <memory>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <algorithm>

template<typename T>
class AVLTree {
private:
    struct Node {
        T data;
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        int height;
        
        Node(const T& value) : data(value), height(1) {}
    };
    
    std::unique_ptr<Node> root;
    
    int getHeight(const Node* node) const {
        return node ? node->height : 0;
    }
    
    int getBalance(const Node* node) const {
        return node ? getHeight(node->left.get()) - getHeight(node->right.get()) : 0;
    }
    
    void updateHeight(Node* node) {
        node->height = 1 + std::max(getHeight(node->left.get()), 
                                  getHeight(node->right.get()));
    }
    
    std::unique_ptr<Node> rightRotate(std::unique_ptr<Node> y) {
        auto x = std::move(y->left);
        auto T2 = std::move(x->right);
        
        x->right = std::move(y);
        x->right->left = std::move(T2);
        
        updateHeight(x->right.get());
        updateHeight(x.get());
        
        return x;
    }
    
    std::unique_ptr<Node> leftRotate(std::unique_ptr<Node> x) {
        auto y = std::move(x->right);
        auto T2 = std::move(y->left);
        
        y->left = std::move(x);
        y->left->right = std::move(T2);
        
        updateHeight(y->left.get());
        updateHeight(y.get());
        
        return y;
    }
    
    std::unique_ptr<Node> insert(std::unique_ptr<Node> node, const T& value) {
        if (!node) {
            return std::make_unique<Node>(value);
        }
        
        if (value < node->data) {
            node->left = insert(std::move(node->left), value);
        } else if (value > node->data) {
            node->right = insert(std::move(node->right), value);
        } else {
            return node; // No dupe values
        }
        
        updateHeight(node.get());
        
        int balance = getBalance(node.get());
        
        // Left Left Case
        if (balance > 1 && value < node->left->data) {
            return rightRotate(std::move(node));
        }
        
        // Right Right Case
        if (balance < -1 && value > node->right->data) {
            return leftRotate(std::move(node));
        }
        
        // Left Right Case
        if (balance > 1 && value > node->left->data) {
            node->left = leftRotate(std::move(node->left));
            return rightRotate(std::move(node));
        }
        
        // Right Left Case
        if (balance < -1 && value < node->right->data) {
            node->right = rightRotate(std::move(node->right));
            return leftRotate(std::move(node));
        }
        
        return node;
    }
    
    void inorderTraversal(const Node* node, std::vector<T>& result) const {
        if (node) {
            inorderTraversal(node->left.get(), result);
            result.push_back(node->data);
            inorderTraversal(node->right.get(), result);
        }
    }

public:
    AVLTree() = default;
    
    void insert(const T& value) {
        root = insert(std::move(root), value);
    }
    
    std::vector<T> getInorder() const {
        std::vector<T> result;
        inorderTraversal(root.get(), result);
        return result;
    }
};

int main() {
    try {
        AVLTree<int> tree;
        std::vector<int> values = {10, 20, 30, 40, 50, 25};
        
        for (const auto& value : values) {
            tree.insert(value);
        }
        
        std::cout << "Inorder traversal: ";
        for (const auto& value : tree.getInorder()) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
