#include <stdio.h>
#include <istream>

using namespace std;

class Parser
{
public:
  void parser(istream *input); // TODO: change to the tree root Node

private:
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

  void in();
  void out();
  void _if();
  void loop();
  void assign();
  void RO();
  void label();
  void _goto();
};