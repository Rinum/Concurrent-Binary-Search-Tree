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
void CTree<Object>::stop(){
  pRoot->Q.push(new Command<Object>('t',NULL));
}

template <class Object>
CTree<Object>::CTree(unsigned nodes){
  if(!nodes)
    return;

  //create balanced CTree of Trees
  pRoot = new Tree<Object>;//root of CTree
  pRoot->maxNodes = nodes;
  pRoot->threads = &threads;
  nodes--;

  //initialize boost thread
  threads.push_back(new boost::thread( boost::bind(&Tree<Object>::run, &*pRoot) ));
}
