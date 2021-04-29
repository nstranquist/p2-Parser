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
  if(this->token->tokenInstance == "main") {
    block();
  }
  else
    cout << "Error: 'main' expected in program" << endl;
}
void Parser::block()
{
  // begin <vars><stats> end
  if(this->token->tokenInstance == "begin") {
    vars();
    stats();
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "end") {
      cout << "block used correctly" << endl;
      return;
    }
    else
      cout << "Error: 'end' expected after block" << endl;
  }
  else
    cout << "Error: 'begin' expected at the start of a block" << endl;
}
void Parser::vars()
{
  // empty | data Identifier := Integer ; <vars>
  // 1. If empty, return;
  if(this->token->tokenInstance == "") {
    cout << "token in vars() is empty. returning" << endl;
    return;
  }
  // 2. If data identifier, process token, (see semi-colon?), call vars() again
  if(this->token->tokenInstance == "data") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == ":=") {
        this->token = this->getTokenFromScanner();
        // Check that next is an integer
        if(this->token->tokenID == NUM_tk) {
          cout << "<vars> used correctly!" << endl;
          return;
        }
        else
          cout << "Error: Integer expected after ':=' in variable declarations" << endl;
      }
      else
        cout << "Error: ':=' expected after Identifier in variable declarations" << endl;
    }
    else
      cout << "Error: Identifier expected after 'data' in variable declarations" << endl;
  }
  else
    cout << "Error: 'data' expected at the beginning of a variable declaration" << endl;
}
void Parser::expr()
{
  // <N> - <expr> | <N>
  // 1. Calculate First Set
  // firstSetOfN = {"*", "(", IDENT_tk, NUM_tk} // is * or R() -> (<expr) | Identifier | Integer
  // string firstSetOfN = {"*", "("};
  // so if firstSetOfN.includes(token->tokenInstace), call just N()

  // 2. Either <N> - <expr> or <N>
  N();
  this->token = this->getTokenFromScanner();

  if (this->token->tokenInstance == "-") // && this->token->tokenID == OpDelim_tk
  {
    cout << "Processing minus operator. Refreshing token" << endl;
    this->token = this->getTokenFromScanner();
    expr();
    return;
  }
  else
    return;
}

void Parser::N()
{
  // <A> / <N> | <A> * <N> | <A>
  A();
  // Use lookahead to detect if / or *
  this->token = this->getTokenFromScanner();
  if (this->token->tokenInstance == "/" || this->token->tokenInstance == "*")
  {
    // 1. handle / or * sign
    cout << "Handling token" << endl;
    // 2. Call N
    N();
  }
  else
    return;
}
void Parser::A()
{
  // <M> + <A> | <M>
  // 1. User First Sets and Lookahead to see if M + A or M
  M();
  this->token = this->getTokenFromScanner();
  if (this->token->tokenInstance == "+") {
    cout << "handling '+' token and processing next" << endl;
    A();
  }
  else
    return;
}
void Parser::M()
{
  // * <M> | <R>
  // 1. if mult terminal, consume it and call M

  if (this->token->tokenInstance == "*") {
    cout << "processing '*' token and processing next" << endl;
    this->token = this->getTokenFromScanner();
    M();
  }
  else
    R();
}
void Parser::R()
{
  // (<expr) | Identifier | Integer
  // 1. If parens, call expr,
  if (this->token->tokenInstance == "(") {
    expr();
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == ")") {
      cout << "<R> with parens used correctly" << endl;
      return;
    }
    else
      cout << "Error: Missing closing ')' from R" << endl;
  }
  else if(this->token->tokenID == IDENT_tk) {
    cout << "Is Identifier in R(). Returning" << endl;
    return;
  }
  else if(this->token->tokenID == NUM_tk) {
    cout << "Is Numeric/Integer in R(). Returning" << endl;
    return;
  }
  else {
    cout << "Error: Unrecognized token in R()" << endl;
    this->printToken(this->token);
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
  if (this->token->tokenInstance == "") {
    cout << "Token is empty. returning" << endl;
    return;
  }
  stat();
  mStat();
}
void Parser::stat()
{
  // <in>; | <out>; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
  // 1. Check which category the token is in
  if(this->token->tokenInstance == "begin") {
    cout << "token is a block. processing and consuming next." << endl;
    block();
    return;
  }
  else {
    if(this->token->tokenInstance == "getter") {
      cout << "Token is <in>" << endl;
      in();
    }
    else if (this->token->tokenInstance == "outter") {
      cout << "Token is <out>" << endl;
      out();
    }
    else if(this->token->tokenInstance == "if") {
      cout << "token is an if statement. processing and consuming next." << endl;
      _if();
    }
    else if(this->token->tokenInstance == "loop") {
      cout << "token is a loop. processing and consuming next." << endl;
      loop();
    }
    else if (this->token->tokenInstance == "assign") {
      cout << "token is an assign. processing and consuming next." << endl;
      assign();
    }
    else if(this->token->tokenInstance == "proc") {
      cout << "token is a <goto> (proc Identifier). No idea what this means. processing and consuming next." << endl;
      _goto();
    }
    else if(this->token->tokenInstance == "void") {
      cout << "token is a label (void identifier). processing and consuming next." << endl;
      label();
    }
    else {
      cout << "Error: Token could not be identified (yet)" << endl;
      this->printToken(this->token);
    }

    // Since these aren't blocks, they would all require semi-colons at the end
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == ";") {
      cout << "';' used correctly in stat()" << endl;
      return;
    }
    else
      cout << "Error: ';' expected" << endl;
  }
}

