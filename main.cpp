#include <iostream>
#include <cstdlib>
#include <string>
#include <time.h>
#include <cmath>
#include "tree.cpp"
#include "ctree.cpp"

using namespace std;

void constructTree(CTree<int> &T, int right){

  int max = log2(right + 1);

  for(int i=1;i<=max;i++){

    int base = (right+1)/pow(2,i);

    for(int j=1;(j*base)<(right + 1);j+=2){
      T.insert(j*base);
    }

  }
}

int main(int argc, char* argv[]){

  int test = atoi(argv[1]);
  int sizeOfTree = atoi(argv[2]) + 1;
  int partitions = atoi(argv[3]);

  int nodes = sizeOfTree/partitions;

  CTree<int> T(nodes);

  if(test == 1 || test == 3){
    //load T
    int nodes = atoi(argv[2]);
    constructTree(T,nodes);
    T.stop();
  }

  if(test == 2 || test == 3){
    //execute operations
    int operations = atoi(argv[2]);
    int n = operations;
    
    srand(time(NULL));

    while(operations--){
      int r = rand()%3;
      if(r == 0){
	T.insert(rand()%n+1);
      } else if(r == 1){
	T.search(rand()%n+1);
      } else {
	T.destroy(rand()%n+1);
      }
    }
    T.stop();
  }

  return 0;
}
