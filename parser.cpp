#include <istream>
#include <iostream>
#include <string>
#include <stdio.h>

#include "parser.h"
#include "node.h"
#include "scanner.h"
#include "tree.h"

using namespace std;

Node* Parser::parser(istream *input)
{
  ParseTree *tree = new ParseTree();
  this->tree = tree;

  Node *root_node;

  cout << "1. Parsing...\n\n";
  this->input = input;

  Scanner *scanner = new Scanner();
  this->scanner = scanner; // save to state to retain info about line numbers and stuff

  // Is this already our lookahead token, or do we get one more? --> This is the lookahead.
  this->token = this->scanner->getToken(input);

  this->printToken(token);

  root_node = program();

  cout << "Root node of parse tree returned from program. Printing it now:" << endl;
  this->tree->printNode(root_node);

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

  return root_node;

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

Node* Parser::program()
{
  // <vars>main<block>
  // 1. Call variables
  // 2. main? --> it's a "structural identifier", similar to "begin" and "end"
  // 3. Call block
  Node *root;
  string rootLabel = "program";
  root = this->tree->buildTree(rootLabel);
  this->tree->printNode(root);

  Node *varsNode;
  varsNode = vars();
  root->nodes.push_back(varsNode);
  this->tree->printNode(varsNode);
  if(this->token->tokenInstance == "main") {
    cout << "Discard 'main' keyword token and consume next" << endl;
    this->token = this->getTokenFromScanner();
    Node *blockNode;
    blockNode = block();
    root->nodes.push_back(blockNode);
    return root;
  }
  else
    this->throwError("Error: 'main' expected in program");
}
Node* Parser::block()
{
  Node *subRoot;
  subRoot = this->tree->insertNode("block");
  cout << "block()" << endl;
  // begin <vars><stats> end
  if(this->token->tokenInstance == "begin") {
    cout << "Processing 'begin' keyword token and consuming next" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    Node *varsNode, *statsNode;
    varsNode = vars();
    statsNode = stats();
    cout << "end of stats() in block()" << endl;
    // this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "end") {
      cout << "block used correctly. Processing 'end' and consuming next" << endl;
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      return subRoot;
    }
    else
      this->throwError("Error: 'end' expected after block");
  }
  else
    this->throwError("Error: 'begin' expected at the start of a block");

  return subRoot;
}
Node* Parser::vars()
{
  Node *subRoot;
  subRoot = this->tree->insertNode("vars");

  // empty | data Identifier := Integer ; <vars>
  // 1. If empty, return;
  cout << "vars()" << endl;
  // Question: What to do if token is 'main'... would not be empty... Can I just use 'else if not data, return' ?
  if(this->token->tokenInstance == "") {
    cout << "token in vars() is empty. returning" << endl;
    return subRoot;
  }
  // 2. If data identifier, process token, (see semi-colon?), call vars() again
  if(this->token->tokenInstance == "data") {
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == ":=") {
        subRoot->tokens.push_back(this->token);
        this->token = this->getTokenFromScanner();
        // Check that next is an integer
        if(this->token->tokenID == NUM_tk) {
          subRoot->tokens.push_back(this->token);
          this->token = this->getTokenFromScanner();
          if(this->token->tokenInstance == ";") {
            cout << "<vars> used correctly! Processing and calling again" << endl;
            subRoot->tokens.push_back(this->token);
            this->token = this->getTokenFromScanner();
            Node *varsNode;
            varsNode = vars();
            subRoot->nodes.push_back(varsNode);
            return subRoot;
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
    return subRoot;
    // cout << "Error: 'data' expected at the beginning of a variable declaration" << endl;
  }
}
Node* Parser::expr()
{
  Node *subRoot;
  subRoot = this->tree->insertNode("expr");
  // <N> - <expr> | <N>
  // 1. Calculate First Set
  // firstSetOfN = {"*", "(", IDENT_tk, NUM_tk} // is * or R() -> (<expr) | Identifier | Integer
  // string firstSetOfN = {"*", "("};
  // so if firstSetOfN.includes(token->tokenInstace), call just N()

  // 2. Either <N> - <expr> or <N>
  cout << "expr()" << endl;
  Node *nNode;
  nNode = N();
  subRoot->nodes.push_back(nNode);
  cout << "token after expr(): "; this->printToken(this->token);
  // this->token = this->getTokenFromScanner();

  if (this->token->tokenInstance == "-") // && this->token->tokenID == OpDelim_tk
  {
    cout << "Processing minus operator. Refreshing token" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    Node *exprToken;
    exprToken = expr();
    return subRoot;
  }
  else {
    cout << "expr() called as is, nothing returned additionally. Current token: " << endl;
    this->printToken(this->token);
    // this->token = this->getTokenFromScanner();
    return subRoot;
  }
}

Node* Parser::N()
{
  Node *subRoot;
  subRoot = this->tree->insertNode("N");
  // <A> / <N> | <A> * <N> | <A>
  cout << "N()" << endl;

  Node *aNode;
  aNode = A();
  subRoot->nodes.push_back(aNode);

  if (this->token->tokenInstance == "/" || this->token->tokenInstance == "*")
  {
    // 1. handle / or * sign
    cout << "Token is '/' or '*'. Handling token: " << this->token->tokenInstance << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    // 2. Call N
    Node *nNode;
    nNode = N();
    subRoot->nodes.push_back(nNode);
    return subRoot;
  }
  else
    return subRoot;
}
Node* Parser::A()
{
  Node *subRoot;
  subRoot = this->tree->createNode("A");
  cout << "A()" << endl;
  // <M> + <A> | <M>
  // 1. User First Sets and Lookahead to see if M + A or M
  Node *mNode;
  mNode = M();
  subRoot->nodes.push_back(mNode);
  // this->token = this->getTokenFromScanner();
  if (this->token->tokenInstance == "+") {
    cout << "handling '+' token and processing next" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    Node *aNode;
    aNode = A();
    subRoot->nodes.push_back(aNode);
    return subRoot;
  }
  else
    return subRoot;
}
Node* Parser::M()
{
  Node *subRoot;
  subRoot = this->tree->createNode("M");
  cout << "M()" << endl;
  // * <M> | <R>
  // 1. if mult terminal, consume it and call M
  this->printToken(this->token);
  if (this->token->tokenInstance == "*") {
    cout << "processing '*' token and processing next" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    Node *mNode;
    mNode = M();
    subRoot->nodes.push_back(mNode);
    return subRoot;
  }
  else {
    Node *rNode;
    rNode = R();
    subRoot->nodes.push_back(rNode);
    return subRoot;
  }
}
Node* Parser::R()
{
  Node *subRoot;
  subRoot = this->tree->createNode("R");
  cout << "R()" << endl;
  // (<expr) | Identifier | Integer
  this->printToken(this->token);

  // 1. If parens, call expr,
  if (this->token->tokenInstance == "(") {
    cout << "Got opening parens. Processing and calling expr()" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    Node *exprNode;
    exprNode = expr();
    subRoot->nodes.push_back(exprNode);
    if(this->token->tokenInstance == ")") {
      cout << "<R> with parens used correctly. Processing and returning" << endl;
      // Add to subRoot as a token
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      return subRoot;
    }
    else
      this->throwError("Error: Missing closing ')' from R");
  }
  else if(this->token->tokenID == IDENT_tk) {
    cout << "Token: " << this->token->tokenInstance << " Is an Identifier in R(). Processing and Returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else if(this->token->tokenID == NUM_tk) {
    cout << "Token: " << this->token->tokenInstance << " Is a Numeric/Integer in R(). Processing and Returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else {
    cout << "About to throw error. Unrecognized token: " << endl;
    this->printToken(this->token);
    this->throwError("Error: Unrecognized token in R()");
    return subRoot;
  }
}
Node* Parser::stats()
{
  Node *subRoot;
  subRoot = this->tree->createNode("stats");
  cout << "stats()" << endl;
  // <stat> <mStat>
  Node *statNode, *mStatNode;
  statNode = stat();
  subRoot->nodes.push_back(statNode);
  mStatNode = mStat();
  subRoot->nodes.push_back(mStatNode);

  return subRoot;
}
Node* Parser::mStat()
{
  Node *subRoot;
  subRoot = this->tree->createNode("mStat");
  cout << "mStat()" << endl;
  // empty | <stat> <mStat>
  // 1. Use lookahead to detect EOF token
  if (this->token->tokenInstance == "") {
    cout << "Token is empty. returning" << endl;
    return subRoot;
  }
  Node *statNode;
  statNode = stat();
  subRoot->nodes.push_back(statNode);
  cout << "Stat has been called. Now processing and consuming next(?)" << endl;
  if(this->token->tokenInstance == "end") {
    cout << "is end of block. skipping mStat loop" << endl;
    return subRoot;
  }
  Node *mStatNode;
  mStatNode = mStat();
  subRoot->nodes.push_back(mStatNode);

  return subRoot;
}
Node* Parser::stat()
{
  Node *subRoot;
  subRoot = this->tree->createNode("stat");
  cout << "stat(). Token: " << endl;
  this->printToken(this->token);
  // <in>; | <out>; | <block> | <if> ; | <loop> ; | <assign> ; | <goto> ; | <label> ;
  // 1. Check which category the token is in
  if(this->token->tokenInstance == "begin") {
    cout << "token is a block. processing and consuming next." << endl;
    Node *blockNode;
    blockNode = block();
    subRoot->nodes.push_back(blockNode);
    // this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else if(this->token->tokenInstance == "end") {
    cout << "token is 'end'. Returning, will process in block()" << endl;
    return subRoot;
  }
  else {
    Node *node;
    if(this->token->tokenInstance == "getter") {
      cout << "Token is <in>" << endl;
      node = in();
    }
    else if (this->token->tokenInstance == "outter") {
      cout << "Token is <out>" << endl;
      node = out();
    }
    else if(this->token->tokenInstance == "if") {
      cout << "token is an if statement. processing and consuming next." << endl;
      node = _if();
    }
    else if(this->token->tokenInstance == "loop") {
      cout << "token is a loop. processing and consuming next." << endl;
      node = loop();
      cout << "returning from loop tho" << endl;
    }
    else if (this->token->tokenInstance == "assign") {
      cout << "token is an assign. processing and consuming next." << endl;
      node = assign();
    }
    else if(this->token->tokenInstance == "proc") {
      cout << "token is a <goto> (proc Identifier). No idea what this means. processing and consuming next." << endl;
      node = _goto();
    }
    else if(this->token->tokenInstance == "void") {
      cout << "token is a label (void identifier). processing and consuming next." << endl;
      node = label();
    }
    else {
      this->throwError("Error: Token could not be identified (yet)");
      this->printToken(this->token);
      return subRoot;
    }
    subRoot->nodes.push_back(node);

    cout << "checking for semicolon at the end. Token: " << endl;
    this->printToken(this->token);
    // Since these aren't blocks, they would all require semi-colons at the end
    // this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == ";") {
      cout << "';' used correctly in stat(). Discarding and consuming next:" << endl;
      this->token = this->getTokenFromScanner();
      return subRoot;
    }
    else {
      this->throwError("Error: ';' expected");
      return subRoot;
    }
  }
}

Node* Parser::in()
{
  Node *subRoot;
  subRoot = this->tree->createNode("in");
  cout << "in()" << endl;
  // getter Identifier (??)
  // cout << "'in' getter Identifier" << endl;
  if(this->token->tokenInstance == "getter") {
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "'in' statement used correctly! Processing and returning" << endl;
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      return subRoot;
    }
    else
      this->throwError("Error: Identifier expected after 'in' keyword");
  }
  else
    this->throwError("Error: 'in' missing from in statement(?)");
}
Node* Parser::out()
{
  Node *subRoot;
  subRoot = this->tree->createNode("out");
  cout << "out()" << endl;
  // outter <expr>
  // cout << "'outter' expr()" << endl;
  if(this->token->tokenInstance == "outter") {
    cout << "outter expression used correctly. Processing and returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    Node *exprNode;
    exprNode = expr();
    subRoot->nodes.push_back(exprNode);
    return subRoot;
  }
  else
    this->throwError("Error: 'outter' missing from outter statement(?)");
}
Node* Parser::_if()
{
  Node *subRoot;
  subRoot = this->tree->createNode("if");
  cout << "if()" << endl;
  // if [ <expr> <RO> <expr> ] then <stat>
  // cout << "'if' identifier" << endl;
  // cout << "[ expected" << endl;
  if(this->token->tokenInstance == "if") {
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "[") {
      cout << "if statement checks out! if [ ... " << endl;
      subRoot->tokens.push_back(this->token);
      Node *exprNode, *roNode;
      exprNode = expr();
      subRoot->nodes.push_back(exprNode);
      roNode = RO();
      subRoot->nodes.push_back(roNode);
      exprNode = expr();
      exprNode->nodes.push_back(exprNode);
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == "]") {
        subRoot->tokens.push_back(this->token);
        this->token = this->getTokenFromScanner();
        if(this->token->tokenInstance == "then") {
          cout << "if statement is FULLY good! if [ ... ] then stat(). Processing and returning" << endl;
          subRoot->tokens.push_back(this->token);
          this->token = this->getTokenFromScanner();
          Node *statNode;
          statNode = stat();
          subRoot->nodes.push_back(statNode);
          return subRoot;
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
Node* Parser::loop()
{
  Node *subRoot;
  subRoot = this->tree->createNode("loop");
  cout << "loop()" << endl;
  // loop [ <expr> <RO> <expr> ] <stat>
  // cout << "'loop' identifier" << endl;
  // cout << "[ expected" << endl;
  if(this->token->tokenInstance == "loop") {
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    if(this->token->tokenInstance == "[") {
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      Node *exprNode, *roNode;
      exprNode = expr();
      subRoot->nodes.push_back(exprNode);
      cout << "expr returned" << endl;
      roNode = RO();
      subRoot->nodes.push_back(roNode);
      cout << "RO returned" << endl;
      exprNode = expr();
      subRoot->nodes.push_back(exprNode);
      cout << "second expr returned" << endl;
      this->printToken(this->token);
      // cout << "getting next token" << endl;

      // this->token = this->getTokenFromScanner();
      // this->printToken(this->token);

      // Process token? subRoot->tokens.push_back(this->token);
      cout << "In loop, we just got the next token. Should be ']'" << endl;
      this->printToken(this->token);
      if(this->token->tokenInstance == "]") {
        subRoot->tokens.push_back(this->token);
        this->token = this->getTokenFromScanner();
        cout << "In loop, token should be next now:"<< endl;
        this->printToken(this->token);
        Node *statNode;
        statNode = stat();
        subRoot->nodes.push_back(statNode);
        return subRoot;
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
Node* Parser::assign()
{
  Node *subRoot;
  subRoot = this->tree->createNode("assign");
  cout << "assign()" << endl;
  // assign Identifier := <expr>
  // cout << "'assign' keyword" << endl;
  // cout << "identifier expected" << endl;
  // cout << ":= expected" << endl;
  if(this->token->tokenInstance == "assign") {
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      if(this->token->tokenInstance == ":=") {
        cout << "'assign' used correctly! Processing and returning next" << endl;
        subRoot->tokens.push_back(this->token);
        this->token = this->getTokenFromScanner();
        Node *exprNode;
        exprNode = expr();
        subRoot->nodes.push_back(exprNode);
        cout << "assign() <expr> is over. returning with current token: " << endl;
        this->printToken(this->token);
        return subRoot;
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
Node* Parser::RO()
{
  Node *subRoot;
  subRoot = this->tree->createNode("RO");
  cout << "RO()" << endl;
  // => | =< | == | [==] (3 tokens) | %
  if (this->token->tokenInstance == "=>")
  {
    cout << "Is =>  Processing and Returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else if (this->token->tokenInstance == "=<")
  {
    cout << "Is =<  Processing and Returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else if (this->token->tokenInstance == "==")
  {
    cout << "Is ==  Processing and Returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else if (this->token->tokenInstance == "[" || this->token->tokenInstance == "==" || this->token->tokenInstance == "]")
  {
    cout << "Is [==]  Processing and Returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else if (this->token->tokenInstance == "%")
  {
    cout << "Is %  Processing and Returning" << endl;
    subRoot->tokens.push_back(this->token);
    this->token = this->getTokenFromScanner();
    return subRoot;
  }
  else
  {
    this->throwError("Error: Symbol Did Not Match. Symbol: " + this->token->tokenInstance + "\n");
    return subRoot;
  }
}
Node* Parser::label()
{
  Node *subRoot;
  subRoot = this->tree->createNode("label");
  cout << "label()" << endl;
  // void Identifier
  // cout << "'void' keyword expected, followed by Identifier" << endl;
  if(this->token->tokenInstance == "void") {
    cout << "has 'void'. Processing and getting next token" << endl;
    subRoot->tokens.push_back(this->token);
    // Next token should be identifier, or error
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "next token is Identifier, we are ok! Processing and returning" << endl;
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      return subRoot;
    }
    else {
      this->throwError("Error: next token is NOT identifier, we are NOT okay.");
    }
  }
  else {
    this->throwError("Error: 'void' not detected in label()");
  }
}
Node* Parser::_goto()
{
  Node *subRoot;
  subRoot = this->tree->createNode("goto");
  cout << "_goto()" << endl;
  // proc Identifier
  cout << "'proc' keyword expected, followed by Identifier" << endl;
  if(this->token->tokenInstance == "proc") {
    cout << "has 'proc'. Processing and getting next token" << endl;
    subRoot->tokens.push_back(this->token);
    // Next token should be identifier, or error
    this->token = this->getTokenFromScanner();
    if(this->token->tokenID == IDENT_tk) {
      cout << "next token is Identifier, we are ok! Processing and returning" << endl;
      subRoot->tokens.push_back(this->token);
      this->token = this->getTokenFromScanner();
      return subRoot;
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