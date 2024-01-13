#include "Node.h"

int countElements(Node* tree) {
    if(!tree) return 0;
    return 1 + countElements(tree->left) + countElements(tree->right);
}

Node* getMax(Node *root) {
    if(!root) return nullptr;
    if(!root->right) return root;

    return getMax(root->right);
}

Node* getMin(Node* root) {
    if(!root) return nullptr;
    if(!root->left) return root;

    return getMin(root->left);
}

void release(Node*& node){
    if(node == nullptr) return;
    release(node->left);
    release(node->right);
    delete node;
    node = nullptr;
}

Node* copy(Node* root) {
    
    if (!root) {
        return nullptr;
    }

    Node* newNode = nullptr;
    Node* leftTree  = nullptr;
    Node* rightTree = nullptr;

    try {
        newNode = new Node(root->value);
        leftTree = copy(root->left);
        rightTree = copy(root->right);
    }
    catch(std::bad_alloc& e) {
        release(newNode);
        release(leftTree);
        release(rightTree);
        throw;
    }

    newNode->left = leftTree;
    newNode->right = rightTree;
    
    return newNode;
}


void insertNode(Node*& root, int value) {
    try {
        if (!root) {
            root = new Node(value);
        } else if (value < root->value) {
            insertNode(root->left, value);
        } else if (value > root->value) {
            insertNode(root->right, value);
        }
    }
    catch (const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        throw;
    }
}

bool containsNode(Node* root, int value) {
    if (root == nullptr) {
        return false; 
    }

    if (value == root->value) {
        return true; 
    } else if (value < root->value) {
        return containsNode(root->left, value);
    } else {
        return containsNode(root->right, value); 
    }
}

