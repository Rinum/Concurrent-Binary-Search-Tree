#include "ctree.h"

using namespace std;

template <class Object>
void CTree<Object>::search(Object data){
  pRoot->Q.push(new Command<Object>('s',new Node<Object>(data)));
}


template <class Object>
void CTree<Object>::insert(Object data){
  pRoot->Q.push(new Command<Object>('i',new Node<Object>(data)));
}

template <class Object>
void CTree<Object>::destroy(Object data){
  pRoot->Q.push(new Command<Object>('d',new Node<Object>(data)));
}

template <class Object>
CTree<Object>::CTree(unsigned nodes){
  if(!nodes)
    return;

  threads = 0;

  //create balanced CTree of Trees
  pRoot = new Tree<Object>;//root of CTree
  pRoot->thread = threads++;
  pRoot->threads = &threads;
  pRoot->maxNodes = nodes;
  nodes--;

  //initialize boost thread & put to sleep
  new boost::thread( boost::bind(&Tree<Object>::run, &*pRoot) );
}
