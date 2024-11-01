//
//  main.cpp
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//

#include "Lox.h"
#include "GenerateAST.h"
#include "Expr.h"
#include "Parser.h"

#include <string>
#include <vector>

using namespace std;

vector<string> loxGrammer = {
	"Binary : Expr left, Token op, Expr right",
	"Grouping : Expr expression",
	"Literal : any value",
	"Unary : Token op, Expr right"
};

int main(int argc, const char * argv[]) {
	Lox lox;

	if (argc > 2) {
		cout << "Usage: cpplox [script]" << endl;
		return 64;
	} else if (argc == 2) {
		lox.runFile(argv[1]);
	} else {
		lox.runPrompt();
	}
	return lox.exitCode();

//	GenerateAST generator;
//	generator.defineAST(argv[1], "Expr", loxGrammer);
}
