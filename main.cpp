#include <iostream>
#include "tree.cpp"
#include "ctree.cpp"
#include <string>

using namespace std;

int initialize(){

  int nodes;

  cout << "What should the maximum number of nodes per tree be?" << endl;
  cin >> nodes;

  if(nodes <= 0){
    cout << "ERROR: INVALID NUMBER" << endl;
    exit(-1);
  }

  return nodes;
}

int main(){

  int nodes = initialize();

  CTree<int> T(nodes);

  string cmd;
  int data;

  cin.ignore(numeric_limits<streamsize>::max(),'\n');
  cin.clear();

  while(true){
    cout << endl << "What would you like to do (find, insert, delete)? Example: find 5" << endl;

    cin >> cmd;
    cin >> data;

    cin.ignore(numeric_limits<streamsize>::max(),'\n');
    cin.clear();

    cout << endl;

    if(cmd == "find"){
      T.search(data);
    } else if(cmd == "insert"){
      T.insert(data);
    } else if(cmd == "delete"){
      T.destroy(data);
    } else {
      cout << "ERROR: INVALID COMMAND" << endl;
    }

  }

  return 0;
}
