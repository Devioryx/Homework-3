#include "Node.h"

int countElements(Node* tree) {
    if(!tree) return 0;
    return 1 + countElements(tree->left) + countElements(tree->right);
}
// Helpers
int getHeight(Node *root) {
    if(!root) return 0;
    return root->height;
}

int getBalance(Node* root) {
    if(!root) return 0;
    return getHeight(root->right) - getHeight(root->left);
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

void release(Node* node){
    if(node == nullptr) return;
    release(node->left);
    release(node->right);
    delete node;
    node = nullptr;
}

Node* copyTree(Node* root) {
    if (!root) {
        return nullptr;
    }
    Node* newNode = nullptr;
    try {
        newNode = new Node(root->value);
        newNode->left = copyTree(root->left);
        newNode->right = copyTree(root->right);
        newNode->height = 1 + std::max(getHeight(newNode->left), getHeight(newNode->right));
    }
    catch(const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        release(newNode);
        throw;
    }
    
    return newNode;
}

Node *rotateRight(Node *root) {
    Node* tempL = root->left;
    Node* tempR = tempL->right;

    tempL->right = root;
    root->left = tempR;

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    tempL->height = 1 + std::max(getHeight(tempL->left), getHeight(tempL->right));

    return tempL;
}
Node *rotateLeft(Node *root) {
    Node* tempR = root->right;
    Node* tempL = tempR->left;

    tempR->left = root;
    root->right = tempL;

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    tempR->height = 1 + std::max(getHeight(tempR->left), getHeight(tempR->right));

    return tempR;
}

// Rotations
Node *rebalance(Node *root, int value) {
    // Get the current balance
    int balance = getBalance(root);

    // Check if any rotations are needed:
    // LL case
    if(balance < -1 && value < root->left->value) return rotateRight(root);

    // RR case
    if(balance > 1 && value > root->right->value) return rotateLeft(root);

    // LR case
    if(balance < -1 && value > root->left->value) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // RL case
    if(balance > 1 && value < root->right->value) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;

}

Node* insertNode(Node *root, int value) {
    try {
        if(!root)
        return new Node(value);

        if(value < root->value)
            root->left = insertNode(root->left, value);

        else if (value > root->value)
            root->right = insertNode(root->right, value);

        root = rebalance(root, value);
        root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));
    }
    catch(const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        throw;
    }
    
    return root;
}

Node* deleteNode(Node* root, int value) {
    if (root == nullptr)
        return root;
    if (value < root->value)
        root->left = deleteNode(root->left, value);

    else if (value > root->value)
        root->right = deleteNode(root->right, value);
        
    else {
        if ((root->left == nullptr) || (root->right == nullptr)) {
            Node* temp = root->left ? root->left : root->right;
            if (temp == nullptr) {
                temp = root;
                root = nullptr;
            } else
                *root = *temp;
            delete temp;
        } else {
            Node* temp = getMin(root->right);
            root->value = temp->value;
            root->right = deleteNode(root->right, temp->value);
        }
    }
    if (root == nullptr)
        return root;

    root->height = 1 + std::max(getHeight(root->left), getHeight(root->right));

    int balance = getBalance(root);

    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
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


