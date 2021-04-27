#include <iostream>

using namespace std;

// - Each node will print one line or possibly nothing (if the leave has nothing in it).
// - The line will start with indentations of 2 spaces per level, then display the node's label and any tokes stored in the node
// - Continue over children left to right

void printTree()
{
  cout << "2. Printing Tree...\n\n";

  // Print with Preorder traversal, with indentations, for testing purposes
  // printPreorder(node, 0);
}

// void printPreorder(Node *rootP, int level) {
//   if (rootP==NULL) return;
//   // printf("%*c%d:%-9s ",level*2,' ',level/*, NodeId.info*/); // assume some info printed as string
//   printf("\n");
//   // cout << rootP->data << " ";
//   printPreorder(rootP->left,level+1);
//   printPreorder(rootP->right,level+1);
// }