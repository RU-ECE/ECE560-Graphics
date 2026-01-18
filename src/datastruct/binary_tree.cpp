class binary_tree {
    struct node {
        int value;
        node* left;
        node* right;
    };
    node* root;
    binary_tree() {
        root = nullptr;
    }
    ~binary_tree() {
        delete root;
    }
};

int main() {
    binary_tree tree;
    for (int i = 0; i < 1'000'000; i++) {
        tree.insert(i);
    }
}
