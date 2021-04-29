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

  Scanner *scanner = new Scanner();
  this->scanner = scanner; // save to state to retain info about line numbers and stuff

  // Is this already our lookahead token, or do we get one more? --> This is the lookahead.
  this->token = this->scanner->getToken(input);

  this->printToken(token);

  program();

  if (token->tokenID == EOF_tk)
  {
    cout << "Parse was OK" << endl;
  }
  else
  {
    this->throwError("Error, EOF token not found");
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
    cout << "Process 'main' keyword token and consume next" << endl;
    this->token = this->getTokenFromScanner();
    block();
  }
  else
    this->throwError("Error: 'main' expected in program");
}
void Parser::block()
{
  // begin <vars><stats> end
  if(this->token->tokenInstance == "begin") {
    cout << "Processing 'begin' keyword token and consuming next" << endl;
    this->token = this->getTokenFromScanner();
    vars();
    stats();
    // this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "end") {
      cout << "block used correctly. Processing 'end' and consuming next" << endl;
      this->token = this->getTokenFromScanner();
      return;
    }
    else
      this->throwError("Error: 'end' expected after block");
  }
  else
    this->throwError("Error: 'begin' expected at the start of a block");
}
void Parser::vars()
{
  // empty | data Identifier := Integer ; <vars>
  // 1. If empty, return;
  cout << "vars()" << endl;
  // Question: What to do if token is 'main'... would not be empty... Can I just use 'else if not data, return' ?
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
          this->token = this->getTokenFromScanner();
          if(this->token->tokenInstance == ";") {
            cout << "<vars> used correctly! Processing and calling again" << endl;
            this->token = this->getTokenFromScanner();
            vars();
            return;
          }
          else
            this->throwError("Error: ';' missing after Integer in variable declaration");
        }
        else
          this->throwError("Error: Integer expected after ':=' in variable declarations");
      }
      else
        this->throwError("Error: ':=' expected after Identifier in variable declarations");
    }
    else
      this->throwError("Error: Identifier expected after 'data' in variable declarations");
  }
  else {
    cout << "Will assume empty <vars> and return" << endl;
    return;
    // cout << "Error: 'data' expected at the beginning of a variable declaration" << endl;
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
  cout << "expr()" << endl;
  N();
  // this->token = this->getTokenFromScanner();

  if (this->token->tokenInstance == "-") // && this->token->tokenID == OpDelim_tk
  {
    cout << "Processing minus operator. Refreshing token" << endl;
    this->token = this->getTokenFromScanner();
    expr();
    return;
  }
  else {
    cout << "expr() called as is, nothing returned additionally. Current token: " << endl;
    this->printToken(this->token);
    // this->token = this->getTokenFromScanner();
    return;
  }
}

