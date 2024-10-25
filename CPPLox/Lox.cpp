//
//  Lox.cpp
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//

#include "Lox.h"
#include "Token.h"
#include "Scanner.h"

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

Lox::Lox() {
	errorHandler = ErrorHandler();
}

void Lox::runFile(string path) {
	ifstream program(path);
	string bytes;
	string line;

	if (!program.is_open()) {
		cerr << "Could not open file" << endl;
	}

	while (getline(program, line)) {
		bytes.append(line);
		bytes.append("\n");
	}
	program.close();

	run(bytes);

}

void Lox::runPrompt() {
	while(true) {
		cout << "> ";
		string line;

		getline(cin, line);
		if (line.empty()) {
			break;
		}

		run(line);
		errorHandler.hadError = false;
	}
}

void Lox::run(string source) {
	Scanner scanner(source, errorHandler);
	vector<Token> tokens = scanner.scanTokens();

	for (auto t : tokens) {
		cout << t.toString() << endl;
	}
}

int Lox::exitCode() {
	return errorHandler.hadError ? 65 : 0;
}
