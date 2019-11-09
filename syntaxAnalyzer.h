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

int		getRow(string check);
int		getCol(string check);
bool	isTerminal(string check);
void	printRule(string ruleNum);

bool isIdentifier(const string& str);
int columnNum(char c);

#endif