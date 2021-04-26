#include <istream>
#include <iostream>

#include "parser.h"
// #include "scanner.h"

using namespace std;

int Parser::parser() // istream *input
{
  cout << "Parsing..." << endl;

  // while(Node n = getToken()) { ... }

  return 0;
}

// Implement the Non-Terminal Functions (20)

void Parser::program() {}
void Parser::block() {}
void Parser::vars() {}
void Parser::expr() {}

void Parser::N() {}
void Parser::A() {}
void Parser::M() {}
void Parser::R() {}
void Parser::stats() {}
void Parser::mStat() {}

void Parser::in() {}
void Parser::out() {}
void Parser::_if() {}
void Parser::loop() {}
void Parser::assign() {}
void Parser::RO() {}
void Parser::label() {}
void Parser::_goto() {}