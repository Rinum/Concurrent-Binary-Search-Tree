#include <cstddef>
#include <unistd.h>
#include <queue>
#include <iostream>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>

#ifndef TREE_H
#define TREE_H

using namespace std;

template <class Object>
struct Node {
  Node(Object d): data(d), pL(NULL), pR(NULL){}
  ~Node(){}

  Object data;
  Node<Object>* pL;
  Node<Object>* pR;
  Node<Object>* parent;
};

template <class Object>
struct Command {
  Command(char c, Node<Object>* n): cmd(c), node(n){}
  char cmd;
  Node<Object>* node;
};

template <class Object>
class Tree {
public:
  Tree() : pParent(NULL),pL(NULL),pR(NULL),threads(NULL),pRoot(NULL),max(NULL),min(NULL),thread(0),nodes(0){}
  Node<Object>* search(Object data);
  void insert(Node<Object>* pNew);
  int destroy(Object data);
  int run();
  Node<Object>* findMax(Node<Object>* node);

  Tree<Object>* pParent;
  Tree<Object>* pL;
  Tree<Object>* pR;

  unsigned* threads;

  Node<Object>* pRoot;
  Node<Object>* max;
  Node<Object>* min;

  queue<Command<Object>*> Q;

  unsigned thread;
  unsigned nodes;
  unsigned maxNodes;
};

#endif
