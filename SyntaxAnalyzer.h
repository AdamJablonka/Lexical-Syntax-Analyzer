#ifndef SYNTAXANALYZER_H
#define SYNTAXANALYZER_H

#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"

using namespace std;

class SyntaxAnalyzer
{
private:
    LexicalAnalyzer *la;
    Token *currentToken;
    Token *nextToken;
    TokenCodes nextTokenCode;
    TokenCodes currentTokenCode;
    void Functions();
    void Function();
    void ParamList();
    void Parameter();
    void CompStmt();
    void SeqOfStmt();
    void Statement();
    void Declaration();
    void IdentList();
    void Block();
    void Expression();
    void Relational();
    void IfStatement();
    void WhileStatement();
    void DoWhileStatement();
    void forStatement();
    void ForStatement();
    void PrintfStatement();
    void ScanfStatement();
    void ReturnStatement();
    void Or();
    void And();
    void Equality();
    void Term();
    void Factor();
    void Unary();
    void Primary();
    void error(const string &errorMessage);

public:
    SyntaxAnalyzer(LexicalAnalyzer *);
    void Program();
};

#endif