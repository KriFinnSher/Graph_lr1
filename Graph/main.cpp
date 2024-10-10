// ����� 1. ���� ����୮� ��ॢ�.���� �����ॢ�� � ���ᨬ���� � ��������� ᮮ⭮襭���(���� / �᫮ ����쥢).
#include<iostream>
#include<fstream>
#include<string>
#include<ctime>
#include<chrono>
#include<cstdio>
#include<sstream>
#include<vector>

// ������� Node �।�⠢��� ᮡ�� ���� 㧥� � ����୮� ��ॢ� � ����砥� � ᥡ� ���祭�� 㧫�,
// ���祭�� �⭮襭��(���� / �᫮ ����쥢) � 㪠��⥫� �� ���� ��⮬���
struct Node {
	int value;
    double ratio;
	Node* left_child;
	Node* right_child;
    Node() : left_child(nullptr), right_child(nullptr), value(0), ratio(0) {}
};

// ���������� 㧫� � ��ॢ� � ������� ���祭��� � ����⢥ ��ࠬ��� �㭪樨, �믮������
// �ந����쭮 �� �᭮�� �㭪樨 rand()
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

// ���� ����� ��ॢ�, ��୥� ���ண� ���� ��।���� � �㭪�� 㪠��⥫� �� 㧥�
int tree_height(Node*& root) {
    if (root == nullptr) return -1;

    int left_height = tree_height(root->left_child);
    int right_height = tree_height(root->right_child);

    return std::max(left_height, right_height) + 1;
}

// ���� ���-�� ����쥢 ��ॢ�, ��୥� ���ண� ���� ��।���� � �㭪�� 㪠��⥫� �� 㧥�
int leaves_count(Node*& root) {
    int leaves_cnt = 0;
    
    if (root == nullptr) return 0;

    else if (root->left_child == nullptr && root->right_child == nullptr)
        leaves_cnt++;

    return leaves_cnt + leaves_count(root->left_child) + leaves_count(root->right_child);
}

// ���� �⭮襭��(���� / �᫮ ����쥢) ��� ��� �����ॢ쥢 ��室���� ��ॢ�
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

// ���⪠ �뤥������ �� 㪠��⥫� 㧫�� ��ॢ� �����
void clear_tree(Node*& root) {
    if (root == nullptr) return;

    clear_tree(root->left_child);
    clear_tree(root->right_child);
    delete root;
}

// ���� ��ॢ쥢 � ����. � ���. ᮮ⭮襭�ﬨ(���� / �᫮ ����쥢), � ������ �� ��୥��� 㧫��
std::pair<Node*, Node*> find_max_min_ratio_nodes(Node*& root, double max_ratio, double min_ratio) {
    static Node* max_result = nullptr, *min_result = nullptr;

    if (root == nullptr) return { max_result, min_result };
    if (root->ratio == max_ratio) max_result = root;
    if (root->ratio == min_ratio) min_result = root;

    find_max_min_ratio_nodes(root->left_child, max_ratio, min_ratio);
    find_max_min_ratio_nodes(root->right_child, max_ratio, min_ratio);
    return { max_result, min_result };
}

// �뢮� ��ॢ� � ���᮫�, ��୥� ���ண� ���� ��।���� � �㭪�� 㪠��⥫� �� 㧥�
void print_tree(const std::string& prefix, Node*& node, bool isLeft) {
    if (node) {
        std::cout << prefix << (isLeft ? "|--" : "|__") << node->value << std::endl;
        print_tree(prefix + (isLeft ? "|   " : "    "), node->right_child, true);
        print_tree(prefix + (isLeft ? "|   " : "    "), node->left_child, false);
    }
}

// �뢮� ��ॢ� � ���᮫� �� ��������� 䠩��
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
        std::cerr << "���������� ������ 䠩� " << filename << std::endl;
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
    
    std::cout << "\t\t\t\t----------- ���ଠ�� �� �ணࠬ�� -----------\n\n"
              << "��� ࠡ��� � ��ॢ�� �롥�� ᯮᮡ ��� ᮧ�����:\n\n"
              << "1 - �ந����쭮� ����஥��� �� ��������� ���-�� 㧫��\n"
              << "2 - ����஥��� �� ��������� 蠡���� � ������묨 ���設���\n";

    int choice = 0;
    std::cin >> choice;
    std::string file_name;
    int counts_of_nodes = 0;

    if (choice == 1) {
        std::cout << "������ ���-�� ᮧ�������� 㧫��: ";
        std::cin >> counts_of_nodes;

        file_name = "num" + std::to_string(counts_of_nodes) + ".txt";
        generate_file(file_name, counts_of_nodes);
        std::cout << "\n���� ��࠭�� � ������ " << file_name << ".\n";

        std::ifstream in_file(file_name);
        int num = 0;
        while (in_file >> num) add_node(num, root);
        print_tree("", root, false);
    }
    else if (choice == 2) {
        std::cout << "������ ����� 㧥� �� ����� ��ப� � �ଠ� val lc rc, ���\n"
                  << "val - ���祭�� 㧫�, lc(rc) = 1, �᫨ ����(�ࠢ�) ��⮬��\n���� � 0 � ��⨢��� ��砥\n\n"
                  << "*ᮧ����� ��ॢ� �����⢫���� � ��אַ� ���浪�*" 
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

        std::cout << "\n���� ��࠭�� � ������ custom_1.txt" << ".\n";
        std::ifstream custom1("custom_1.txt");
        root = create_custom_tree(custom1, 0, 0);
    }
    else {
        std::cout << "�����४�� ����, �ணࠬ�� �����襭�";
        return -1;
    }

    auto start_time = std::chrono::high_resolution_clock::now();
    auto result = max_min_ratios(root);
    auto end_time = std::chrono::high_resolution_clock::now();

    auto execution_time = std::chrono::duration_cast<std::chrono::microseconds>(end_time - start_time).count();

    std::printf("�६� �믮������ �����⬠ ��� %d 㧫��: ", counts_of_nodes);
    std::cout << execution_time << " ���." << std::endl;
    

    Node* max_root = find_max_min_ratio_nodes(root, result.first, result.second).first;
    Node* min_root = find_max_min_ratio_nodes(root, result.first, result.second).second;

    
    std::cout << "���ᨬ��쭮� ᮮ⭮襭��: " << result.first << "\n�������쭮� ᮮ⭮襭��: " << result.second << std::endl;
    std::cout << "�����ॢ� � ���ᨬ���� ᮮ⭮襭���:" << std::endl;
    print_tree("", max_root, false);

    std::cout << "�����ॢ� � ��������� ᮮ⭮襭���:" << std::endl;
    print_tree("", min_root, false);

    clear_tree(root);

    return 0;
}