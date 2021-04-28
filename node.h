// Each Node will contain the label and token, and anything else that's relevant
// TODO: Make a Class, using the Node class from P0

#ifndef NODE_H
#define NODE_H

#include <string>
#include "token.h"

using namespace std;

struct Node
{
  string label;
  Token *token;
};

#endif