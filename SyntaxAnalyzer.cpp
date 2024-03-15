#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"
#include <cstdlib>

using namespace std;

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer *l)
{
    la = l;
    currentToken = nullptr;
}

void SyntaxAnalyzer::Program()
{
    currentToken = la->getNextToken();

    Functions();

    if (currentToken->getTokenCode() != EOI)
    {
        error("Unexpected tokens after program end.");
    }
}

void SyntaxAnalyzer::error(const string &errorMessage)
{
    int errorPosition = la->getCurrentTokenPosition();
    for (int i = 0; i < errorPosition; ++i)
    {
        cout << ' ';
    }
    cout << '^' << endl;

    cout << "Error: " << errorMessage << endl;
    exit(1);
}

void SyntaxAnalyzer::Functions()
{
    Function();
    while (currentToken->getTokenCode() == BOOLSYM ||
           currentToken->getTokenCode() == FLOATSYM ||
           currentToken->getTokenCode() == INTSYM ||
           currentToken->getTokenCode() == VOIDSYM)
    {
        Function();
    }
}

void SyntaxAnalyzer::Function()
{
    if (currentToken->getTokenCode() == BOOLSYM ||
        currentToken->getTokenCode() == FLOATSYM ||
        currentToken->getTokenCode() == INTSYM ||
        currentToken->getTokenCode() == VOIDSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected return type (bool, float, int, void) at the start of function.");
    }

    if (currentToken->getTokenCode() == IDENT)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected function name (identifier) after return type.");
    }

    if (currentToken->getTokenCode() == LPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '(' after function name.");
    }

    ParamList();

    if (currentToken->getTokenCode() == RPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ')' after parameter list.");
    }

    CompStmt();
}

void SyntaxAnalyzer::ParamList()
{
    if (currentToken->getTokenCode() == RPAREN)
    {
        return;
    }

    Parameter();

    while (currentToken->getTokenCode() == COMMA)
    {
        currentToken = la->getNextToken();
        Parameter();
    }
}

void SyntaxAnalyzer::Parameter()
{
    if (currentToken->getTokenCode() == BOOLSYM ||
        currentToken->getTokenCode() == FLOATSYM ||
        currentToken->getTokenCode() == INTSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected parameter type (bool, float, int).");
    }

    if (currentToken->getTokenCode() == IDENT)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected parameter name (identifier) after type.");
    }
}

void SyntaxAnalyzer::CompStmt()
{
    if (currentToken->getTokenCode() == LBRACE)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '{' at the beginning of compound statement.");
    }

    SeqOfStmt();

    if (currentToken->getTokenCode() == RBRACE)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '}' at the end of compound statement.");
    }
}

void SyntaxAnalyzer::SeqOfStmt()
{
    if (currentToken->getTokenCode() != RBRACE)
    {
        Statement();
        SeqOfStmt();
    }
}

void SyntaxAnalyzer::Statement()
{
    switch (currentToken->getTokenCode())
    {
    case IFSYM:
        IfStatement();
        break;
    case WHILESYM:
        WhileStatement();
        break;
    case DOSYM:
        DoWhileStatement();
        break;
    case FORSYM:
        ForStatement();
        break;
    case PRINTFSYM:
        PrintfStatement();
        break;
    case SCANFSYM:
        ScanfStatement();
        break;
    case RETURNSYM:
        ReturnStatement();
        break;
    case BOOLSYM:
    case INTSYM:
    case FLOATSYM:
        Declaration();
        break;
    default:
        Expression();
        break;
    }
}

void SyntaxAnalyzer::DoWhileStatement()
{
    if (currentToken->getTokenCode() == DOSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'do' keyword at the beginning of do-while statement.");
    }

    Block();

    if (currentToken->getTokenCode() == WHILESYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'while' keyword after block in do-while statement.");
    }

    if (currentToken->getTokenCode() == LPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '(' after 'while' keyword in do-while statement.");
    }

    Expression();

    if (currentToken->getTokenCode() == RPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ')' after expression in do-while statement.");
    }

    if (currentToken->getTokenCode() == SEMICOLON)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ';' at the end of do-while statement.");
    }
}

void SyntaxAnalyzer::ForStatement()
{
    if (currentToken->getTokenCode() == FORSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'for' keyword at the beginning of the for statement.");
    }

    if (currentToken->getTokenCode() == LPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '(' after 'for' keyword");
    }

    Expression();

    if (currentToken->getTokenCode() == SEMICOLON)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ';' after expression in an if statement.");
    }

    Expression();

    if (currentToken->getTokenCode() == SEMICOLON)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ';' after expression in an if statement.");
    }

    Expression();

    if (currentToken->getTokenCode() == RPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ')' after expression in an if statement.");
    }

    Block();
}

void SyntaxAnalyzer::IfStatement()
{
    if (currentToken->getTokenCode() == IFSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'if' keyword at the beginning of an if statement.");
    }

    if (currentToken->getTokenCode() == LPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '(' after 'if' keyword.");
    }

    Expression();

    if (currentToken->getTokenCode() == RPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ')' after an expression in an if statement.");
    }

    Block();

    if (currentToken->getTokenCode() == ELSESYM)
    {
        currentToken = la->getNextToken();
        Block();
    }
}

