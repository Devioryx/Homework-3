#pragma once

#include <iostream>
#include "Node.h"
#include <stack>
#include <climits>
#include <vector>

class IntegerSet {
private:
  Node* root;

public:
  IntegerSet();
  IntegerSet(const IntegerSet&);
  IntegerSet& operator=(const IntegerSet&);
  ~IntegerSet();

  class IntegerSetIterator {
  private:
    std::stack<Node*> stack;
    Node* current;

    void pushLeft(Node* node) {
        while (node != nullptr) {
            stack.push(node);
            node = node->left;
        }
    } 

public:
    IntegerSetIterator(Node* root) : current(root) {
        pushLeft(current);
    }

    bool hasNext() {
        return !stack.empty();
    }

    int next() {
        current = stack.top();
        stack.pop();
        int value = current->value;
        pushLeft(current->right);
        return value;
    };

    int peek() {
      if (hasNext()) {
          return stack.top()->value;
      }
      throw std::runtime_error("No more elements");
    }
};
public:
  // Връща броя на елементите в множеството
  size_t size() const;

  /// Проверява дали две множества се състоят от едни и същи елементи
  bool equals(const IntegerSet&) const;

  /// Проверява дали елемент се съдържа в множеството
  bool contains(int) const;

  /// Проверява дали текущия обект е подмножество на друг
  bool subsetOf(const IntegerSet&) const;

  /// Връща обединението на две множества
  IntegerSet getUnion(const IntegerSet&) const;

  /// Връща сечението на две множества
  IntegerSet getIntersection(const IntegerSet&) const;

  /// Десериализира съдържанието на едно множество.
  /// Новата информация напълно заменя старата.
  void deserialize(std::istream&);

  /// Сериализира съдържанието на едно множество
  void serialize(std::ostream&) const;


  private:
    void clearTree();
    void serializeHelper(Node* node, std::ostream& out, bool& isFirst) const;
    void insert(int value);
    Node* buildTree(const std::vector<int>& values, int start, int end);
};

inline std::istream& operator>>(std::istream& in, IntegerSet& set)
{
  set.deserialize(in);
  return in;
}

inline std::ostream& operator<<(std::ostream& out, const IntegerSet& set)
{
  set.serialize(out);
  return out;
}
