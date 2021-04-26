#include <stdio.h>

using namespace std;

class Parser
{
public:
  // Parser();
  int parser(); // istream *input
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