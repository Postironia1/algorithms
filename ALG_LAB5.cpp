#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <numeric>

using namespace std;

struct Node {
    int key;
    Node* left;
    Node* right;
    int height;
};

class BinarySearchTree {
private:
    Node* root;

    // Рекурсивная функция добавления узла
    Node* insert_helper_bst(Node* current_node, int key) {
        if (current_node == NULL) {
            Node* new_node = new Node;
            new_node->key = key;
            new_node->left = NULL;
            new_node->right = NULL;
            return new_node;
        }
        else if (key < current_node->key) {
            current_node->left = insert_helper_bst(current_node->left, key);
        }
        else if (key > current_node->key) {
            current_node->right = insert_helper_bst(current_node->right, key);
        }
        return current_node;
    }

    // Рекурсивная функция нахождения узла
    bool search_helper_bst(Node* current_node, int key) {
        if (current_node == NULL) {
            return false;
        }
        else if (key == current_node->key) {
            return true;
        }
        else if (key < current_node->key) {
            return search_helper_bst(current_node->left, key);
        }
        else {
            return search_helper_bst(current_node->right, key);
        }
    }

    Node* find_helper_bst(int key) {
        Node* current_node = this->root;
        while (current_node != nullptr) {
            if (current_node->key == key) {
                break;
            }
            current_node = current_node->key < key ? current_node->right : current_node->left;
        }
        return current_node;
    }

    // Рекурсивная функция удаления узла
    Node* remove_helper_bst(Node* current_node, int key) {
        if (current_node == NULL) {
            return NULL;
        }
        else if (key < current_node->key) {
            current_node->left = remove_helper_bst(current_node->left, key);
        }
        else if (key > current_node->key) {
            current_node->right = remove_helper_bst(current_node->right, key);
        }
        else {
            if (current_node->left == NULL && current_node->right == NULL) {
                delete current_node;
                current_node = NULL;
            }
            else if (current_node->left == NULL) {
                Node* temp = current_node;
                current_node = current_node->right;
                delete temp;
            }
            else if (current_node->right == NULL) {
                Node* temp = current_node;
                current_node = current_node->left;
                delete temp;
            }
            else {
                Node* min_right = current_node->right;
                while (min_right->left != NULL) {
                    min_right = min_right->left;
                }
                current_node->key = min_right->key;
                current_node->right = remove_helper_bst(current_node->right, min_right->key);
            }
        }
        return current_node;
    }

public:
    BinarySearchTree() {
        this->root = NULL;
    }

    void insert_bst(int key) {
        this->root = insert_helper_bst(root, key);
    }

    bool search_bst(int key) {
        return search_helper_bst(root, key);
    }

    void find_bst(int key) {
        this->root = find_helper_bst(key);
    }

    void remove_bst(int key) {
        this->root = remove_helper_bst(root, key);
    }
};


class AVLTree {
private:
    Node* root;

    int height(Node* N) {
        if (N == NULL)
            return 0;
        return N->height;
    }

    int max(int a, int b) {
        return (a > b) ? a : b;
    }

    // New node creation
        Node * newNode(int key) {
        Node* node = new Node();
        node->key = key;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
        return (node);
    }

