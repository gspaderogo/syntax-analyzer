/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan

	CPSC 323 Compilers and Languages
	Syntax Analyzer
	November 5, 2019
*/

#ifndef SyntaxAnalyzer_hpp
#define SyntaxAnalyzer_hpp

#include <iostream>
#include <fstream>
#include <iomanip>
#include <tuple>
#include <string>
#include <algorithm>
#include <stack>
using namespace std;

//Used in parsing table
int	getRow(string check);

//Used in parsing table
int	getCol(string check);

//Determines if string is a terminal
bool isTerminal(string check);

//Displays which production rule is used
void printRule(string ruleNum);

//Determines if string is an identifier
bool isIdentifier(const string& str);

//Helper function for isIdentifier
int columnNum(char c);

#endif