void Parser::in()
{
  // getter Identifier (??)
  cout << "'in' getter Identifier" << endl;
  if(this->token->tokenInstance == "getter") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "'in' statement used correctly!" << endl;
      return;
    }
    else
      cout << "Error: Identifier expected after 'in' keyword" << endl;
  }
  else
    cout << "Error: 'in' missing from in statement(?)" << endl;
}
void Parser::out()
{
  // outter <expr>
  cout << "'outer' expr()" << endl;
  if(this->token->tokenInstance == "outer") {
    expr();
  }
  else
    cout << "Error: 'outer' missing from outer statement(?)" << endl;
}
void Parser::_if()
{
  // if [ <expr> <RO> <expr> ] then <stat>
  cout << "'if' identifier" << endl;
  cout << "[ expected" << endl;
  if(this->token->tokenInstance == "if") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "[") {
      cout << "if statement checks out! if [ ... " << endl;
      expr();
      RO();
      expr();
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == "]") {
        this->token = this->getTokenFromScanner();
        if(this->token->tokenInstance == "then") {
          cout << "if statement is FULLY good! if [ ... ] then stat()" << endl;
          stat();
        }
        else
          cout << "Error: 'then' missing from if statement" << endl;
      }
      else
        cout << "Error: ']' missing from if statement" << endl;
    }
    else
      cout << "Error: '[' missing from if statement" << endl;
  }
  else
    cout << "Error: 'if' missing from if statement(?)" << endl;
}
void Parser::loop()
{
  // loop [ <expr> <RO> <expr> ] <stat>
  cout << "'loop' identifier" << endl;
  cout << "[ expected" << endl;
  if(this->token->tokenInstance == "loop") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "[") {
      expr();
      RO();
      expr();
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == "]") {
        stat();
      }
      else
        cout << "Error: ']' missing from loop statement" << endl;
    }
    else
      cout << "Error: '[' missing from loop statement" << endl;
  }
  else
    cout << "Error: 'loop' missing from loop statement(?)" << endl;
}
void Parser::assign()
{
  // assign Identifier := <expr>
  cout << "'assign' keyword" << endl;
  cout << "identifier expected" << endl;
  cout << ":= expected" << endl;
  if(this->token->tokenInstance == "assign") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == ":=") {
        cout << "'assign' used correctly!" << endl;
        expr();
      }
      else
        cout << "':=' missing from assign statement" << endl;
    }
    else
      cout << "Identifier expected after assign keyword." << endl;
  }
  else
    cout << "'assign' missing from assign statement(?)" << endl;
}
void Parser::RO()
{
  // => | =< | == | [==] (3 tokens) | %
  if (this->token->tokenInstance == "=>")
  {
    cout << "Is =>" << endl;
    return;
  }
  else if (this->token->tokenInstance == "=<")
  {
    cout << "Is =<" << endl;
    return;
  }
  else if (this->token->tokenInstance == "==")
  {
    cout << "Is ==" << endl;
    return;
  }
  else if (this->token->tokenInstance == "[" || this->token->tokenInstance == "==" || this->token->tokenInstance == "]")
  {
    cout << "Is [==]" << endl;
    return;
  }
  else if (this->token->tokenInstance == "%")
  {
    cout << "Is %" << endl;
    return;
  }
  else
  {
    cout << "Error: Symbol Did Not Match. Symbol: " << this->token->tokenInstance << "\n"
         << endl;
    return;
  }
}
void Parser::label()
{
  // void Identifier
  cout << "'void' keyword expected, followed by Identifier" << endl;
  if(this->token->tokenInstance == "void") {
    cout << "has 'void'. Processing and getting next token" << endl;
    // Next token should be identifier, or error
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "next token is Identifier, we are ok!" << endl;
      return ;
    }
    else {
      cout << "Error: next token is NOT identifier, we are NOT okay." << endl;
    }
  }
  else {
    cout << "Error: 'void' not detected in label()" << endl;
  }
}
void Parser::_goto()
{
  // proc Identifier
  cout << "'proc' keyword expected, followed by Identifier" << endl;
  if(this->token->tokenInstance == "proc") {
    cout << "has 'proc'. Processing and getting next token" << endl;
    // Next token should be identifier, or error
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "next token is Identifier, we are ok!" << endl;
      return ;
    }
    else {
      cout << "Error: next token is NOT identifier, we are NOT okay." << endl;
    }
  }
  else {
    cout << "Error: 'proc' not detected in _goto()" << endl;
  }
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
