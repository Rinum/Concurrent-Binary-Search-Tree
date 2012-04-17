#include "tree.h"
using namespace std;

template <class Object>
Node<Object>* Tree<Object>::search(Object data){
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
void Tree<Object>::insert(Node<Object>* pNew){
  if(!max){
    max = pNew;
    min = pNew;
  } else if(pNew->data > max->data){
    max = pNew;
  } else if(pNew->data < min->data){
    min = pNew;
  }

  Object data = pNew->data;
  if(pRoot==NULL){
    pRoot = pNew;
    pNew->parent = NULL;
  } else {
    Node<Object>* pCur = pRoot;
    Node<Object>* pParent;
    while(true){
      pParent = pCur;
      if(data < pCur->data){
	pCur = pCur->pL;
	if(pCur == NULL) {
	  pParent->pL = pNew;
	  pNew->parent = pParent;
	  return;
	}
      } else{
	pCur = pCur->pR;
	if(pCur == NULL){
	  pParent->pR = pNew;
	  pNew->parent = pParent;
	  return;
	}
      }
    }
  }
}

template <class Object>
int Tree<Object>::destroy(Object data){
  Node<Object>* node = search(data);

  if(node != NULL){
    //update max & min if necessary
    if(max->data == node->data)
      max = node->parent;
    else if(min->data == node->data)
      min = node->parent;

    if(node->pL != NULL && node->pR != NULL){//has 2 children
      //replace with largest element in left subtree
      Node<Object>* m = findMax(node);
      node->data = m->data;
      m->parent->pR = NULL;
      delete m;
    } else if(node->pL != NULL){
      node->data = node->pL->data;
      delete node->pL;
      node->pL = NULL;
    } else if(node->pR != NULL){
      node->data = node->pR->data;
      delete node->pR;
      node->pR = NULL;
    } else {
      if(node->parent == NULL){
	delete node;
      } else if(node->parent->pL->data == node->data){
	node->parent->pL = NULL;
	delete node;
      } else {
	node->parent->pR = NULL;
	delete node;
      }
    }
  } else {
    return 0;
  }

  nodes--;

  return 1;

}

template <class Object>
Node<Object>* Tree<Object>::findMax(Node<Object>* node){
  if(node != NULL)
    while(node->pR != NULL)
      node = node->pR;

  return node;
}

template <class Object>
int Tree<Object>::run(){
  while(!Q.size()){
    
  }

  Command<Object>* command = Q.front();
  Q.pop();

  switch(command->cmd){
  case 's': //search
    cout << "\t\t\t" << "Thread " << thread << ": Finding " << command->node->data << endl;
    if(max && command->node->data > max->data && pR != NULL){
      //pass to right
      cout << "\t\t\t" <<  "Thread " << thread << ": Passing Finding " << command->node->data << " to Thread " << pR->thread << endl;
      pR->Q.push(command);
    } else if(min && command->node->data < min->data && pL != NULL){
      //pass to left
      cout << "\t\t\t" <<  "Thread " << thread << ": Passing Finding " << command->node->data << " to Thread " << pL->thread << endl;
      pL->Q.push(command);
    } else {
      //node is in this tree
      Node<Object>* node = search(command->node->data);
      if(!node){
	cout << "\t\t\t" << "Thread " << thread << ": " << command->node->data << " not found" << endl;
      } else {
	cout << "\t\t\t" << "Thread " << thread << ": Found " << node->data << endl;
      }
    }
    run();
    break;
  case 'i': //insert
    cout << "\t\t\t" <<  "Thread " << thread << ": Inserting " << command->node->data << endl;
    if(max && command->node->data > max->data && pR != NULL){
      //pass to right
      cout << "\t\t\t" <<  "Thread " << thread << ": Passing Inserting " << command->node->data << " to Thread " << pR->thread << endl;
      pR->Q.push(command);
    } else if(min && command->node->data < min->data && pL != NULL){
      //pass to left
      cout << "\t\t\t" <<  "Thread " << thread << ": Passing Inserting " << command->node->data << " to Thread " << pL->thread << endl;
      pL->Q.push(command);
    } else {
      //node is in this tree
      insert(command->node);
      nodes++;

      cout << "\t\t\t" <<  "Thread " << thread << ": Inserted " << command->node->data << endl;

      //do we have too many nodes?
      if(nodes > maxNodes){
	cout << "\t\t\t" <<  "Thread " << thread << " is FULL. Moving biggest element (" << max->data << ") to right child." << endl;
	//is right child available?
	if(pR != NULL){
	  nodes--;
	  pR->Q.push(new Command<Object>('i',new Node<Object>(max->data)));
	  
	  if(max->parent){
	    Node<Object>* parent = max->parent;
	    parent->pR = NULL;
	    max = parent;
	  } else {
	    //max is also the root node
	    Node<Object>* newRoot = findMax(pRoot->pL);
	    newRoot->parent->pR = newRoot->pL;
	    newRoot->pL = pRoot->pL;
	    pRoot = newRoot;
	    delete max;
	    max = newRoot;
	  }

	} else {
	  //create new tree
	  pR = new Tree<Object>;
	  pR->thread = (*threads)++;
	  pR->threads = threads;
	  pR->pParent = this;
	  pR->maxNodes = maxNodes;
	  nodes--;
	  pR->Q.push(new Command<Object>('i',new Node<Object>(max->data)));

	  if(max->parent){
	    Node<Object>* parent = max->parent;
	    parent->pR = NULL;
	    max = parent;
	  } else {
	    //max is also the root node
	    Node<Object>* newRoot = findMax(pRoot->pL);
	    newRoot->parent->pR = newRoot->pL;
	    newRoot->pL = pRoot->pL;
	    pRoot = newRoot;
	    delete max;
	    max = newRoot;
	  }

	  new boost::thread( boost::bind(&Tree<Object>::run, &*pR) );
	}
      }
    }
    run();
    break;
  case 'd': //delete
    cout << "\t\t\t" << "Thread " << thread << ": Deleting " << command->node->data << endl;
    if(max && command->node->data > max->data && pR != NULL){
      //pass to right
      cout << "\t\t\t" << "Thread " << thread << ": Passing Deleting " << command->node->data << " to Thread " << pR->thread << endl;
      pR->Q.push(command);
    } else if(min && command->node->data < min->data && pL != NULL){
      //pass to left
      cout << "\t\t\t" <<  "Thread " << thread << ": Passing Deleting " << command->node->data << " to Thread " << pL->thread << endl;
      pL->Q.push(command);
    } else {
      //node is in this tree
      if(destroy(command->node->data)){
	cout << "\t\t\t" <<  "Thread " << thread << ": Deleted " << command->node->data << endl;
      } else {
	cout << "\t\t\t" <<  "Thread " << thread << ": Could Not Find " << command->node->data << " For Deletion" << endl;	
      }

      if(nodes == 0)
	pRoot = NULL;
    }
    run();
    break;
  default:
    break;
  }

  return 0;
}
