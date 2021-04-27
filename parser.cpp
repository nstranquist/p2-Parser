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

  Scanner scanner;

  string delimiterSpace = " ";

  Token *token;

  token = scanner.getToken(input);

  program(token);

  cout << "Finished Processing" << endl;

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

void Parser::program(Token *token)
{
  // <vars>main<block>
  // 1. Call variables
  // 2. main? --> it's a "structural identifier", similar to "begin" and "end"
  // 3. Call block

  vars(token);

  block(token);
}
void Parser::block(Token *token)
{
  // begin <vars><stats> end
}
void Parser::vars(Token *token)
{
  // empty | data Identifier := Integer ; <vars>
  // 1. data Identifier? empty?
  // 2. Call vars
  if (!token || token->tokenID == IDENT_tk)
    return;
  vars(token);
}
void Parser::expr(Token *token)
{
  // <N> - <expr> | <N>
  // 1. Calculate First Set
  // 2. Either <N> - <expr> or <N>
  int choice = 1;
  if (choice == 1)
  {
    // return N() - expr();
  }
  else
    N(token);
}

void Parser::N(Token *token)
{
  // <A> / <N> | <A> * <N> | <A>
  // 1. Use lookahead to detect if / or *
  bool mult = false;
  bool div = false;
  A(token);
  if (mult || div)
  {
    // 1. handle / or * sign? terminal, so....
    // 2. Call N
    N(token);
  }
}
void Parser::A(Token *token)
{
  // <M> + <A> | <M>
  // 1. User First Sets and Lookahead to see if M + A or M
  bool useM = false;
  M(token);
  if (useM)
  {
    // handle '+'
    A(token);
  }
}
void Parser::M(Token *token)
{
  // * <M> | <R>
  // 1. if mult terminal, consume it and call M
  bool isMult = false;
  if (isMult)
    M(token);
  else
    R(token);
}
void Parser::R(Token *token)
{
  // (<expr) | Identifier | Integer
  // 1. If parens, call expr,
  bool isParens = false;
  if (isParens)
  {
    expr(token);
  }
  else
  {
    // Is Identifier or Integer. Return
    return;
  }
}
void Parser::stats(Token *token)
{
  // <stat> <mStat>
  stat(token);
  mStat(token);
}
void Parser::mStat(Token *token)
{
  // empty | <stat> <mStat>
  // 1. Use lookahead to detect EOF token
  bool isEOF = false;
  if (isEOF)
    return;
  stat(token);
  mStat(token);
}
void Parser::stat(Token *token)
{
  // <in>; | <out>; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
  // 1. Calculate First (and Follow?) Sets?
}

void Parser::in(Token *token)
{
  // getter Identifier (??)
  cout << "getter Identifier" << endl;
}
void Parser::out(Token *token)
{
  // outter <expr>
  cout << "outer" << endl;
  expr(token);
}
void Parser::_if(Token *token)
{
  // if [ <expr> <RO> <expr> ] then <stat>
  expr(token);
  RO(token);
  expr(token);
  stat(token);
}
void Parser::loop(Token *token)
{
  // loop [ <expr> <RO> <expr> ] <stat>
  expr(token);
  RO(token);
  expr(token);
  stat(token);
}
void Parser::assign(Token *token)
{
  // assign Identifier := <expr>
  cout << "assign Identifier" << endl;
  expr(token);
}
void Parser::RO(Token *token)
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
void Parser::label(Token *token)
{
  // void Identifier
  cout << "void Identifier" << endl;
}
void Parser::_goto(Token *token)
{
  // proc Identifier
  cout << "proc Identifier" << endl;
}