#ifndef PARSER_H
#define PARSER_H

#include <stdio.h>
#include <istream>
#include "token.h"

using namespace std;

class Parser
{
public:
  void parser(istream *input); // TODO: change to the tree root Node

private:
  istream *input; // for getting the next token from scanner
  Token *token; // the current token lookahead

  // Define nonterminal functions (as templates for now)
  void program();
  void block();
  void vars();
  void expr();

  void N();
  void A();
  void M();
  void R();
  void stats();
  void mStat();
  void stat();

  void in();
  void out();
  void _if();
  void loop();
  void assign();
  void RO();
  void label();
  void _goto();

  void printToken(Token *token);
  Token* getTokenFromScanner();
  bool isEofToken(Token *token);
  void throwError(string message);
};

#endif