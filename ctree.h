#include <cstddef>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include "tree.h"

#ifndef CTREE_H
#define CTREE_H

using namespace std;

template <class Object>
class CTree {
public:
  CTree(unsigned nodes);
  void search(Object data);
  void insert(Object data);
  void destroy(Object data);

  Tree<Object>* pRoot;

  unsigned threads;
};

#endif
