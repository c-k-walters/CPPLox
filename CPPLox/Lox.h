//
//  Lox.h
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//

#ifndef LOX_H
#define LOX_H

#include <string>
#include <iostream>
#include "ErrorHandler.h"

using namespace std;

class Lox {
public:
	Lox();

	void runFile(string path);
	void runPrompt();

	int exitCode();

private:
	ErrorHandler errorHandler;

	void run(string source);
};

#endif
