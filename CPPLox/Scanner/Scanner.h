//
//  Scanner.h
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//

#ifndef SCANNER_H
#define SCANNER_H

#include <vector>
#include <any>
#include <map>
#include <iostream>
#include "Token.h"
#include "ErrorHandler.h"

using namespace std;

class Scanner {
public:
	Scanner(string source, ErrorHandler& handler);

	vector<Token> scanTokens();


private:
	string source;
	ErrorHandler& errorHandler;
	vector<Token> tokens;

	int start = 0;
	int current = 0;
	int line = 1;

	bool isAtEnd() {
		return current >= source.length();
	}

	void scanToken();
	char advance();
	void addToken(TokenType type);
	void addToken(TokenType type, any literal);
	bool match(char expected);
	char peek();
	char peekNext();
	void scanString();
	void scanDigit();
	void scanIdentifier();
	bool isDigit(char c);
	bool isAlpha(char c);
	bool isAlphaNumeric(char c);

	map<string, TokenType> reserved {
		{ "and",    AND },
		{ "class",  CLASS },
		{ "else",   ELSE },
		{ "false",  FALSE },
		{ "for",    FOR },
		{ "fun",    FUN },
		{ "if",     IF },
		{ "nil",    NIL },
		{ "or",     OR },
		{ "print",  PRINT },
		{ "return", RETURN },
		{ "super",  SUPER },
		{ "this",   THIS },
		{ "true",   TRUE },
		{ "var",    VAR },
		{ "while",  WHILE }
	};
};

#endif
