//
//  Scanner.cpp
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//

#include "Scanner.h"

Scanner::Scanner(string source, ErrorHandler& handler) : errorHandler(handler) {
	this->source = source;
	this->tokens = vector<Token>();
}

vector<Token> Scanner::scanTokens() {
	while (!isAtEnd()) {
		start = current;
		scanToken();
	}

	tokens.push_back(Token(END_OF_FILE, "", NULL, line));
	return tokens;
}

void Scanner::scanToken() {
	char c = advance();

	switch (c) {
		case '(': addToken(LEFT_PAREN); break;
		case ')': addToken(RIGHT_PAREN); break;
		case '{': addToken(LEFT_BRACE); break;
		case '}': addToken(RIGHT_BRACE); break;
		case ',': addToken(COMMA); break;
		case '.': addToken(DOT); break;
		case '-': addToken(MINUS); break;
		case '+': addToken(PLUS); break;
		case ';': addToken(SEMICOLON); break;
		case '*': addToken(STAR); break;
		case '!':
			addToken(match('=') ? BANG_EQUAL : BANG);
			break;
		case '=':
			addToken(match('=') ? EQUAL_EQUAL : EQUAL);
			break;
		case '<':
			addToken(match('=') ? LESS_EQUAL : LESS);
			break;
		case '>':
			addToken(match('=') ? GREATER_EQUAL : GREATER);
			break;
		case '/':
			if (match('/')) {
				while (peek() != '\n' && !isAtEnd()) { advance(); }
			}
			else
				addToken(SLASH);
			break;
		case ' ':
		case '\r':
		case '\t':
			// ignore whitespace
			break;
		case '\n':
			line++;
			break;
		case '\"':
			scanString();
			break;

		default:
			if (isDigit(c)) {
				scanDigit();
			} else if (isAlpha(c)) {
				scanIdentifier();
			} else {
				errorHandler.error(line, "Unexpected character.");
			}
			break;
	}
}

// after advance, current will be at the next position. We have consumed the char in front.
char Scanner::advance() {
	// we know that we should not be at the end but we check anyway
	if (current >= source.length()) {
		return EOF;
	}
	return source[current++];
}

void Scanner::addToken(TokenType type) {
	addToken(type, NULL);
}

void Scanner::addToken(TokenType type, any literal) {
	string text = source.substr(start, current-start);
	tokens.push_back(Token(type, text, literal, line));
}

bool Scanner::match(char expected) {
	if (peek() != expected) {
		return false;
	}
	
	current++;
	return true;
}

char Scanner::peek() {
	if (isAtEnd())
		return '\0';

	return source[current];
}

char Scanner::peekNext() {
	if (current + 1 >= source.length())
		return '\0';

	return source[current+1];
}

void Scanner::scanString() {
	while (peek() != '"' && !isAtEnd()) {
		if (peek() == '\n')
			line++;

		advance();
	}

	if (isAtEnd())
		errorHandler.error(line, "Unterminated string");

	// The closing "
	advance();

	addToken(STRING, source.substr(start+1, current-start-2));
}

void Scanner::scanDigit() {
	// 1234.1234
	while (isdigit(peek()) && !isAtEnd())
		advance();

	if (peek() == '.' && isDigit(peekNext())) {
		// consume the .
		advance();

		while (isDigit(peek()))
			advance();
	}

	addToken(NUMBER, stod(source.substr(start, current-start)));
}

void Scanner::scanIdentifier() {
	while (isAlphaNumeric(peek()))
		advance();

	string text = source.substr(start, current-start);
	TokenType type = reserved[text];

	if (type == NULL)
		type = IDENTIFIER;

	addToken(type);


}

bool Scanner::isDigit(char c) {
	return c >= '0' && c <= '9';
}

bool Scanner::isAlpha(char c) {
	return ('a' <= c && c <= 'z')
	|| ('A' <= c && c <= 'Z')
	|| c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
	return isAlpha(c) || isDigit(c);
}
