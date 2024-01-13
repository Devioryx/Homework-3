#pragma once

#include <iostream>
#include "Node.h"
#include <stack>
#include <climits>
#include <vector>
#include <cassert>

class IntegerSet {
private:
  Node* root;

public:
  IntegerSet();
  IntegerSet(const IntegerSet&);
  IntegerSet& operator=(const IntegerSet&);
  ~IntegerSet();

class NodeIterator {

private:
    std::stack<Node*> backtrack;

private:
    void pushAllTheWayToTheLeft(Node* startFrom)
    {
        for(; startFrom; startFrom = startFrom->left)
            backtrack.push(startFrom);
    }

public:
    static NodeIterator endIterator()
    {
        return NodeIterator(nullptr);
    }

public:
    NodeIterator(Node* startFrom)
    {
        pushAllTheWayToTheLeft(startFrom);
    }
    
    bool atEnd() const
    {
        return backtrack.empty();
    }

    Node& operator*()
    {
        assert( ! atEnd() );
        return *backtrack.top();
    }

    Node* operator->()
    {
        assert( ! atEnd() );
        return backtrack.top();
    }
    
    void operator++()
    {
        assert( ! atEnd() );
        Node* p = backtrack.top();
        backtrack.pop();
        pushAllTheWayToTheLeft(p->right);
    }

    bool operator==(const NodeIterator& other) const
    {
        if(atEnd() || other.atEnd())
            return atEnd() == other.atEnd();

        return backtrack.top() == other.backtrack.top();
    }

    bool operator!=(const NodeIterator& other) const
    {
        return ! operator==(other);
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