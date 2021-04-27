#include <istream>
#include <iostream>

#include "parser.h"
#include "scanner.h"

using namespace std;

void Parser::parser(istream *input)
{
  cout << "1. Parsing...\n\n";

  Scanner scanner;

  string delimiterSpace = " ";

  Token *token;

  token = scanner.getToken(input);
  while (token)
  {
    if (!token)
    {
      cout << "Error! Token not existent\n"
           << endl;
      break;
    }
    cout << "line " << token->lineNumber << ": "
         << token->tokenInstance
         << "\tid: " << token->tokenID << " "
         << scanner.tokenNames[token->tokenID] << "\n"
         << endl;
    token = scanner.getToken(input);
  }
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