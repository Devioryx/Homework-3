#include "IntegerSet.h"

IntegerSet::IntegerSet()
: root(nullptr) {}



IntegerSet::IntegerSet(const IntegerSet& other)
: root(nullptr) {
  root = copy(other.root);
}

IntegerSet& IntegerSet::operator=(const IntegerSet& other) {
  if(this != &other)
  {
    clearTree();
    root = copy(other.root);
  }
  return *this;
}

IntegerSet::~IntegerSet() {
  clearTree();
}

size_t IntegerSet::size() const {
  return countElements(root);
}

bool IntegerSet::equals(const IntegerSet& other) const {
    int thisSize = size(); // Assuming 'size()' method calculates the number of elements
    int otherSize = other.size();

    if (thisSize != otherSize) {
        return false; // Sets of different sizes are not equal
    }

    NodeIterator it1(this->root);
    NodeIterator it2(other.root);
    NodeIterator end = NodeIterator::endIterator();

    while (!it1.atEnd() && !it2.atEnd()) {
        if (it1->value != it2->value) {
            return false; // Mismatch found
        }
        ++it1;
        ++it2;
    }

    return true; // All elements matched
}

bool IntegerSet::contains(int elem) const {
  return containsNode(root, elem);
}

bool IntegerSet::subsetOf(const IntegerSet& other) const {
    NodeIterator it1(this->root);
    NodeIterator it2(other.root);
    NodeIterator end = NodeIterator::endIterator();

    if (it1.atEnd()) {
        return true; // Empty set is a subset of any set
    }

    if (it2.atEnd()) {
        return false; // Non-empty set cannot be a subset of an empty set
    }

    int value1 = it1->value;
    int value2 = it2->value;

    while (true) {
        if (value1 < value2) {
            return false; // Element in 'this' not found in 'other'
        } else if (value1 == value2) {
            ++it1; // Move to next element in 'this'
            if (it1.atEnd()) {
                return true; // All elements of 'this' found in 'other'
            }
            value1 = it1->value;
        }

        ++it2; // Move to next element in 'other'
        if (it2.atEnd()) {
            return false; // Reached end of 'other' without finding all elements of 'this'
        }
        value2 = it2->value;
    }
}

IntegerSet IntegerSet::getUnion(const IntegerSet& other) const {
    IntegerSet unionSet;
    Node* rightmostNode = nullptr;
    NodeIterator it1(this->root);
    NodeIterator it2(other.root);
    NodeIterator end = NodeIterator::endIterator();

    while (!it1.atEnd() || !it2.atEnd()) {
        Node* newNode = nullptr;
        try {
            if (it2.atEnd() || (!it1.atEnd() && it1->value < it2->value)) {
                newNode = new Node((*it1).value);
                ++it1;
            } else if (it1.atEnd() || (!it2.atEnd() && it2->value < it1->value)) {
                newNode = new Node((*it2).value);
                ++it2;
            } else {
                newNode = new Node((*it1).value);
                ++it1; ++it2;
            }
        }
        catch(std::bad_alloc& e) {
            unionSet.clearTree();
            throw;
        }

        if (unionSet.root == nullptr) {
            unionSet.root = newNode;
            rightmostNode = newNode;
        } else {
            rightmostNode->right = newNode;
            rightmostNode = newNode;
        }
    }

    return unionSet;
}

IntegerSet IntegerSet::getIntersection(const IntegerSet& other) const {
    IntegerSet intersectionSet;
    Node* rightmostNode = nullptr;
    NodeIterator it1(this->root);
    NodeIterator it2(other.root);
    NodeIterator end = NodeIterator::endIterator();

    while (!it1.atEnd() && !it2.atEnd()) {
        Node* newNode = nullptr;

        int value1 = it1->value; 
        int value2 = it2->value;

        try {
            if (value1 == value2) {
                newNode = new Node(value1);
                if (intersectionSet.root == nullptr) {
                    intersectionSet.root = newNode;
                    rightmostNode = newNode;
                } else {
                    rightmostNode->right = newNode;
                    rightmostNode = newNode;
                }
                ++it1; 
                ++it2; 
            } else if (value1 < value2) {
                ++it1;
            } else {
                ++it2;
            }
        }
        catch(std::exception& e) {
            intersectionSet.clearTree();
            throw;
        }
    }

    return intersectionSet;
}

void IntegerSet::deserialize(std::istream& in) {
    int itemsSize, value, prevValue = INT_MIN;
    in >> itemsSize;

    std::vector<int> values(itemsSize);

    for(int i = 0; i < itemsSize; i++) {
        if(!(in >> value))
            throw std::runtime_error("Invalid input format: Expected integer value");

        if(value <= prevValue)
            throw std::runtime_error("Invalid input: Numbers are not in strictly increasing order or there are duplicates");

        prevValue = value;
        values[i] = value;
    }

    try {
        root = buildTree(values, 0, itemsSize - 1);
    }
    catch(std::bad_alloc& e) {
        clearTree();
        throw;
    }
}

Node* IntegerSet::buildTree(const std::vector<int>& values, int start, int end) {
    if (start > end)
        return nullptr;

    Node* node = nullptr;
    try {
        int mid = start + (end - start) / 2;
        node = new Node(values[mid]);

        node->left = buildTree(values, start, mid - 1);
        node->right = buildTree(values, mid + 1, end);
    }
    catch (std::bad_alloc& e) {
        release(node);
        throw;
    }

    return node;
}

void IntegerSet::serializeHelper(Node* node, std::ostream& out, bool& isFirst) const {
    if (node == nullptr) {
        return;
    }

    serializeHelper(node->left, out, isFirst);

    if (!isFirst)
        out << " ";
    else 
        isFirst = false;

    out << node->value;       
    serializeHelper(node->right, out, isFirst);
}

void IntegerSet::insert(int value) {
    try {
        insertNode(root, value);
    }
    catch(std::bad_alloc& ) {
        throw; 
    }
}

void IntegerSet::serialize(std::ostream& out) const
{
    int numberOfItems = size();
    out << numberOfItems;

    if(numberOfItems)
      out << " ";
      
    bool isFirst = true;
    serializeHelper(root, out, isFirst);
}

void IntegerSet::clearTree() {
  release(root);
  root = nullptr;
}