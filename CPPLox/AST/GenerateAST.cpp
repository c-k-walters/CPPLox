//
//  GenerateAST.cpp
//  CPPLox
//
//  Created by Charles Kelley on 10/25/24.
//

#include "GenerateAST.h"

#include <fstream>

#include "String+Split.h"

void GenerateAST::defineAST(string outputDir, string baseName, vector<string> types) {
	string path = outputDir + "/" + baseName + ".h";
	ofstream outputFile(path);

	if (!outputFile.is_open())
		errorHandler.error(0, "Error opening output file for GenerateAST::defineAST");

	// if def
	outputFile << "#ifndef EXPR_H" << endl;
	outputFile << "#define EXPR_H" << endl << endl;

	outputFile << "#include \"Token.h\"" << endl << endl;

	outputFile << "#include <any>" << endl;
	outputFile << "#include <memory>" << endl;
	outputFile << "#include <utility>" << endl;
	outputFile << endl;

	outputFile << "using namespace std;" << endl << endl;

	// Expr Visitor
	defineVisitor(outputFile, baseName, types);

	// base class
	outputFile << "struct " << baseName << " {" << endl;
	outputFile << "\tvirtual any accept(ExprVisitor& visitor) = 0;" << endl;
	outputFile << "};" << endl << endl;

	// expr classes
	for (auto type : types) {
		vector<string_view> splitType = split(type, " : ");
		defineType(outputFile, baseName, splitType[0], splitType[1]);
	}

	// end if
	outputFile << "#endif" << endl;
	outputFile.close();
}

void GenerateAST::defineVisitor(ofstream& outputFile, string baseName, vector<string> types) {
	vector<string_view> typeNames;

	for (auto type : types) {
		outputFile << "struct " << split(type, " : ")[0] << ";" << endl;
	}
	outputFile << endl;

	outputFile << "struct ExprVisitor {" << endl;

	for (auto type : types) {
		string_view name = split(type, " : ")[0];
		outputFile << "\tvirtual any visit" << name << "Expr(shared_ptr<" << name << "> expr) = 0;" << endl;
	}

	outputFile << "\tvirtual ~ExprVisitor() = default;" << endl;
	outputFile << "};" << endl << endl;
}

void GenerateAST::defineType(ofstream& outputFile, string baseName, string_view typeName, string_view fieldList) {
	outputFile << "struct " << typeName << ": " << baseName;
	outputFile << ", public enable_shared_from_this<" << typeName << "> {" << endl;

	// constructor
	vector<string_view> fields = split(fieldList, ", ");
	outputFile << "\t";
	outputFile << typeName << "(";
	for (auto field: fields) {
		vector<string_view> splitField = split(field, " ");

		if (splitField[0] == "Expr")
			outputFile << "shared_ptr<Expr> " << splitField[1];
		else
			outputFile << field;

		if (field != fields.back())
			outputFile << ", ";
	}
	outputFile << "): ";

	for (auto field : fields) {
		string_view fieldName = split(field, " ")[1];
		outputFile << fieldName << "{std::move(" << fieldName << ")}";
		if (field != fields.back())
			outputFile << ", ";
	}
	outputFile << " {}" << endl << endl;

	outputFile << "\tany accept(ExprVisitor& visitor)override {" << endl;
	outputFile << "\t\treturn visitor.visit" << typeName << "Expr(shared_from_this());" << endl;
	outputFile << "\t}" << endl << endl;

	for (auto field : fields) {
		vector<string_view> splitField = split(field, " ");
		outputFile << "\tconst ";
		if (splitField[0] == "Expr")
			outputFile << "shared_ptr<Expr>" << splitField[1];
		else
			outputFile << field;
		outputFile << ";" << endl;
	}


	outputFile << "};\n\n";
}

//struct ASTPrinter: ExprVisitor {
//	string print(Expr expr) {
//		expr.accept(this)
//	}
//
//	string print(shared_ptr<Expr> expr) {
//		return any_cast<string>(expr->accept(*this));
//	  }
//
//	any visitBinaryExpr(shared_ptr<Binary> expr) {
//		return "";
//	}
//};
