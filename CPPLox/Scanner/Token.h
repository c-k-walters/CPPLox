//
//  Token.h
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//
#ifndef TOKEN_H
#define TOKEN_H

#include <any>
#include <string>
#include "TokenType.h"

using namespace std;

class Token {
public:
	Token(TokenType type, string lexeme, any literal, int line) {
		this->type = type;
		this->lexeme = lexeme;
		this->literal = literal;
		this->line = line;
	}

	string tokenString(TokenType type) {
	  static const std::string strings[] = {
		"LEFT_PAREN", "RIGHT_PAREN", "LEFT_BRACE", "RIGHT_BRACE",
		"COMMA", "DOT", "MINUS", "PLUS", "SEMICOLON", "SLASH", "STAR",
		"BANG", "BANG_EQUAL",
		"EQUAL", "EQUAL_EQUAL",
		"GREATER", "GREATER_EQUAL",
		"LESS", "LESS_EQUAL",
		"IDENTIFIER", "STRING", "NUMBER",
		"AND", "CLASS", "ELSE", "FALSE", "FUN", "FOR", "IF", "NIL", "OR",
		"PRINT", "RETURN", "SUPER", "THIS", "TRUE", "VAR", "WHILE",
		"END_OF_FILE"
	  };

	  return strings[static_cast<int>(type)];
	}

	string toString() {
		string literal_text;

		switch (type) {
		  case (IDENTIFIER):
			literal_text = lexeme;
			break;
		  case (STRING):
			literal_text = any_cast<string>(literal);
			break;
		  case (NUMBER):
			literal_text = to_string(any_cast<double>(literal));
			break;
		  case (TRUE):
			literal_text = "true";
			break;
		  case (FALSE):
			literal_text = "false";
			break;
		  default:
			literal_text = "nil";
		}

		return tokenString(type) + " " + lexeme + " " + literal_text;
	  }



private:
	TokenType type;
	string lexeme;
	any literal;
	int line;
};

#endif
