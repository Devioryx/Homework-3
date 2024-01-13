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

    Node(int value, Node* left = nullptr, Node* right = nullptr) : value(value), left(left), right(right) {}; 
};

// Helpers
int countElements(Node* tree);
Node* getMax(Node *root);
Node* getMin(Node* root);

void release(Node*& node);

Node* copy(Node* root);


void insertNode(Node *&root, int value);
bool containsNode(Node* root, int value);