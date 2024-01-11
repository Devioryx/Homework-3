#pragma once
#include <utility>
#include <cmath>
#include <stdexcept>
#include <iostream>

class Node {
public:
    int value;
    Node* left;
    Node* right;

    int height = 1;
    Node(int value, Node* left = nullptr, Node* right = nullptr, int height = 1) : value(value), left(left), right(right), height(height){}; 
};

// Helpers
int countElements(Node* tree);
int getHeight(Node *root);
int getBalance(Node* root);
Node* getMax(Node *root);
Node* getMin(Node* root);

void release(Node* node);

Node* copyTree(Node* root);

Node *rotateRight(Node *root);
Node *rotateLeft(Node *root);

// Rotations
Node *rebalance(Node *root, int value);

Node* insertNode(Node *root, int value);
Node* deleteNode(Node* root, int value);
bool containsNode(Node* root, int value);