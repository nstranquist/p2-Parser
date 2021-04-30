#include "tree.h"

#include <fstream>
#include <iostream>
#include <string>
#include <queue>

using namespace std;

ParseTree::ParseTree() {
  root = nullptr;
}

Node* ParseTree::createNode(string label) {
  Node *node = new Node();

  // cout << "creating node with label: " << label << endl;
  node->label = label;

  return node;
}

Node* ParseTree::buildTree(string label) {
  string delimiterComma = ",";
  string delimiterSpace = " ";
  string delimiterTab = "\t";
  string line;

  cout << "Building Tree by creating first Node." << endl;

  Node *rootNode;
  rootNode = this->createNode(label);

  // string key = this->getCharKey(line);

  // TODO: replace with this->addTokenToNode();
  // this->handleNewNode(label);
  // this->addTokenToNode(node, );

  // return this->root;
  return rootNode;
}

void ParseTree::addTokenToNode(Node *node, Token *token) {
  node->tokens.push_back(token);
}

// root -> left -> right
void ParseTree::printPreorder(Node *rootPtr, int level) {
  if (rootPtr==NULL) return;

  cout << std::string((level*2), '-') << rootPtr->label << endl; // << ": " << rootPtr->printToken()
  // Iterate over vectors, from left to right, calling printPreorder() on the vector
  // (source) https://cal-linux.com/tutorials//vectors.html
  int i;
  for(vector<Node*>::iterator t = rootPtr->nodes.begin(); t != rootPtr->nodes.end(); ++t) {
    cout << "iteration #" << i << endl;
    printPreorder(rootPtr->nodes[i], level + 1);
    i++;
  }
  // printPreorder(rootPtr->left, level+1);
  // printPreorder(rootPtr->right, level+1);
}

void ParseTree::printToken(Token *token) {
  if(token == NULL) {
    cout << "token is null" << endl;
    return;
  }
  cout << "Printing Token in tree.cpp" << endl;
  cout << "Token 1: Line #: " << token->lineNumber
    << "\tInstance: " << token->tokenInstance
    << "\tID: " << token->tokenID
    << endl;
}

void ParseTree::printNode(Node *node) {
  if(node == NULL) {
    cout << "node is null" << endl;
    return;
  }

  cout << "Node: Label: " << node->label << endl;
  cout<< "Tokens: " << endl;
  
  int i = 1;
  // Print Tokens
  for(vector<Token*>::iterator token_iterator = node->tokens.begin(); token_iterator != node->tokens.end(); ++token_iterator) {
    cout << "Token #" << i << endl;
    cout << "\tline " << (*token_iterator)->lineNumber
      << " \tInstance: " << (*token_iterator)->tokenInstance
      << " \tID: " << (*token_iterator)->tokenID << endl;
    i++;
  }
  // Print Sub-Nodes
  i = 1;
  for(vector<Node*>::iterator node_iterator = node->nodes.begin(); node_iterator != node->nodes.end(); ++node_iterator) {
    cout << "Node #" << i
       << ": " << (*node_iterator)->label << endl;
    i++;
  }
}

Node* ParseTree::insertNode(string label) { // Node *root
  // if root is null, need to initialize it
  Node *newNode;
  newNode = this->createNode(label);
  return newNode;

  // if root exists, can safely search node to insert
  // Node *nodeToInsert = this->createNode(label);

  // queue<Node*> q;
  // q.push(root);

  // while(!q.empty()) {
  //   Node *temp_node = q.front();

  //   q.pop();

  //   // vector<Node*>::iterator node_iterator = temp_node->nodes.begin();
  //   // size_t i = 0;
  //   // size_t maxSize = temp_node->nodes.size();
  //   // Vector Iteration Reference: https://riptutorial.com/cplusplus/example/1679/accessing-elements
    
  //   for (size_t i = 0; i < temp_node->nodes.size(); ++i) {
  //     if(temp_node->nodes[i] == NULL) {
  //       temp_node->nodes[i] = nodeToInsert;
  //       cout << "returning root node in for loop" << endl;
  //       return root; // return from ALL or just this loop?
  //     }
  //     else {
  //       q.push(temp_node->nodes[i]);
  //     }
  //   }
  //   // if(temp_node->right == NULL) {
  //   //   temp_node->right = nodeToInsert;
  //   //   return;
  //   // }
  //   // else {
  //   //   q.push(temp_node->right);
  //   // }
  // }
  // cout << "node insertion completed" << endl;

  // return root;
}

bool ParseTree::areNodesEmpty(Node *node) {
  size_t numNodes = node->nodes.size();
  if(numNodes == 0) {
    return true;
  }
  return false;
}

bool ParseTree::areTokensEmpty(Node *node) {
  size_t numTokens = node->tokens.size();
  if(numTokens == 0) {
    return true;
  }
  return false;
}

// Node* ParseTree::searchNode(string searchKey) {
//   if(this->root == NULL || root->key == searchKey)
//     return this->root;

//   queue<Node*> q;
//   Node *out = nullptr;

//   q.push(this->root);

//   while(!q.empty()) {
//     Node *temp = q.front();
//     q.pop();

//     if(temp->key == searchKey)
//       out = temp;
//     if(temp->left != NULL)
//       q.push(temp->left);
//     if(temp->right != NULL)
//       q.push(temp->right);
//   }
  
//   return out;
// }

// returns the 1 or 2 letter strings from word
// string ParseTree::getCharKey(string word) {
//   // get length of word
//   int length = word.length();

//   if(length > 2) {
//     string charKey = word.substr(0, 2);
//     return charKey;
//   }
//   else
//     return word;
// }

// void ParseTree::splitByDelimiter (string delimiter, string line) {
//   size_t last = 0;
//   size_t next = 0;

//   string word;
//   string key;
//   while((next = line.find(delimiter, last)) != string::npos) {
//     word = line.substr(last, (next-last));
//     key = this->getCharKey(word);

//     this->handleNewNode(key, word);

//     last = next + 1;
//   }

//   if(last > 0) {
//     word = line.substr(last);
//     key = this->getCharKey(word);

//     this->handleNewNode(key, word);
//   }
// }

// void ParseTree::handleNewNode (string key, string line) {
//   Node *foundNode = this->searchNode(key);
    
//   if(foundNode == NULL) {
//     this->insertNode(key, line);
//   }
//   else {
//     // Check if the node already contains the word
//     bool isDupliate = this->checkDuplicateWord(foundNode, line);
//     if(isDupliate) return;
//     this->addTokenToNode(foundNode, line);
//   }
// }

// bool ParseTree::checkDuplicateWord(Node *foundNode, string line) {
//   // iterate through words by space
//   size_t last = 0;
//   size_t next = 0;

//   string word;
//   string key;
//   while((next = foundNode->words.find(" ", last)) != string::npos) {
//     word = line.substr(last, (next-last));

//     // check if equal
//     if(word == line)
//       return true;

//     last = next + 1;
//   }

//   if(last > 0) {
//     word = line.substr(last);

//     if(word == line)
//       return true;
//   }

//   return false;
// }