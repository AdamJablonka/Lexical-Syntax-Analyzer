#include <iostream>
#include <fstream>

#include "LexicalAnalyzer.h"
#include "Token.h"
#include "TokenCodes.h"

using namespace std;

LexicalAnalyzer::LexicalAnalyzer(ifstream *scf)
{
    currentChar = '\n';
    currentCharIndex = 0;
    currentTokenPosition = 0;
    sourceCodeFile = scf;
    line = "\n";
}

bool LexicalAnalyzer::isEOI()
{
    return (sourceCodeFile->eof() && (currentCharIndex == (line.length() - 1)));
}

void LexicalAnalyzer::readNextLine()
{
    getline(*sourceCodeFile, line);
    line = line + "\n";
    cout << line;
}

void LexicalAnalyzer::getNextChar()
{
    if (currentCharIndex == (line.length() - 1))
    {
        if (sourceCodeFile->eof())
        {
            currentChar = EOF;
            return;
        }
        readNextLine();
        currentCharIndex = -1;
        currentTokenPosition = 0;
    }

    currentCharIndex++;
    currentChar = line.at(currentCharIndex);
}

std::unordered_map<std::string, TokenCodes> LexicalAnalyzer::initializeLexemeTokenMap()
{
    std::unordered_map<std::string, TokenCodes> lexemeTokenMap;

    lexemeTokenMap["+"] = PLUS;
    lexemeTokenMap["-"] = MINUS;
    lexemeTokenMap["*"] = TIMES;
    lexemeTokenMap["/"] = SLASH;
    lexemeTokenMap["%"] = MOD;
    lexemeTokenMap["("] = LPAREN;
    lexemeTokenMap[")"] = RPAREN;
    lexemeTokenMap["{"] = LBRACE;
    lexemeTokenMap["}"] = RBRACE;
    lexemeTokenMap[","] = COMMA;
    lexemeTokenMap[";"] = SEMICOLON;
    lexemeTokenMap["||"] = OR;
    lexemeTokenMap["&&"] = AND;
    lexemeTokenMap["="] = ASSIGN;
    lexemeTokenMap["=="] = EQL;
    lexemeTokenMap["!"] = NOT;
    lexemeTokenMap["!="] = NEQ;
    lexemeTokenMap["<"] = LSS;
    lexemeTokenMap["<="] = LEQ;
    lexemeTokenMap[">"] = GTR;
    lexemeTokenMap[">="] = GEQ;
    lexemeTokenMap["bool"] = BOOLSYM;
    lexemeTokenMap["do"] = DOSYM;
    lexemeTokenMap["else"] = ELSESYM;
    lexemeTokenMap["false"] = FALSESYM;
    lexemeTokenMap["float"] = FLOATSYM;
    lexemeTokenMap["for"] = FORSYM;
    lexemeTokenMap["if"] = IFSYM;
    lexemeTokenMap["int"] = INTSYM;
    lexemeTokenMap["printf"] = PRINTFSYM;
    lexemeTokenMap["return"] = RETURNSYM;
    lexemeTokenMap["scanf"] = SCANFSYM;
    lexemeTokenMap["true"] = TRUESYM;
    lexemeTokenMap["void"] = VOIDSYM;
    lexemeTokenMap["while"] = WHILESYM;
    lexemeTokenMap["identifier"] = IDENT;
    lexemeTokenMap["numeric literal"] = NUMLIT;
    lexemeTokenMap["end of input"] = EOI;
    lexemeTokenMap["not a lexeme"] = NAL;

    return lexemeTokenMap;
}

int LexicalAnalyzer::getCurrentTokenPosition()
{
    return currentTokenPosition;
}

Token *LexicalAnalyzer::getNextToken()
{
    std::unordered_map<std::string, TokenCodes> lexemeTokenMap = initializeLexemeTokenMap();

    while (isspace(currentChar))
    {
        getNextChar();
    }

    currentTokenPosition = currentCharIndex;

    if (isEOI())
    {

        return new Token(EOI, "");
    }

    if (isalpha(currentChar))
    {
        string lexeme = "";
        while (isalnum(currentChar) || currentChar == '_')
        {
            if (lexeme.empty())
            {
                currentTokenPosition = currentCharIndex;
            }
            lexeme += currentChar;
            getNextChar();
        }

        if (lexemeTokenMap.find(lexeme) != lexemeTokenMap.end())
        {

            return new Token(lexemeTokenMap[lexeme], lexeme);
        }
        else
        {
            return new Token(IDENT, lexeme);
        }
    }

    else if (isdigit(currentChar))
    {
        string lexeme = "";
        bool isFloat = false;

        if (currentChar == '0')
        {
            currentTokenPosition = currentCharIndex;
            lexeme += currentChar;
            getNextChar();
        }
        else
        {
            while (isdigit(currentChar) || currentChar == '.')
            {
                if (lexeme.empty())
                {
                    currentTokenPosition = currentCharIndex;
                }
                if (currentChar == '.')
                {
                    isFloat = true;
                }
                lexeme += currentChar;
                getNextChar();
            }
        }

        return new Token(NUMLIT, lexeme);
    }

    else
    {
        string lexeme(1, currentChar);
        // 5
        currentTokenPosition = currentCharIndex;
        getNextChar();

        switch (currentChar)
        {
        case '=':
            if (lexeme == "=")
            {
                lexeme += currentChar;
                getNextChar();
            }
            else if (lexeme == "!")
            {
                lexeme += currentChar;
                getNextChar();
            }
            else if (lexeme == "<")
            {
                lexeme += currentChar;
                getNextChar();
            }
            else if (lexeme == ">")
            {
                lexeme += currentChar;
                getNextChar();
            }
            break;
        case '&':
            if (lexeme == "&")
            {
                lexeme += currentChar;
                getNextChar();
            }
            break;
        case '|':
            if (lexeme == "|")
            {
                lexeme += currentChar;
                getNextChar();
            }
            break;
        }

        if (lexemeTokenMap.find(lexeme) != lexemeTokenMap.end())
        {

            return new Token(lexemeTokenMap[lexeme], lexeme);
        }
    }

    return new Token(NAL, "");
}