    // Rotate right
    Node* rightRotate(Node* y) {
        Node* x = y->left;
        Node* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left),
            height(y->right)) +
            1;
        x->height = max(height(x->left),
            height(x->right)) +
            1;
        return x;
    }

    // Rotate left
    Node* leftRotate(Node* x) {
        Node* y = x->right;
        Node* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left),
            height(x->right)) +
            1;
        y->height = max(height(y->left),
            height(y->right)) +
            1;
        return y;
    }

    // Get the balance factor of each node
    int getBalanceFactor(Node* N) {
        if (N == NULL)
            return 0;
        return height(N->left) -
            height(N->right);
    }

    // Insert a node
    Node* insertNode(Node* node, int key) {
        // Find the correct postion and insert the node
        if (node == NULL)
            return (newNode(key));
        if (key < node->key)
            node->left = insertNode(node->left, key);
        else if (key > node->key)
            node->right = insertNode(node->right, key);
        else
            return node;

        // Update the balance factor of each node and
        // balance the tree
        node->height = 1 + max(height(node->left),
            height(node->right));
        int balanceFactor = getBalanceFactor(node);
        if (balanceFactor > 1) {
            if (key < node->left->key) {
                return rightRotate(node);
            }
            else if (key > node->left->key) {
                node->left = leftRotate(node->left);
                return rightRotate(node);
            }
        }
        if (balanceFactor < -1) {
            if (key > node->right->key) {
                return leftRotate(node);
            }
            else if (key < node->right->key) {
                node->right = rightRotate(node->right);
                return leftRotate(node);
            }
        }
        return node;
    }

    // Node with minimum value
    Node* nodeWithMimumValue(Node* node) {
        Node* current = node;
        while (current->left != NULL)
            current = current->left;
        return current;
    }

    Node* find_helper_avl(int key) {
        Node* current_node = this->root;
        while (current_node != nullptr) {
            if (current_node->key == key) {
                break;
            }
            current_node = current_node->key < key ? current_node->right : current_node->left;
        }
        return current_node;
    }

    // Delete a node
    Node* deleteNode(Node* root, int key) {
        // Find the node and delete it
        if (root == NULL)
            return root;
        if (key < root->key)
            root->left = deleteNode(root->left, key);
        else if (key > root->key)
            root->right = deleteNode(root->right, key);
        else {
            if ((root->left == NULL) ||
                (root->right == NULL)) {
                Node* temp = root->left ? root->left : root->right;
                if (temp == NULL) {
                    temp = root;
                    root = NULL;
                }
                else
                    *root = *temp;
                free(temp);
            }
            else {
                Node* temp = nodeWithMimumValue(root->right);
                root->key = temp->key;
                root->right = deleteNode(root->right,
                    temp->key);
            }
        }

        if (root == NULL)
            return root;

        // Update the balance factor of each node and
        // balance the tree
        root->height = 1 + max(height(root->left),
            height(root->right));
        int balanceFactor = getBalanceFactor(root);
        if (balanceFactor > 1) {
            if (getBalanceFactor(root->left) >= 0) {
                return rightRotate(root);
            }
            else {
                root->left = leftRotate(root->left);
                return rightRotate(root);
            }
        }
        if (balanceFactor < -1) {
            if (getBalanceFactor(root->right) <= 0) {
                return leftRotate(root);
            }
            else {
                root->right = rightRotate(root->right);
                return leftRotate(root);
            }
        }
        return root;
    }

    public:
        AVLTree() {
            this->root = NULL;
        }

        void insert_avl(int key) {
            this->root = insertNode(root, key);
        }

        void search_avl(int key) {
            this->root = find_helper_avl(key);
        }

        void remove_avl(int key) {
            this->root = deleteNode(root, key);
        }
};


