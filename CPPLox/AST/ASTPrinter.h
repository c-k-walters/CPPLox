//
//  ASTPrinter.cpp
//  CPPLox
//
//  Created by Charles Kelley on 10/29/24.
//
#ifndef AST_PRINTER_H
#define AST_PRINTER_H

#include "Expr.h"

#include <string>
#include <any>
#include <memory>
#include <sstream>
#include <cassert>

using namespace std;

struct ASTPrinter: ExprVisitor {
	string print(shared_ptr<Expr> expr) {
		return any_cast<string>(expr->accept(*this));
	  }

	any visitBinaryExpr(shared_ptr<Binary> expr) override {
	  return parenthesize(expr->op.lexeme,
						  expr->left, expr->right);
	}

	any visitGroupingExpr(shared_ptr<Grouping> expr) override {
		return parenthesize("grouping", expr->expression);
	}

	any visitLiteralExpr(shared_ptr<Literal> expr) override {
		auto& value_type = expr->value.type();

		if (value_type == typeid(nullptr)) {
		  return "nil";
		} else if (value_type == typeid(string)) {
		  return any_cast<string>(expr->value);
		} else if (value_type == typeid(double)) {
		  return to_string(any_cast<double>(expr->value));
		} else if (value_type == typeid(bool)) {
		  return any_cast<bool>(expr->value) ? "true" : "false";
		}

		return "Error in visitLiteralExpr: literal type not recognized.";
	}

	any visitUnaryExpr(shared_ptr<Unary> expr) override {
		return parenthesize(expr->op.lexeme, expr->right);
	}

private:
	template <class... E>
	string parenthesize(string_view name, E... expr)
	{
		assert((... && is_same_v<E, shared_ptr<Expr>>));

		ostringstream builder;

		builder << "(" << name;
		((builder << " " << print(expr)), ...);
		builder << ")";

		return builder.str();
	}
};

#endif


