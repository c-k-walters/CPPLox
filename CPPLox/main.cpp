//
//  main.cpp
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//

#include "Lox.h"
using namespace std;

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
}
