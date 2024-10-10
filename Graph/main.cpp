// Задача 1. Дано бинарное дерево.Найти поддеревья с максимальным и минимальным соотношением(высота / число листьев).
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<chrono>
#include<cstdio>
#include<sstream>
#include<vector>

// структура Node представляет собой один узел в бинарном дереве и включает в себя значение узла,
// значение отношения(высота / число листьев) и указатели на двух потомков
struct Node {
	int value;
    double ratio;
	Node* left_child;
	Node* right_child;
    Node() : left_child(nullptr), right_child(nullptr), value(0), ratio(0) {}
};

// добавление узла в дерево с конкретным значением в качестве параметра функции, выполняется
// произвольно на основе функции rand()
void add_node(int val, Node*& node) {
    if (node != nullptr) {
        switch (rand() % 2) {
            case 0:
                add_node(val, node->left_child);
                break;
            case 1:
                add_node(val, node->right_child);
                break;
        }
    }
	else {
		node = new Node;
		node->value = val;
	}
}

// расчет высоты дерева, корнем которого является переданный в функцию указатель на узел
int tree_height(Node*& root) {
    if (root == nullptr) return -1;

    int left_height = tree_height(root->left_child);
    int right_height = tree_height(root->right_child);

    return std::max(left_height, right_height) + 1;
}

// расчет кол-ва листьев дерева, корнем которого является переданный в функцию указатель на узел
int leaves_count(Node*& root) {
    int leaves_cnt = 0;
    
    if (root == nullptr) return 0;

    else if (root->left_child == nullptr && root->right_child == nullptr)
        leaves_cnt++;

    return leaves_cnt + leaves_count(root->left_child) + leaves_count(root->right_child);
}

// расчет отношения(высота / число листьев) для всех поддеревьев исходного дерева
std::pair<double, double> max_min_ratios(Node*& root) {
    static double min_ratio = INT_MAX, max_ratio = 0;    
    if (root == nullptr) return { 0, 0 }; 

    double ratio = static_cast<double>(tree_height(root)) / (leaves_count(root));
    root->ratio = ratio;

    if (ratio > max_ratio) max_ratio = ratio;
    if (ratio < min_ratio) min_ratio = ratio;

    max_min_ratios(root->left_child);
    max_min_ratios(root->right_child);
    
    return { max_ratio, min_ratio };
}

// очистка выделенной на указатели узлов дерева памяти
void clear_tree(Node*& root) {
    if (root == nullptr) return;

    clear_tree(root->left_child);
    clear_tree(root->right_child);
    delete root;
}

// поиск деревьев с макс. и мин. соотношениями(высота / число листьев), а именно их корневых узлов
std::pair<Node*, Node*> find_max_min_ratio_nodes(Node*& root, double max_ratio, double min_ratio) {
    static Node* max_result = nullptr, *min_result = nullptr;

    if (root == nullptr) return { max_result, min_result };
    if (root->ratio == max_ratio) max_result = root;
    if (root->ratio == min_ratio) min_result = root;

    find_max_min_ratio_nodes(root->left_child, max_ratio, min_ratio);
    find_max_min_ratio_nodes(root->right_child, max_ratio, min_ratio);
    return { max_result, min_result };
}

// вывод дерева в консоль, корнем которого является переданный в функцию указатель на узел
void print_tree(const std::string& prefix, Node*& node, bool isLeft) {
    if (node) {
        std::cout << prefix << (isLeft ? "|--" : "|__") << node->value << std::endl;
        print_tree(prefix + (isLeft ? "|   " : "    "), node->right_child, true);
        print_tree(prefix + (isLeft ? "|   " : "    "), node->left_child, false);
    }
}

// вывод дерева в консоль по заданному файлу
Node* create_custom_tree(std::ifstream& tree_file, int is_left, int is_right) {
    std::string node_line;
    std::getline(tree_file, node_line);
    std::istringstream temp_node(node_line);

    int val = 0, lc = 0, rc = 0;
    temp_node >> val >> lc >> rc;
    Node* root = new Node;
    root->value = val;

    if (lc == 1) root->left_child = create_custom_tree(tree_file, lc, rc);
    if (rc == 1) root->right_child = create_custom_tree(tree_file, lc, rc);
    return root;
}

void generate_file(const std::string& filename, int count) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Невозможно открыть файл " << filename << std::endl;
        return;
    }

    int rand_num = 0;
    for (int i = 0; i < count; ++i) {
        rand_num = rand() % 10;
        file << rand_num << std::endl;
    }

    file.close();
}

int main() {
    srand(time(0));
    setlocale(LC_ALL, "ru");

    Node* root = nullptr;
    
    std::cout << "\t\t\t\t----------- Информация по программе -----------\n\n"
              << "Для работы с деревом выберете способ его создания:\n\n"
              << "1 - произвольное построение по заданному кол-ву узлов\n"
              << "2 - построение по заданному шаблону с конкретными вершинами\n";

    int choice = 0;
    std::cin >> choice;
    std::string file_name;
    int counts_of_nodes = 0;

    if (choice == 1) {
        std::cout << "Укажите кол-во создаваемых узлов: ";
        std::cin >> counts_of_nodes;

        file_name = "num" + std::to_string(counts_of_nodes) + ".txt";
        generate_file(file_name, counts_of_nodes);
        std::cout << "\nФайл сохранен с именем " << file_name << ".\n";

        std::ifstream in_file(file_name);
        int num = 0;
        while (in_file >> num) add_node(num, root);
        print_tree("", root, false);
    }
    else if (choice == 2) {
        std::cout << "Введите каждый узел на новой строке в формате val lc rc, где\n"
                  << "val - значение узла, lc(rc) = 1, если левый(правый) потомок\nесть и 0 в противном случае\n\n"
                  << "*создание дерева осуществляется в прямом порядке*" 
                  << std::endl;

        std::string node_line;
        std::vector<std::string> nodes;
        std::ofstream custom("custom_1.txt");
        std::cin.get();

        while (std::getline(std::cin, node_line)) {
            nodes.push_back(node_line);
            if (node_line == "-1") break;
        }

        nodes.pop_back();
        for (const std::string& node : nodes) {
            custom << node << std::endl;
        }

        std::cout << "\nФайл сохранен с именем custom_1.txt" << ".\n";
        std::ifstream custom1("custom_1.txt");
        root = create_custom_tree(custom1, 0, 0);
    }
    else {
        std::cout << "Некорректный ввод, программа завершена";
        return -1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = max_min_ratios(root);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::printf("Время выполнения алгоритма для %d узлов: ", counts_of_nodes);
    std::cout << execution_time << " мкс." << std::endl;
    

    Node* max_root = find_max_min_ratio_nodes(root, result.first, result.second).first;
    Node* min_root = find_max_min_ratio_nodes(root, result.first, result.second).second;

    
    std::cout << "Максимальное соотношение: " << result.first << "\nМинимальное соотношение: " << result.second << std::endl;
    std::cout << "Поддерево с максимальным соотношением:" << std::endl;
    print_tree("", max_root, false);

    std::cout << "Поддерево с минимальным соотношением:" << std::endl;
    print_tree("", min_root, false);

    clear_tree(root);

    return 0;
}
