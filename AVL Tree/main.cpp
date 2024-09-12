#include <iostream>
#include <algorithm>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;
    int height;
};

class AVLTree {
private:
    Node* root;

    int Height(Node* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int BalanceFactor(Node* node) {
        if (node == nullptr)
            return 0;
        return Height(node->left) - Height(node->right);
    }

    Node* rotateRight(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;

        x->right = y;
        y->left = T2;

        y->height = max(Height(y->left), Height(y->right)) + 1;
        x->height = max(Height(x->left), Height(x->right)) + 1;

        return x;
    }

    Node* rotateLeft(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;

        y->left = x;
        x->right = T2;

        x->height = max(Height(x->left), Height(x->right)) + 1;
        y->height = max(Height(y->left), Height(y->right)) + 1;

        return y;
    }

    Node* balance(Node* node) {
        if (node == nullptr)
            return nullptr;

        // Обновляет вес
        node->height = max(Height(node->left), Height(node->right)) + 1;

        int balance = BalanceFactor(node);
        
        if (balance < -1 && BalanceFactor(node->right) <= 0)
            return rotateLeft(node);

        if (balance > 1 && BalanceFactor(node->left) >= 0)
            return rotateRight(node);
        
        if (balance < -1 && BalanceFactor(node->right) > 0) {
            node->right = rotateRight(node->right);
            return rotateLeft(node);
        }

        if (balance > 1 && BalanceFactor(node->left) < 0) {
            node->left = rotateLeft(node->left);
            return rotateRight(node);
        }

        return node;
    }
    

    Node* insert(Node* node, int key) {
        if (node == nullptr)
            return new Node{key, nullptr, nullptr, 1};

        if (key < node->data)
            node->left = insert(node->left, key);
        else if (key > node->data)
            node->right = insert(node->right, key);
        else
            return node;

        node->height = 1 + max(Height(node->left), Height(node->right));

        return balance(node);
    }

    Node* minValueNode(Node* node) {
        Node* current = node;
        while (current->left != nullptr)
            current = current->left;
        return current;
    }

    Node* deleteNode(Node* root, int key) {
        if (root == nullptr)
            return root;

        if (key < root->data)
            root->left = deleteNode(root->left, key);
        else if (key > root->data)
            root->right = deleteNode(root->right, key);
        else {
            if (root->left == nullptr || root->right == nullptr) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == nullptr) {
                    temp = root;
                    root = nullptr;
                } else
                    *root = *temp;

                delete temp;
            } else {
                Node* temp = minValueNode(root->right);

                root->data = temp->data;

                root->right = deleteNode(root->right, temp->data);
            }
        }

        if (root == nullptr)
            return root;
        root->height = 1 + max(Height(root->left), Height(root->right));

        return balance(root);
    }

public:
    AVLTree() : root(nullptr) {}

    void insert(int key) {
        root = insert(root, key);
    }

    void remove(int key) {
        root = deleteNode(root, key);
    }

    bool search(int key) {
        Node* current = root;
        while (current != nullptr) {
            if (key == current->data)
                return true;
            else if (key < current->data)
                current = current->left;
            else
                current = current->right;
        }
        return false;
    }

    void displayMenu() {
        cout << "\nМеню АВЛ-дерева:\n";
        cout << "1 - Показать дерево\n";
        cout << "2 - Вставка ключа\n";
        cout << "3 - Удаление ключа\n";
        cout << "4 - Поиск ключа\n";
        cout << "5 - Завершение\n";
    }

    void displayTree(Node* root, int space) {
        const int INDENT = 5;
        if (root == nullptr)
            return;

        space += INDENT;

        displayTree(root->right, space);

        cout << endl;
        for (int i = INDENT; i < space; i++)
            cout << " ";

        cout << root->data << "\n";

        displayTree(root->left, space);
    }

    Node* Root() {
        return root;
    }
};

int main() {
    AVLTree avl;

    int choice, key;

    do {
        avl.displayMenu();
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
            case 1:
                avl.displayTree(avl.Root(), 0);
                break;
            case 2:
                cout << "Введите ключ, который хотите добавить: ";
                cin >> key;
                avl.insert(key);
                break;
            case 3:
                cout << "Введите ключ, который хотите удалить: ";
                cin >> key;
                avl.remove(key);
                break;
            case 4:
                cout << "Введите ключ, который хотите найти: ";
                cin >> key;
                if (avl.search(key)) {
                    cout << "Ключ найден\n";
                } else {
                    cout << "Ключ не найден\n";
                }
                break;
            case 5:
                cout << "Завершение программы\n";
                break;
            default:
                cout << "Неверный выбор из меню\n";
        }
    } while (choice != 5);

    return 0;
}