void Parser::N()
{
  // <A> / <N> | <A> * <N> | <A>
  cout << "N()" << endl;

  A();
  // Use lookahead to detect if / or *
  // this->token = this->getTokenFromScanner();
  if (this->token->tokenInstance == "/" || this->token->tokenInstance == "*")
  {
    // 1. handle / or * sign
    cout << "Handling token: " << this->token->tokenInstance << endl;
    // 2. Call N
    N();
  }
  else
    return;
}
void Parser::A()
{
  cout << "A()" << endl;
  // <M> + <A> | <M>
  // 1. User First Sets and Lookahead to see if M + A or M
  M();
  // this->token = this->getTokenFromScanner();
  if (this->token->tokenInstance == "+") {
    cout << "handling '+' token and processing next" << endl;
    this->token = this->getTokenFromScanner();
    A();
    return;
  }
  else
    return;
}
void Parser::M()
{
  cout << "M()" << endl;
  // * <M> | <R>
  // 1. if mult terminal, consume it and call M
  this->printToken(this->token);
  if (this->token->tokenInstance == "*") {
    cout << "processing '*' token and processing next" << endl;
    this->token = this->getTokenFromScanner();
    M();
    return;
  }
  else
    R();
}
void Parser::R()
{
  cout << "R()" << endl;
  // (<expr) | Identifier | Integer
  this->printToken(this->token);

  // 1. If parens, call expr,
  if (this->token->tokenInstance == "(") {
    expr();
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == ")") {
      cout << "<R> with parens used correctly. Processing and returning" << endl;
      this->token = this->getTokenFromScanner();
      return;
    }
    else
      this->throwError("Error: Missing closing ')' from R");
  }
  else if(this->token->tokenID == IDENT_tk) {
    cout << "Token: " << this->token->tokenInstance << " Is an Identifier in R(). Processing and Returning" << endl;
    this->token = this->getTokenFromScanner();
    return;
  }
  else if(this->token->tokenID == NUM_tk) {
    cout << "Token: " << this->token->tokenInstance << " Is a Numeric/Integer in R(). Processing and Returning" << endl;
    this->token = this->getTokenFromScanner();
    return;
  }
  else {
    cout << "About to throw error. Unrecognized token: " << endl;
    this->printToken(this->token);
    this->throwError("Error: Unrecognized token in R()");
    return;
  }
}
void Parser::stats()
{
  cout << "stats()" << endl;
  // <stat> <mStat>
  stat();
  mStat();
}
void Parser::mStat()
{
  cout << "mStat()" << endl;
  // empty | <stat> <mStat>
  // 1. Use lookahead to detect EOF token
  if (this->token->tokenInstance == "") {
    cout << "Token is empty. returning" << endl;
    return;
  }
  stat();
  cout << "Stat has been called. Now processing and consuming next(?)" << endl;
  if(this->token->tokenInstance == "end") {
    cout << "is end of block. skipping mStat loop" << endl;
    return;
  }
  mStat();
}
void Parser::stat()
{
  cout << "stat(). Token: " << endl;
  this->printToken(this->token);
  // <in>; | <out>; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
  // 1. Check which category the token is in
  if(this->token->tokenInstance == "begin") {
    cout << "token is a block. processing and consuming next." << endl;
    block();
    this->token = this->getTokenFromScanner();
    return;
  }
  else if(this->token->tokenInstance == "end") {
    cout << "token is 'end'. Returning, will process in block()" << endl;
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
      this->throwError("Error: Token could not be identified (yet)");
      this->printToken(this->token);
      return;
    }

    cout << "checking for semicolon at the end. Token: " << endl;
    this->printToken(this->token);
    // Since these aren't blocks, they would all require semi-colons at the end
    // this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == ";") {
      cout << "';' used correctly in stat(). Processing and consuming next:" << endl;
      this->token = this->getTokenFromScanner();
      return;
    }
    else
      this->throwError("Error: ';' expected");
  }
}

