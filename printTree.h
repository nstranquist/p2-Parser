#ifndef PRINTTREE_H
#define PRINTTREE_H

#include "node.h"

class PrintTree {
  public:
    void printTree(Node *root);
    void printPreorder(Node *node, int level);
};

#endif