int main() {
   /* BinarySearchTree bst;

    bst.insert_bst(5);
    bst.insert_bst(2);
    bst.insert_bst(8);
    bst.insert_bst(1);
    bst.insert_bst(4);
    bst.insert_bst(7);
    bst.insert_bst(10);

    chrono::high_resolution_clock::time_point start = chrono::high_resolution_clock::now();
    bst.search_bst(4);
    chrono::high_resolution_clock::time_point end = chrono::high_resolution_clock::now();
    chrono::duration<double, nano> nano_diff = end - start;
    cout << "Time: " << nano_diff.count() << " nano sec." << endl;


    chrono::high_resolution_clock::time_point start1 = chrono::high_resolution_clock::now();
    bst.find_bst(4);
    chrono::high_resolution_clock::time_point end1 = chrono::high_resolution_clock::now();
    chrono::duration<double, nano> nano_diff1 = end1 - start1;
    cout << "Time: " << nano_diff1.count() << " nano sec." << endl;*/

    for (int i = 0; i < 10; i++) {
        cout << "Test seria: " << i + 1 << endl << endl;
        for (int j = 0; j < 20; j++) {
            cout << "test number: " << j + 1 << endl;
            BinarySearchTree bst;
            AVLTree avl;
            auto vec = std::vector<int>(pow(2, 4));
            if (j > 9) {
                mt19937 engine(time(nullptr));
                uniform_real_distribution<double> gen(0, pow(2, 4));
                for (auto& el : vec)
                    el = gen(engine);
            }
            else {
                std::iota(vec.begin(), vec.end(), 1);
            }

            //inserting into bst
            chrono::high_resolution_clock::time_point start1 = chrono::high_resolution_clock::now();
            for (auto& el : vec) {
                bst.insert_bst(el);
            }
            chrono::high_resolution_clock::time_point end1 = chrono::high_resolution_clock::now();
            chrono::duration<double> sec_diff1 = end1 - start1;
            cout << "Time inserting into BST tree: " << sec_diff1.count() << " sec." << endl;

            //inserting into avl
            chrono::high_resolution_clock::time_point start2 = chrono::high_resolution_clock::now();
            for (auto& el : vec) {
                avl.insert_avl(el);
            }
            chrono::high_resolution_clock::time_point end2 = chrono::high_resolution_clock::now();
            chrono::duration<double> sec_diff2 = end2 - start2;
            cout << "Time inserting into AVL tree: " << sec_diff2.count() << " sec." << endl;
            
            //searching in bst
            chrono::high_resolution_clock::time_point start3 = chrono::high_resolution_clock::now();
            for (int k = 0; k < 1000; k++) {
                mt19937 engine(time(nullptr));
                uniform_real_distribution<double> gen(0, pow(2, 4));
                bst.search_bst(gen(engine));
            }
            chrono::high_resolution_clock::time_point end3 = chrono::high_resolution_clock::now();
            chrono::duration<double> sec_diff3 = end3 - start3;
            cout << "Time searching in BST tree: " << sec_diff3.count() << " sec." << endl;

            //searching in avl
            chrono::high_resolution_clock::time_point start4 = chrono::high_resolution_clock::now();
            for (int k = 0; k < 1000; k++) {
                mt19937 engine(time(nullptr));
                uniform_real_distribution<double> gen(0, pow(2, 4));
                avl.search_avl(gen(engine));
            }
            chrono::high_resolution_clock::time_point end4 = chrono::high_resolution_clock::now();
            chrono::duration<double> sec_diff4 = end4 - start4;
            cout << "Time searching in AVL tree: " << sec_diff4.count() << " sec." << endl;

            //searching in vector
            chrono::high_resolution_clock::time_point start5 = chrono::high_resolution_clock::now();
            for (int k = 0; k < 1000; k++) {
                mt19937 engine(time(nullptr));
                uniform_real_distribution<double> gen(0, pow(2, 4));
                std::find(vec.begin(), vec.end(), gen(engine));
            }
            chrono::high_resolution_clock::time_point end5 = chrono::high_resolution_clock::now();
            chrono::duration<double> sec_diff5 = end5 - start5;
            cout << "Time searching in Vector tree: " << sec_diff5.count() << " sec." << endl;

            //deleting in BST
            chrono::high_resolution_clock::time_point start6 = chrono::high_resolution_clock::now();
            for (int k = 0; k < 1000; k++) {
                mt19937 engine(time(nullptr));
                uniform_real_distribution<double> gen(0, pow(2, 4));
                bst.remove_bst(gen(engine));
            }
            chrono::high_resolution_clock::time_point end6 = chrono::high_resolution_clock::now();
            chrono::duration<double> sec_diff6 = end6 - start6;
            cout << "Time deleting from BST tree: " << sec_diff6.count() << " sec." << endl;

            //deleting in AVL
            chrono::high_resolution_clock::time_point start7 = chrono::high_resolution_clock::now();
            for (int k = 0; k < 1000; k++) {
                mt19937 engine(time(nullptr));
                uniform_real_distribution<double> gen(0, pow(2, 4));
                avl.remove_avl(gen(engine));
            }
            chrono::high_resolution_clock::time_point end7 = chrono::high_resolution_clock::now();
            chrono::duration<double> sec_diff7 = end7 - start7;
            cout << "Time deleting from AVL tree: " << sec_diff7.count() << " sec." << endl;
        }
        cout << endl;
    }
  
       
}
