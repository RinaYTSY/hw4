#ifndef RECCHECK
#include <iostream>

//if you want to add any #includes like <iostream> you must do them here (before the next endif)

#endif

#include "equal-paths.h"
using namespace std;


// You may add any prototypes of helper functions here
int getHeight(Node* root);

bool equalPaths(Node * root)
{
    // Add your code below
    if (root==nullptr){//base case: empty subtree
      return true;
    }
    if (root->left == nullptr){
      return equalPaths(root->right);
    }
    else if (root->right == nullptr){
      return equalPaths(root->left);
    }
    else{
      bool left=equalPaths(root->left);
      bool right=equalPaths(root->right);

      if(left && right){
        int leftH=getHeight(root->left);
        int rightH=getHeight(root->right);
        return(leftH==rightH);
      }
    }
    return false;
    

}
int getHeight(Node* root){
  if(root==nullptr){
    return 0;
  }
  return 1+ max(getHeight(root->left), getHeight(root->right));
}


