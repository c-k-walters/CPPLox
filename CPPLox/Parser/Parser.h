//
//  Parser.h
//  CPPLox
//
//  Created by Charles Kelley on 11/1/24.
//
#ifndef PARSER_H
#define PARSER_H

#include <vector>
#include <memory>
#include <cassert>
#include <stdexcept>

#include "Token.h"
#include "TokenType.h"
#include "Expr.h"

using namespace std;



class Parser {
public:
	Parser(vector<Token>& tokens, ErrorHandler& errorHandler) : tokens(tokens), errorHandler(errorHandler) {}

	struct ParseError: public std::runtime_error {
		using runtime_error::runtime_error;
	};

	shared_ptr<Expr> parse() {
		try {
			return expression();
		} catch (ParseError er) {
			return NULL;
		}
	}

private:
	vector<Token>& tokens;
	int current = 0;
	ErrorHandler errorHandler;

	shared_ptr<Expr> expression() {
		return equality();
	}

	shared_ptr<Expr> equality() {
		shared_ptr<Expr> expr = comparison();

		while (match(BANG_EQUAL, EQUAL_EQUAL)) {
			Token op = previous();
			shared_ptr<Expr> right = comparison();
			expr = make_shared<Binary>(expr, std::move(op), right);
		}

		return expr;
	}

	shared_ptr<Expr> comparison() {
		shared_ptr<Expr> expr = term();

		while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
			Token op = previous();
			shared_ptr<Expr> right = term();
			expr = make_shared<Binary>(expr, std::move(op), right);
		}

		return expr;
	}

	shared_ptr<Expr> term() {
		shared_ptr<Expr> expr = factor();

		while (match(MINUS, PLUS)) {
			Token op = previous();
			shared_ptr<Expr> right = factor();
			expr = make_shared<Binary>(expr, op, right);
		}

		return expr;
	}

	shared_ptr<Expr> factor() {
		shared_ptr<Expr> expr = unary();

		while (match(SLASH, STAR)) {
			Token op = previous();
			shared_ptr<Expr> right = unary();
			expr = make_shared<Binary>(expr, op, right);
		}

		return expr;
	}

	shared_ptr<Expr> unary() {
		if (match(BANG, MINUS)) {
			Token op = previous();
			shared_ptr<Expr> right = unary();
			return make_shared<Unary>(op, right);
		}
		return primary();
	}

	shared_ptr<Expr> primary() {
		if (match(FALSE))
			return make_shared<Literal>(false);
		if (match(TRUE))
			return make_shared<Literal>(true);
		if (match(NIL))
			return make_shared<Literal>(NULL);

		if (match(NUMBER, STRING)) {
			return make_shared<Literal>(previous().literal);
		}

		if (match(LEFT_PAREN)) {
			shared_ptr<Expr> expr = expression();
			// consume
			return make_shared<Grouping>(expr);
		}

		throw error(peek(), "Expect expression.");
	}

	template <class... T>
	bool match(T... types) {
		assert((... && is_same_v<T, TokenType>));
		if ((... || check(types))) {
			advance();
			return true;
		}

		return false;
	}

	bool check(TokenType type) {
		if (isAtEnd())
			return false;

		return peek().type == type;
	}

	Token advance() {
		if (!isAtEnd())
			current++;

		return previous();
	}

	bool isAtEnd() { return peek().type == END_OF_FILE; }

	Token peek() {
		return tokens[current];
	}

	Token previous() {
		return tokens[current - 1];
	}

	Token consume(TokenType type, string message) {
		if (check(type))
			return advance();

		throw error(peek(), message);
	}

	ParseError error(Token token, string message) {
		errorHandler.error(token, message);
		return ParseError("");
	}

	void sync() {
		advance();

		while (!isAtEnd()) {
			if (previous().type == SEMICOLON)
				return;

			switch (peek().type) {
				case CLASS:
				case FUN:
				case VAR:
				case FOR:
				case IF:
				case WHILE:
				case PRINT:
				case RETURN:
					return;
				default:
					break;
			}

			advance();
		}
	}
};

#endif
