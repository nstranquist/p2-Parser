#include <istream>
#include <iostream>
#include <string>
#include <stdio.h>

#include "parser.h"
#include "scanner.h"

using namespace std;

void Parser::parser(istream *input)
{
  cout << "1. Parsing...\n\n";
  this->input = input;

  Scanner scanner;

  // Is this already our lookahead token, or do we get one more? --> This is the lookahead.
  this->token = scanner.getToken(input);

  program(); 

  if (token->tokenID == EOF_tk)
  {
    cout << "Parse was OK" << endl;
  }
  else
  {
    cout << "Error, EOF token not found" << endl;
  }
  // nextToken = scanner.getToken(input);

  // printToken(token);
  // if (nextToken)
  //   printToken(nextToken);

  // program(token);

  cout << "Finished Processing\n\n";

  // while (token)
  // {
  //   if (!token)
  //   {
  //     cout << "Error! Token not existent\n"
  //          << endl;
  //     break;
  //   }
  //   cout << "line " << token->lineNumber << ": "
  //        << token->tokenInstance
  //        << "\tid: " << token->tokenID << " "
  //        << scanner.tokenNames[token->tokenID] << "\n"
  //        << endl;
  //   token = scanner.getToken(input);
  // }
}

// Implement the Non-Terminal Functions (20) or 19

void Parser::program()
{
  // <vars>main<block>
  // 1. Call variables
  // 2. main? --> it's a "structural identifier", similar to "begin" and "end"
  // 3. Call block

  vars();
  block();
}
void Parser::block()
{
  // begin <vars><stats> end
  vars();
  cout << "\nskipping stats() to test vars()" << endl;
  // stats();
}
void Parser::vars()
{
  // empty | data Identifier := Integer ; <vars>
  // 1. If empty, return;
  cout << "\ntoken instance: " << this->token->tokenInstance << endl;

  if(this->token->tokenInstance == "") {
    cout << "token in vars() is empty. returning" << endl;
    return;
  }
  // 2. If data identifier, process token, (see semi-colon?), call vars() again
  else if (this->token->tokenID == IDENT_tk) {
    // Do we still make sure it is an integer or something??
    cout << "token in vars() is an identifier. Processing and getting next..." << endl;
    token = this->getTokenFromScanner();
    // process for semi-colon?
    vars();
  }
  else {
    cout << "Error: Token in vars() does not match the possible tokens" << endl;
  }
}
void Parser::expr()
{
  // <N> - <expr> | <N>
  // 1. Calculate First Set
  // firstSetOfN = {"*", "(", IDENT_tk, NUM_tk} // is * or R() -> (<expr) | Identifier | Integer
  // string firstSetOfN = {"*", "("};
  // so if firstSetOfN.includes(token->tokenInstace), call just N()

  // 2. Either <N> - <expr> or <N>
  if (this->token->tokenInstance == "*" || this->token->tokenInstance == "(")
  {
    cout << "need to call N and expr" << endl;
    N();
    cout << "expecting minus identifier" << endl;
    expr();
  }
  else
    N();
}

void Parser::N()
{
  // <A> / <N> | <A> * <N> | <A>
  // 1. Use lookahead to detect if / or *
  bool mult = false;
  bool div = false;
  A();
  if (mult || div)
  {
    // 1. handle / or * sign? terminal, so....
    // 2. Call N
    N();
  }
}
void Parser::A()
{
  // <M> + <A> | <M>
  // 1. User First Sets and Lookahead to see if M + A or M
  bool useM = false;
  M();
  if (useM)
  {
    // handle '+'
    A();
  }
}
void Parser::M()
{
  // * <M> | <R>
  // 1. if mult terminal, consume it and call M
  bool isMult = false;
  if (isMult)
    M();
  else
    R();
}
void Parser::R()
{
  // (<expr) | Identifier | Integer
  // 1. If parens, call expr,
  bool isParens = false;
  if (isParens)
  {
    expr();
  }
  else
  {
    // Is Identifier or Integer. Return
    return;
  }
}
void Parser::stats()
{
  // <stat> <mStat>
  stat();
  mStat();
}
void Parser::mStat()
{
  // empty | <stat> <mStat>
  // 1. Use lookahead to detect EOF token
  bool isEOF = false;
  if (isEOF || !token)
  {
    cout << "is EOF or token is empty" << endl;
    return;
  }
  stat();
  mStat();
}
void Parser::stat()
{
  // <in>; | <out>; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
  // 1. Calculate First (and Follow?) Sets?
}

void Parser::in()
{
  // getter Identifier (??)
  cout << "getter Identifier" << endl;
}
void Parser::out()
{
  // outter <expr>
  cout << "outer" << endl;
  expr();
}
void Parser::_if()
{
  // if [ <expr> <RO> <expr> ] then <stat>
  expr();
  RO();
  expr();
  stat();
}
void Parser::loop()
{
  // loop [ <expr> <RO> <expr> ] <stat>
  expr();
  RO();
  expr();
  stat();
}
void Parser::assign()
{
  // assign Identifier := <expr>
  cout << "assign Identifier" << endl;
  expr();
}
void Parser::RO()
{
  // => | =< | == | [==] (3 tokens) | %
  string example = "=>";

  if (example == "=>")
  {
    cout << "Is =>" << endl;
  }
  else if (example == "=<")
  {
    cout << "Is =<" << endl;
  }
  else if (example == "==")
  {
    cout << "Is ==" << endl;
  }
  else if (example == "[" || example == "==" || example == "]")
  {
    cout << "Is [==]" << endl;
  }
  else if (example == "%")
  {
    cout << "Is %" << endl;
  }
  else
  {
    cout << "Error: Symbol Did Not Match. Symbol: " << example << "\n"
         << endl;
  }
}
void Parser::label()
{
  // void Identifier
  cout << "void Identifier" << endl;
}
void Parser::_goto()
{
  // proc Identifier
  cout << "proc Identifier" << endl;
}

void Parser::printToken(Token *token)
{
  Scanner scanner;

  if (!token)
    cout << "LOL that token doesn't exist." << endl;
  else
  {
    cout << "line " << token->lineNumber << ": "
         << token->tokenInstance
         << "\tid: " << token->tokenID << " "
         << scanner.tokenNames[token->tokenID] << "\n"
         << endl;
  }
}

Token* Parser::getTokenFromScanner() {
  Scanner scanner;
  Token *token;

  token = scanner.getToken(this->input);

  cout << "new token: ";
  this->printToken(token);

  return token;
}

bool Parser::isEofToken(Token *token) {
  if(token->tokenID == EOF_tk) {
    return true;
  }
  return false;
}

// bool Parser::isInSet(string value, string set)
// {
//   string setArray = set.split(' ');
//   int arrayLength = sizeof(set) / sizeof(set[0]);
//   for (int i = 0; i < arrayLength; i++)
//   {
//     // cout << "keyword: " << this->keywords[i] << endl;

//     if (set[i] == value)
//     {
//       // cout << "--> matching reserved word!" << endl;
//       return true;
//     }
//   }

//   return false;
// }