#include <stdio.h>
#include <istream>
#include "token.h"

using namespace std;

class Parser
{
public:
  void parser(istream *input); // TODO: change to the tree root Node

private:
  // Define nonterminal functions (as templates for now)
  void program(Token *token);
  void block(Token *token);
  void vars(Token *token);
  void expr(Token *token);

  void N(Token *token);
  void A(Token *token);
  void M(Token *token);
  void R(Token *token);
  void stats(Token *token);
  void mStat(Token *token);
  void stat(Token *token);

  void in(Token *token);
  void out(Token *token);
  void _if(Token *token);
  void loop(Token *token);
  void assign(Token *token);
  void RO(Token *token);
  void label(Token *token);
  void _goto(Token *token);
};