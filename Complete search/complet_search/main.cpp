//#include <iostream>
//using namespace std;
//#include <vector>
//#include <fstream>
//
//void show_one_dim(const vector<int>& vec) {
//  for (int i = 0; i < vec.size(); i++) {
//    cout << vec[i] + 1 << " ";
//  }
//}
//
//void write_to_f (vector<int>&vec, fstream &file) {
//    for (int i = 0; i < vec.size(); i++) {
//      file << vec[i] + 1 << " ";
//    }
//    file << endl;
//}
//
//
//int main() {
//    fstream file("/Users/ilya/Downloads/Типы и структуры данных 2 курс, 1 семестр/complet_search/permutations.txt");
//    int n;
//    int counter = 0;
//    cin >> n;
//    vector<int> free;
//    vector<int> ans;
//    for (int i = 0; i < n; i++) {
//        ans.push_back(i);
//    }
//    int last;
//    last = ans[n - 1];
//    //show_one_dim(ans);
//    //cout << endl;
//    write_to_f(ans, file);
//    ans.pop_back();
//    free.push_back(last);
//    counter++;
//    while (1) {
//        int new_el = n + 1;
//        for (int i = 0; i < free.size(); i++) {
//            if (free[i] > last) {
//                new_el = min(free[i], new_el);
//            }
//        }
//        if (new_el == n + 1) {
//            if (ans.empty()){
//                //show_one_dim(free);
//                write_to_f(ans, file);
//                break;
//            }
//            last = ans[ans.size() - 1];
//            ans.pop_back();
//            free.push_back(last);
//        }
//        else {
//            ans.push_back(new_el);
//            erase(free, new_el);
//            sort(free.begin(), free.end());
//            for (int i = 0; i < free.size(); i++) {
//                ans.push_back(free[i]);
//            }
//            free.clear();
////            show_one_dim(ans);
////            cout << endl;
//            write_to_f(ans, file);
//            counter++;
//        }
//    }
//    cout << endl;
//    cout << counter << endl;
//    file.close();
//    return 0;
//}
//


#include <iostream>
#include <vector>

struct TreeNode {
    int value;
    std::vector<TreeNode*> children;

    TreeNode(int val) : value(val) {}
};

// Функция для создания тернарного дерева (пример для данной задачи)
TreeNode* createSampleTree() {
    TreeNode* root = new TreeNode(1);
    root->children.push_back(new TreeNode(2));
    root->children.push_back(new TreeNode(3));
    root->children.push_back(new TreeNode(4));

    root->children[0]->children.push_back(new TreeNode(5));
    root->children[0]->children.push_back(new TreeNode(6));
    root->children[0]->children.push_back(new TreeNode(7));

    return root;
}

// Рекурсивная функция для полного перебора маршрутов
void traverse(TreeNode* node, std::vector<int>& currentPath, std::vector<std::vector<int>>& allPaths) {
    currentPath.push_back(node->value);

    if (node->children.empty()) {
        // Если достигнут лист, сохраняем маршрут
        allPaths.push_back(currentPath);
    } else {
        // Рекурсивно обходим детей
        for (TreeNode* child : node->children) {
            traverse(child, currentPath, allPaths);
        }
    }

    // Возвращаемся на уровень выше при обратном проходе
    currentPath.pop_back();
}

// Функция для поиска самого длинного маршрута
std::vector<int> findLongestPath(const std::vector<std::vector<int>>& allPaths) {
    std::vector<int> longestPath;

    for (const auto& path : allPaths) {
        if (path.size() > longestPath.size()) {
            longestPath = path;
        }
    }

    return longestPath;
}

int main() {
    TreeNode* root = createSampleTree();

    std::vector<std::vector<int>> allPaths;
    std::vector<int> currentPath;

    // Начинаем полный перебор с корня дерева
    traverse(root, currentPath, allPaths);

    // Выводим все маршруты
    std::cout << "All paths:\n";
    for (const auto& path : allPaths) {
        for (int nodeValue : path) {
            std::cout << nodeValue << " ";
        }
        std::cout << "\n";
    }

    // Находим и выводим самый длинный маршрут
    std::vector<int> longestPath = findLongestPath(allPaths);
    std::cout << "\nLongest path:\n";
    for (int nodeValue : longestPath) {
        std::cout << nodeValue << " ";
    }

    // Очищаем выделенную память
    // (в реальном коде обязательно нужно учесть удаление всех узлов дерева)
    delete root;

    return 0;
}