void SyntaxAnalyzer::PrintfStatement()
{
    if (currentToken->getTokenCode() == PRINTFSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'printf' keyword at the beginning of the print formatted statement.");
    }

    if (currentToken->getTokenCode() == LPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '(' after the 'printf' keyword in the print formatted statement.");
    }

    IdentList();

    if (currentToken->getTokenCode() == RPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ')' after an Identifier list in the print formatted statement.");
    }

    if (currentToken->getTokenCode() == SEMICOLON)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ';' after at the end of the print formatted statement.");
    }
}

void SyntaxAnalyzer::ReturnStatement()
{
    if (currentToken->getTokenCode() == RETURNSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'return' keyword at the beginning of the return statement.");
    }

    Expression();

    if (currentToken->getTokenCode() == SEMICOLON)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ';' at the end of the return statement.");
    }
}

void SyntaxAnalyzer::ScanfStatement()
{
    if (currentToken->getTokenCode() == SCANFSYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'printf' keyword at the beginning of the scan formatted string statement.");
    }

    if (currentToken->getTokenCode() == LPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '(' after the 'printf' keyword in the scan formatted string statement.");
    }

    IdentList();

    if (currentToken->getTokenCode() == RPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ')' after an Identifier list in the scan formatted string statement.");
    }

    if (currentToken->getTokenCode() == SEMICOLON)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ';' after at the end of the scan formatted string statement.");
    }
}

void SyntaxAnalyzer::WhileStatement()
{
    if (currentToken->getTokenCode() == WHILESYM)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected 'while' keyword at the beginning of the while statement.");
    }

    if (currentToken->getTokenCode() == LPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected '(' after 'while' keyword.");
    }

    Expression();

    if (currentToken->getTokenCode() == RPAREN)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ')' after an expression in the while loop.");
    }

    Block();
}

void SyntaxAnalyzer::Declaration()
{
    if (currentToken->getTokenCode() == BOOLSYM ||
        currentToken->getTokenCode() == INTSYM ||
        currentToken->getTokenCode() == FLOATSYM)
    {
        currentToken = la->getNextToken();

        IdentList();
    }
    else
    {
        error("Expected 'bool', 'int', or 'float' at the beginning of the declaration statement.");
    }

    if (currentToken->getTokenCode() == SEMICOLON)
    {
        currentToken = la->getNextToken();
    }
    else
    {
        error("Expected ';' at the end of the declaration statement.");
    }
}

void SyntaxAnalyzer::IdentList()
{
    if (currentToken->getTokenCode() == IDENT)
    {
        currentToken = la->getNextToken();

        if (currentToken->getTokenCode() == COMMA)
        {
            currentToken = la->getNextToken();
            IdentList();
        }
    }
    else
    {
        error("Expected identifier at the beginning of an identifier list.");
    }
}

void SyntaxAnalyzer::Block()
{
    if (currentToken->getTokenCode() == LBRACE)
    {
        CompStmt();
    }
    else
    {
        Statement();
    }
}

void SyntaxAnalyzer::Expression()
{
    if (currentToken->getTokenCode() == IDENT)
    {
        currentToken = la->getNextToken();

        if (currentToken->getTokenCode() == ASSIGN)
        {
            currentToken = la->getNextToken();
            Expression();
        }
        else
        {
            error("Expected '=' after an expression.");
        }
    }
    else
    {
        Or();
    }
}

void SyntaxAnalyzer::Or()
{
    And();
    while (currentToken->getTokenCode() == OR)
    {
        currentToken = la->getNextToken();
        And();
    }
}

void SyntaxAnalyzer::And()
{
    Equality();
    while (currentToken->getTokenCode() == AND)
    {
        currentToken = la->getNextToken();
        Equality();
    }
}

void SyntaxAnalyzer::Equality()
{
    Relational();
    while (currentToken->getTokenCode() == EQL || currentToken->getTokenCode() == NEQ)
    {
        currentToken = la->getNextToken();
        Relational();
    }
}

void SyntaxAnalyzer::Relational()
{
    Term();
    while (currentToken->getTokenCode() == LSS || currentToken->getTokenCode() == LEQ ||
           currentToken->getTokenCode() == GTR || currentToken->getTokenCode() == GEQ)
    {
        currentToken = la->getNextToken();
        Term();
    }
}

void SyntaxAnalyzer::Term()
{
    Factor();
    while (currentToken->getTokenCode() == PLUS || currentToken->getTokenCode() == MINUS)
    {
        currentToken = la->getNextToken();
        Factor();
    }
}

void SyntaxAnalyzer::Factor()
{
    Unary();
    while (currentToken->getTokenCode() == TIMES || currentToken->getTokenCode() == SLASH || currentToken->getTokenCode() == MOD)
    {
        currentToken = la->getNextToken();
        Unary();
    }
}

void SyntaxAnalyzer::Unary()
{
    if (currentToken->getTokenCode() == NOT)
    {
        currentToken = la->getNextToken();
    }
    Primary();
}

void SyntaxAnalyzer::Primary()
{
    switch (currentToken->getTokenCode())
    {
    case IDENT:
        currentToken = la->getNextToken();
        break;
    case NUMLIT:
        currentToken = la->getNextToken();
        break;
    case LPAREN:
        currentToken = la->getNextToken();
        Expression();
        if (currentToken->getTokenCode() == RPAREN)
        {
            currentToken = la->getNextToken();
        }
        else
        {
            error("Expected ')' after expression.");
        }
        break;
    case TRUESYM:
        currentToken = la->getNextToken();
    case FALSESYM:
        currentToken = la->getNextToken();
    default:
        error("Unexpected token in primary expression.");
        break;
    }
}
