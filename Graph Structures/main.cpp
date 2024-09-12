#include <iostream>
#include <string>
#include <queue>
#include <stack>

using namespace std;

template <typename T>
class BinaryTreeNode {
public:
    T data;
    BinaryTreeNode<T>* left;
    BinaryTreeNode<T>* right;

    BinaryTreeNode(T value) : data(value), left(nullptr), right(nullptr) {}
};

template <typename T>
class BinaryTree {
private:
    BinaryTreeNode<T>* root;

    void addElement(BinaryTreeNode<T>* node) {
        string choice;

        cout << "Добавить значение в левый узел для узла " << node->data << "? (да/нет): ";
        cin >> choice;

        if (choice == "да") {
            T value;
            cout << "Введите значение для левого узла: ";
            cin >> value;
            node->left = new BinaryTreeNode<T>(value);
            addElement(node->left);
        }
        else if (choice == "нет") {
            node->left = nullptr;
        }

        cout << "Добавить значение в правый узел для узла " << node->data << "? (да/нет): ";
        cin >> choice;

        if (choice == "да") {
            T value;
            cout << "Введите значение для правого узла: ";
            cin >> value;
            node->right = new BinaryTreeNode<T>(value);
            addElement(node->right);
        }
        else if (choice == "нет") {
            node->right = nullptr;
        }
    }

    void BreadthTree(BinaryTreeNode<T>* node) {
        if (node == nullptr) {
            return;
        }

        queue<BinaryTreeNode<T>*> nodesQueue;
        nodesQueue.push(node);

        while (!nodesQueue.empty()) {
            BinaryTreeNode<T>* current = nodesQueue.front();
            nodesQueue.pop();
            cout << current->data << " ";

            if (current->left != nullptr) {
                nodesQueue.push(current->left);
            }
            if (current->right != nullptr) {
                nodesQueue.push(current->right);
            }
        }
    }

    void DepthTree(BinaryTreeNode<T>* node) {
        if (node == nullptr) {
            return;
        }

        stack<BinaryTreeNode<T>*> nodesStack;
        nodesStack.push(node);

        while (!nodesStack.empty()) {
            BinaryTreeNode<T>* current = nodesStack.top();
            nodesStack.pop();
            std::cout << current->data << " ";

            if (current->right != nullptr) {
                nodesStack.push(current->right);
            }
            if (current->left != nullptr) {
                nodesStack.push(current->left);
            }
        }
    }

public:
    BinaryTree() : root(nullptr) {}

    void addElementToTree() {
        T value;
        cout << "Введите значение для корня дерева: ";
        cin >> value;
        root = new BinaryTreeNode<T>(value);
        addElement(root);
    }

    void printBreadthTree() {
        cout << "Обход дерева в ширину: ";
        BreadthTree(root);
        cout << endl;
    }

    void printDepthFirstTree() {
        cout << "Обход дерева в глубину: ";
        DepthTree(root);
        cout << endl;
    }
};

int main() {
    setlocale(0, "");
    BinaryTree<int> binaryTree;
    binaryTree.addElementToTree();
    cout << endl;
    binaryTree.printBreadthTree();
    cout << endl;
    binaryTree.printDepthFirstTree();
    return 0;
}
