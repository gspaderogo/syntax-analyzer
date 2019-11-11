/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan
	CPSC 323 Compilers and Languages
	Syntax Analyzer
	November 5, 2019
*/

#include "syntaxAnalyzer.h"
#include "lexicalAnalyzer.h"
using namespace std;


int	analyzer(vector<tuple<string, string>> list)
{
	ifstream			inFile;
	ofstream			outFile;
	stack<string>		tableStack;
	int					iterator = 0;
	int					numStatements = 1;
	int					row;
	int					col;
	string				currentTop;
	string				currentToken;
	string				currentLexeme;
	string				dummy;


	string				parserTable[9][12] = { {"-1",  "id",   "=",   "+",   "-",   "*",  "/",   "(",   ")",   "$",   ";", "conditional" },
											   {"S",    "1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1"},
											   {"A",    "2",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1"},
											   {"E",    "3",  "-1",  "-1",  "-1",  "-1",  "-1",  "3",  "-1",  "-1",   "-1",  "-1"},
											   {"E'",   "-1",  "-1",  "4",   "5",  "-1",  "-1",  "-1",   "e",   "e",  "e",  "-1"},
											   {"T",    "6",  "-1",  "-1",  "-1",  "-1",  "-1",   "6",  "-1",  "-1",  "-1",  "-1"},
											   {"T'",   "2",  "-1",   "e",  "e",    "7",   "8",   "-1",   "e",   "e",  "e",  "-1"},
											   {"F",    "10",  "-1",  "-1",  "-1",  "-1",  "-1",  "9",  "-1",  "-1",  "-1",   "-1"},
											   {"C",	"-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1", "12"} };

	for (int i = 0; i < list.size(); i++) {
		if (get<0>(list.at(i)) == "EOS") {
			numStatements++;
		}
	}

	for (int i = 0; i < numStatements; i++) {
		// push "$" and starting "S" symbol
		tableStack.push("$");
		tableStack.push("S");

		// add "$" to the end of stringCheck 
		list.at(list.size() - 1) = make_tuple("EOS", "$");


		while (!tableStack.empty())
		{
			currentTop = tableStack.top();
			currentToken = get<0>(list.at(iterator));
			currentLexeme = get<1>(list.at(iterator));

			if (currentToken == "CONDITIONAL")
			{
				currentToken = "conditional";
				currentLexeme = "conditional";
			}

			if (currentToken == "IDENTIFIER")
			{
				currentToken = "id";
				currentLexeme = "id";
			}

			// terminal at top of stack
			if (isTerminal(currentTop))
			{
				if (currentTop == currentLexeme)
				{
					cout << "Token: " << currentToken << "\t\tLexeme: " << get<1>(list.at(iterator)) << endl << endl;
					tableStack.pop();
					++iterator;
				}
			}

			// non-terminal, so check which rule to use and push to stack 
			else
			{
				row = getRow(currentTop);
				col = getCol(currentLexeme);

				if (row == -1) {
					cout << "Error: unexpected nonterminal" << endl;
				}
				if (row == -1) {
					cout << "Error: unexpected terminal" << endl;
				}

				tableStack.pop();
				printRule(parserTable[row][col]);

				if (parserTable[row][col] == "1")
				{
					tableStack.push("A");
				}
				else if (parserTable[row][col] == "2")
				{
					tableStack.push(";");
					tableStack.push("E");
					tableStack.push("=");
					tableStack.push("id");
				}
				else if (parserTable[row][col] == "3")
				{
					tableStack.push("E'");
					tableStack.push("T");
				}
				else if (parserTable[row][col] == "4")
				{
					tableStack.push("E'");
					tableStack.push("T");
					tableStack.push("+");
				}
				else if (parserTable[row][col] == "5")
				{
					tableStack.push("E'");
					tableStack.push("T");
					tableStack.push("-");
				}
				else if (parserTable[row][col] == "6")
				{
					tableStack.push("T'");
					tableStack.push("F");
				}
				else if (parserTable[row][col] == "7")
				{
					tableStack.push("T'");
					tableStack.push("F");
					tableStack.push("*");
				}
				else if (parserTable[row][col] == "8")
				{
					tableStack.push("T'");
					tableStack.push("F");
					tableStack.push("/");
				}
				else if (parserTable[row][col] == "9")
				{
					tableStack.push(")");
					tableStack.push("E");
					tableStack.push("(");
				}
				else if (parserTable[row][col] == "10")
				{
					tableStack.push("id");
				}
				
				else if (parserTable[row][col] == "11")
				{
					tableStack.push("C");
				}
				else if (parserTable[row][col] == "12")
				{
					tableStack.push(")");
					tableStack.push("E");
					tableStack.push("(");
					tableStack.push("conditional");
				}
				
			}
		}
		cout << "\n---------- END OF STATEMENT ----------\n" << endl;
	}
	return 0;
}

//Displays which production rule is used
void printRule(string ruleNum)
{
	if (ruleNum == "1")
	{
		// S -> A
		cout << "<Statement>\t->\t<Assignment>" << endl;
	}
	else if (ruleNum == "2")
	{
		//A -> id = E
		cout << "<Assignment>\t->\t<id> = <Expression>;" << endl;
	}
	else if (ruleNum == "3")
	{
		//E -> TE'
		cout << "<Expression>\t->\t<Term> <Expression>'" << endl;
	}
	else if (ruleNum == "4")
	{
		//E' -> +TE'
		cout << "<Expression>\t->\t<Expression> + <Term>" << endl;
	}
	else if (ruleNum == "5")
	{
		//E' -> -TE'
		cout << "<Expression>\t->\t<Expression> - <Term>'" << endl;
	}
	else if (ruleNum == "6")
	{
		//T -> FT'
		cout << "<Term>\t\t->\t<Factor> <Term>'" << endl;
	}
	else if (ruleNum == "7")
	{
		//T' -> *FT
		cout << "<Term>\t\t->\t<Term> * <Factor>" << endl;
	}
	else if (ruleNum == "8")
	{
		//T' -> /FT'
		cout << "<Term>\t\t->\t<Term> / <Factor>" << endl;
	}
	else if (ruleNum == "9")
	{
		//F -> (E)
		cout << "<Factor>\t->\t( <Expression> )" << endl;
	}
	else if (ruleNum == "10")
	{
		//F -> id
		cout << "<Factor>\t->\tid" << endl;
	}
	else if (ruleNum == "11")
	{
		//S -> C
		cout << "<Statement>\t-><Conditional>";
	}
	else if (ruleNum == "12")
	{
		//C -> conditional ( E )
		cout << "<Conditional>\t->conditional( <Expression> )";
	}
	else if (ruleNum == "e")
	{
		//epsilon
		//Not necessary since we don't know need to point this out
		cout << "";
	}
	else
	{
		cout << "Error" << endl;
	}
}

////Determines if string is a terminal
bool isTerminal(string check)
{
	if (check == "id" || check == "=" || check == "+" || check == "-" || check == "*" || check == "/" || check == "(" || check == ")" || check == "$" || check == ";" || check == "conditional")
	{
		return true;
	}
	return false;
}


//Used in parsing table
int	getRow(string check)
{
	if (check == "S")
		return 1;
	else if (check == "A")
		return 2;
	else if (check == "E")
		return 3;
	else if (check == "E'")
		return 4;
	else if (check == "T")
		return 5;
	else if (check == "T'")
		return 6;
	else if (check == "F")
		return 7;
	else if (check == "C")
		return 8;
	else
		return -1;
}

//Used in parsing table
int	getCol(string check)
{
	if (check == "id")
		return 1;
	else if (check == "=")
		return 2;
	else if (check == "+")
		return 3;
	else if (check == "-")
		return 4;
	else if (check == "*")
		return 5;
	else if (check == "/")
		return 6;
	else if (check == "(")
		return 7;
	else if (check == ")")
		return 8;
	else if (check == "$")
		return 9;
	else if (check == ";")
		return 10;
	else if (check == "conditional")
		return 11;
	else
		return -1;
}