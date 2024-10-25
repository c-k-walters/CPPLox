//
//  ErrorHandler.h
//  CPPLox
//
//  Created by Charles Kelley on 10/24/24.
//

#ifndef ERROR_HANDLER_H
#define ERROR_HANDLER_H

#include <iostream>
#include <string>

using namespace std;

class ErrorHandler {
public:
	bool hadError = false;

	void error(int line, string error) {
		report(line, "", error);
	}

	void report(int line, string where, string message) {
		cerr << "[line " << line << "] Error" + where << ": " << message << endl;
		hadError = true;
	}
};

#endif
