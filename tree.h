#include <cstddef>
#include <unistd.h>
#include <queue>
#include <iostream>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <math.h>
#include <vector>

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
};

template <class Object>
struct Command {
  Command(char c, Node<Object>* n): cmd(c), node(n){}
  char cmd;
  Node<Object>* node;
};

template <class Object>
class concurrent_queue {
private:
  queue<Object> the_queue;
  mutable boost::mutex the_mutex;
  boost::condition_variable the_condition_variable;
public:
  void push(const Object& object){
    boost::mutex::scoped_lock lock(the_mutex);
    the_queue.push(object);
    lock.unlock();
    the_condition_variable.notify_one();
  }

  int wait_and_pop(Object& popped_value){
    boost::mutex::scoped_lock lock(the_mutex);
    while(the_queue.empty())
      the_condition_variable.wait(lock);

    popped_value=the_queue.front();
    the_queue.pop();

    return 1;
  }
};

template <class Object>
class Tree {
public:
  Tree() : pL(NULL),pR(NULL),pRoot(NULL),max(NULL),min(NULL),nodes(0),threads(NULL){}

  //normal BST functions
  Node<Object>* searchBST(Object data);
  int insertBST(Node<Object>* pNew);
  int destroyBST(Object data, Node<Object>*& root);

  //wrapper for BST functions to manage overarching tree
  Node<Object>* search(Command<Object>* command);
  void insert(Command<Object>* command);
  void destroy(Command<Object>* command);

  //BST helper functions
  int numOfNodes(Node<Object>* node);
  Node<Object>* findMax(Node<Object>* node);
  Node<Object>* findMin(Node<Object>* node);

  //subtree (partition) manager
  int run();

  Tree<Object>* pL;
  Tree<Object>* pR;

  Node<Object>* pRoot;
  Node<Object>* max;
  Node<Object>* min;

  concurrent_queue<Command<Object>*> Q;

  unsigned nodes;
  unsigned maxNodes;

  vector<boost::thread *> *threads;
};

#endif
