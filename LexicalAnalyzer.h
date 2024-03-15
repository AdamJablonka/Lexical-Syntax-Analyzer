#ifndef LEXICALANALYZER_H
#define LEXICALANALYZER_H

#include <fstream>
#include <unordered_map>
#include <string>

#include "Token.h"
#include "TokenCodes.h"

using namespace std;

class LexicalAnalyzer
{
private:
    char currentChar;
    int currentCharIndex;
    ifstream *sourceCodeFile;
    string line;

    bool isEOI();
    void readNextLine();
    void getNextChar();

    int currentTokenPosition;

    std::unordered_map<std::string, TokenCodes> initializeLexemeTokenMap();

public:
    LexicalAnalyzer(ifstream *);
    Token *getNextToken();
    int getCurrentTokenPosition();
};

#endif