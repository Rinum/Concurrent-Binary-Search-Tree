#include "tree.h"
using namespace std;

template <class Object>
Node<Object>* Tree<Object>::searchBST(Object data){
  if(!pRoot)
    return NULL;

  Node<Object>* pCur = pRoot;

  while(pCur->data != data){
    if(data < pCur->data)
      pCur = pCur->pL;
    else
      pCur = pCur->pR;
    if(pCur == NULL)
      return NULL;
  }
  return pCur;
}

template <class Object>
int Tree<Object>::insertBST(Node<Object>* pNew){

  Object data = pNew->data;
  if(pRoot==NULL){
    pRoot = pNew;
    return 1;
  } else {
    Node<Object>* pCur = pRoot;
    Node<Object>* pParent;
    while(true){
      pParent = pCur;
      if(data < pCur->data){
	pCur = pCur->pL;
	if(pCur == NULL) {
	  pParent->pL = pNew;
	  return 1;
	}
      } else{
	pCur = pCur->pR;
	if(pCur == NULL){
	  pParent->pR = pNew;
	  return 1;
	}
      }
    }
  }

  return 0;
}

template <class Object>
int Tree<Object>::destroyBST(Object data, Node<Object>*& root){
  if(root == NULL)
    return 0;//data not found
  if(data < root->data)
    return destroyBST(data, root->pL);
  else if(data > root->data)
    return destroyBST(data, root->pR);
  else if(root->pL != NULL && root->pR != NULL){//two children
    root->data = findMin(root->pR)->data;
    return destroyBST(root->data, root->pR);
  } else {
    Node<Object>* oldNode = root;
    root = (root->pL != NULL) ? root->pL : root->pR;
    delete oldNode;
    return 1;
  }

  return 0;
}

template <class Object>
Node<Object>* Tree<Object>::search(Command<Object>* command){

  Object data = command->node->data;

  if(max && data > max->data && pR != NULL){
    //pass to right
    pR->Q.push(command);
  } else if(min && data < min->data && pL != NULL){
    //pass to left
    pL->Q.push(command);
  } else {
    //node is in this tree
    Node<Object>* node = searchBST(data);
    return node;
  }
  
  return NULL;
}

template <class Object>
void Tree<Object>::insert(Command<Object>* command){
  Object data = command->node->data;

  if(max && data > max->data && pR != NULL){
    //pass to right
    pR->Q.push(command);
  } else if(min && data < min->data && pL != NULL){
    //pass to left
    pL->Q.push(command);
  } else {
    //node is in this tree
    if(insertBST(new Node<Object>(data))){

      if(!max){//only 1 node in tree
	max = pRoot;
	min = pRoot;
      }

      if(max->pR) //new max?
	max = max->pR;
      if(min->pL) //new min?
	min = min->pL;

      nodes++;
    }
    
    //do we have too many nodes?
    if(nodes == maxNodes){
      //create new left & right trees

      if(pRoot->pR){
	Tree<Object>* TR = new Tree<Object>;
	TR->pR = pR;
	pR = TR;
	pR->maxNodes = maxNodes;
	pR->pRoot = pRoot->pR->pR;
	pR->nodes = numOfNodes(pRoot->pR->pR);
	pR->max = max;
	pR->min = findMin(pRoot->pR->pR);
	pR->Q.push(new Command<Object>('i',new Node<Object>(pRoot->pR->data)));
	Node<Object>* oldNode = pRoot->pR;
	pRoot->pR = oldNode->pL;
	delete oldNode;
	pR->threads = threads;
	//initialize new tree partition
	threads->push_back(new boost::thread( boost::bind(&Tree<Object>::run, &*pR) ));
      }

      if(pRoot->pL){
	Tree<Object>* TL = new Tree<Object>;
	TL->pL = pL;
	pL = TL;
	pL->maxNodes = maxNodes;
	pL->pRoot = pRoot->pL->pL;
	pL->nodes = numOfNodes(pRoot->pL->pL);
	pL->max = max;
	pL->min = findMin(pRoot->pL->pL);
	pL->Q.push(new Command<Object>('i',new Node<Object>(pRoot->pL->data)));
	Node<Object>* oldNode = pRoot->pL;
	pRoot->pL = oldNode->pR;
	delete oldNode;
	pL->threads = threads;
	//initialize new tree partition
	threads->push_back(new boost::thread( boost::bind(&Tree<Object>::run, &*pL) ));
      }

      nodes = numOfNodes(pRoot);
      max = findMax(pRoot);
      min = findMin(pRoot);

    }
  }
}

template <class Object>
void Tree<Object>::destroy(Command<Object>* command){

  Object data = command->node->data;

  if(max && data > max->data && pR != NULL){
    //pass to right
    pR->Q.push(command);
  } else if(min && data < min->data && pL != NULL){
    //pass to left
    pL->Q.push(command);
  } else {
    //node is in this tree

    if(destroyBST(data,pRoot)){
      nodes--;

      //the only time max or min is not set is if it was just deleted
      if(!max)
	max = findMax(pRoot);
      if(!min)
	min = findMin(pRoot);
    }    
  }
}

template <class Object>
int Tree<Object>::numOfNodes(Node<Object>* node){
  if(!node)
    return 0;

  return 1 + numOfNodes(node->pL) + numOfNodes(node->pR);
}

template <class Object>
Node<Object>* Tree<Object>::findMax(Node<Object>* node){
  if(node != NULL)
    while(node->pR != NULL)
      node = node->pR;

  return node;
}

template <class Object>
Node<Object>* Tree<Object>::findMin(Node<Object>* node){
  if(node != NULL)
    while(node->pL != NULL)
      node = node->pL;

  return node;
}

template <class Object>
int Tree<Object>::run(){
  Command<Object>* command;

  while(Q.wait_and_pop(command)){

    if(command->cmd == 's')
      search(command);
    else if(command->cmd == 'i')
      insert(command);
    else if(command->cmd == 'd')
      destroy(command);
    else if(command->cmd == 't'){
      if(pR){
	pR->Q.push(command);
      }
      if(pL){
	pL->Q.push(command);
      }
      
      return 1;
    }

  }

  return 0;
}