void Parser::in()
{
  cout << "in()" << endl;
  // getter Identifier (??)
  // cout << "'in' getter Identifier" << endl;
  if(this->token->tokenInstance == "getter") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "'in' statement used correctly! Processing and returning" << endl;
      this->token = this->getTokenFromScanner();
      return;
    }
    else
      this->throwError("Error: Identifier expected after 'in' keyword");
  }
  else
    this->throwError("Error: 'in' missing from in statement(?)");
}
void Parser::out()
{
  cout << "out()" << endl;
  // outter <expr>
  // cout << "'outter' expr()" << endl;
  if(this->token->tokenInstance == "outter") {
    cout << "outter expression used correctly. Processing and returning" << endl;
    this->token = this->getTokenFromScanner();
    expr();
    return;
  }
  else
    this->throwError("Error: 'outter' missing from outter statement(?)");
}
void Parser::_if()
{
  cout << "if()" << endl;
  // if [ <expr> <RO> <expr> ] then <stat>
  // cout << "'if' identifier" << endl;
  // cout << "[ expected" << endl;
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
          cout << "if statement is FULLY good! if [ ... ] then stat(). Processing and returning" << endl;
          this->token = this->getTokenFromScanner();
          stat();
        }
        else
          this->throwError("Error: 'then' missing from if statement");
      }
      else
        this->throwError("Error: ']' missing from if statement");
    }
    else
      this->throwError("Error: '[' missing from if statement");
  }
  else
    this->throwError("Error: 'if' missing from if statement(?)");
}
void Parser::loop()
{
  cout << "loop()" << endl;
  // loop [ <expr> <RO> <expr> ] <stat>
  // cout << "'loop' identifier" << endl;
  // cout << "[ expected" << endl;
  if(this->token->tokenInstance == "loop") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "[") {
      this->token = this->getTokenFromScanner();
      expr();
      cout << "expr returned" << endl;
      RO();
      cout << "RO returned" << endl;
      expr();
      cout << "second expr returned" << endl;
      this->printToken(this->token);
      // cout << "getting next token" << endl;

      // this->token = this->getTokenFromScanner();
      // this->printToken(this->token);

      cout << "In loop, we just got the next token. Should be ']'" << endl;
      this->printToken(this->token);
      if(this->token->tokenInstance == "]") {
        this->token = this->getTokenFromScanner();
        cout << "In loop, token should be next now:"<< endl;
        this->printToken(this->token);
        stat();
      }
      else
        this->throwError("Error: ']' missing from loop statement");
    }
    else
      this->throwError("Error: '[' missing from loop statement");
  }
  else
    this->throwError("Error: 'loop' missing from loop statement(?)");
}
void Parser::assign()
{
  cout << "assign()" << endl;
  // assign Identifier := <expr>
  // cout << "'assign' keyword" << endl;
  // cout << "identifier expected" << endl;
  // cout << ":= expected" << endl;
  if(this->token->tokenInstance == "assign") {
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == ":=") {
        cout << "'assign' used correctly! Processing and returning next" << endl;
        this->token = this->getTokenFromScanner();
        expr();
        cout << "assign() <expr> is over. returning with current token: " << endl;
        this->printToken(this->token);
        return;
      }
      else
        this->throwError("Error: ':=' missing from assign statement");
    }
    else
      this->throwError("Error: Identifier expected after assign keyword.");
  }
  else
    this->throwError("Error: 'assign' missing from assign statement(?)");
}
void Parser::RO()
{
  cout << "RO()" << endl;
  // => | =< | == | [==] (3 tokens) | %
  if (this->token->tokenInstance == "=>")
  {
    cout << "Is =>  Processing and Returning" << endl;
    this->token = this->getTokenFromScanner();
    return;
  }
  else if (this->token->tokenInstance == "=<")
  {
    cout << "Is =<  Processing and Returning" << endl;
    this->token = this->getTokenFromScanner();
    return;
  }
  else if (this->token->tokenInstance == "==")
  {
    cout << "Is ==  Processing and Returning" << endl;
    this->token = this->getTokenFromScanner();
    return;
  }
  else if (this->token->tokenInstance == "[" || this->token->tokenInstance == "==" || this->token->tokenInstance == "]")
  {
    cout << "Is [==]  Processing and Returning" << endl;
    this->token = this->getTokenFromScanner();
    return;
  }
  else if (this->token->tokenInstance == "%")
  {
    cout << "Is %  Processing and Returning" << endl;
    this->token = this->getTokenFromScanner();
    return;
  }
  else
  {
    this->throwError("Error: Symbol Did Not Match. Symbol: " + this->token->tokenInstance + "\n");
    return;
  }
}
void Parser::label()
{
  cout << "label()" << endl;
  // void Identifier
  // cout << "'void' keyword expected, followed by Identifier" << endl;
  if(this->token->tokenInstance == "void") {
    cout << "has 'void'. Processing and getting next token" << endl;
    // Next token should be identifier, or error
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "next token is Identifier, we are ok! Processing and returning" << endl;
      this->token = this->getTokenFromScanner();
      return ;
    }
    else {
      this->throwError("Error: next token is NOT identifier, we are NOT okay.");
    }
  }
  else {
    this->throwError("Error: 'void' not detected in label()");
  }
}
void Parser::_goto()
{
  cout << "_goto()" << endl;
  // proc Identifier
  cout << "'proc' keyword expected, followed by Identifier" << endl;
  if(this->token->tokenInstance == "proc") {
    cout << "has 'proc'. Processing and getting next token" << endl;
    // Next token should be identifier, or error
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "next token is Identifier, we are ok! Processing and returning" << endl;
      this->token = this->getTokenFromScanner();
      return ;
    }
    else {
      this->throwError("Error: next token is NOT identifier, we are NOT okay.");
    }
  }
  else {
    this->throwError("Error: 'proc' not detected in _goto()");
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
  Token *token;

  token = this->scanner->getToken(this->input);

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

void Parser::throwError(string message) {
  throw invalid_argument(message);
}