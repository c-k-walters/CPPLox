//
//  GenerateAST.cpp
//  CPPLox
//
//  Created by Charles Kelley on 10/25/24.
//
#ifndef GENERATE_AST_H
#define GENERATE_AST_H

#include "ErrorHandler.h"

#include <string>
#include <vector>
#include <fstream>

using namespace std;

class GenerateAST {
public:
	void defineAST(string outputDir, string baseName, vector<string> types);

	void defineVisitor(ofstream& outputFile, string baseName, vector<string> types);
	void defineType(ofstream& outputFile, string baseName, string_view typeName, string_view fieldList);

private:
	ErrorHandler errorHandler;
};

#endif
