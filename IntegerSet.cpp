#include "IntegerSet.h"

IntegerSet::IntegerSet()
: root(nullptr) {}

IntegerSet::IntegerSet(const IntegerSet& other)
: root(nullptr) {
  root = copyTree(other.root);
}

IntegerSet& IntegerSet::operator=(const IntegerSet& other) {
  if(this != &other)
  {
    clearTree();
    root = copyTree(other.root);
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
  int thisSize = size();
  int otherSize = other.size();

  if (thisSize != otherSize)
      return false;

  IntegerSetIterator it1(this->root);
  IntegerSetIterator it2(other.root);

  while (it1.hasNext() && it2.hasNext()) 
      if (it1.next() != it2.next()) 
          return false;

  return true;
}

bool IntegerSet::contains(int elem) const {
  return containsNode(root, elem);
}

bool IntegerSet::subsetOf(const IntegerSet& other) const {
  IntegerSetIterator it1(this->root);
  IntegerSetIterator it2(other.root);

  if (!it1.hasNext()) {
      return true; 
  }

  if (!it2.hasNext()) {
      return false; 
  }

  int value1 = it1.next();
  int value2 = it2.next();

  while (true) {
      if (value1 < value2) {
          return false; 
      } else if (value1 == value2) {
          if (!it1.hasNext()) {
              return true; 
          }
          value1 = it1.next();
      }

      if (!it2.hasNext()) {
          return false; 
      }
      value2 = it2.next();
  }
}

IntegerSet IntegerSet::getUnion(const IntegerSet& other) const {
    IntegerSet unionSet;
    IntegerSetIterator it1(this->root);
    IntegerSetIterator it2(other.root);

    while (it1.hasNext() || it2.hasNext()) {
        if (!it2.hasNext() || (it1.hasNext() && it1.peek() < it2.peek())) {
            unionSet.insert(it1.next());
        } else if (!it1.hasNext() || (it2.hasNext() && it2.peek() < it1.peek())) {
            unionSet.insert(it2.next());
        } else {
            unionSet.insert(it1.next());
            it2.next();
        }
    }

    return unionSet;
}

IntegerSet IntegerSet::getIntersection(const IntegerSet& other) const {
    IntegerSet intersectionSet;
    IntegerSetIterator it1(this->root);
    IntegerSetIterator it2(other.root);

    while (it1.hasNext() && it2.hasNext()) {
        int value1 = it1.peek(); 
        int value2 = it2.peek(); 

        if (value1 == value2) {
            intersectionSet.insert(value1); 
            it1.next(); 
            it2.next(); 
        } else if (value1 < value2) {
            it1.next(); 
        } else {
            it2.next(); 
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
    catch(const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        throw;
    }
}

Node* IntegerSet::buildTree(const std::vector<int>& values, int start, int end) {
    if (start > end)
        return nullptr;

    int mid = start + (end - start) / 2;
    Node* node = new Node(values[mid]);

    node->left = buildTree(values, start, mid - 1);
    node->right = buildTree(values, mid + 1, end);

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
        root = insertNode(root, value);
    }
    catch(const std::bad_alloc& e) {
        std::cerr << "Memory allocation failed: " << e.what() << '\n';
        release(root); 
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
