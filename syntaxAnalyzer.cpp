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
	ofstream 			outFile("output.txt");
	stack<string>		tableStack;
	int					iterator = 0;
	int					numStatements = 1;
	int					row;
	int					col;
	string				currentTop;
	string				currentToken;
	string				currentLexeme;


	string				parserTable[9][12] = { {"-1",  "id",   "=",   "+",   "-",   "*",  "/",   "(",   ")",   "$",   ";", "select" },
											   {"S",    "1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "11"},
											   {"A",    "2",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1"},
											   {"E",    "3",  "-1",  "-1",  "-1",  "-1",  "-1",  "3",  "-1",  "-1",   "-1",  "-1"},
											   {"E'",   "-1",  "-1",  "4",   "5",  "-1",  "-1",  "-1",   "e",   "e",  "e",  "-1"},
											   {"T",    "6",  "-1",  "-1",  "-1",  "-1",  "-1",   "6",  "-1",  "-1",  "-1",  "-1"},
											   {"T'",   "2",  "-1",   "e",  "e",    "7",   "8",   "-1",   "e",   "e",  "e",  "-1"},
											   {"F",    "10",  "-1",  "-1",  "-1",  "-1",  "-1",  "9",  "-1",  "-1",  "-1",   "-1"},
											   {"SEL",	"-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1", "12"} };

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

			if (currentToken == "SELECT")
			{
				currentToken = "SELECT";
				currentLexeme = "select";
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
					outFile << "Token: " << currentToken << "\t\tLexeme: " << get<1>(list.at(iterator)) << endl << endl;
					tableStack.pop();
					++iterator;
				}

				// error message
				else 
				{
					outFile << "Error...unexpected terminal..." << endl;
					outFile << "Current terminal is: " << currentLexeme << endl; 
					outFile << "Expected terminal is: " << currentTop << endl; 
					outFile << "Exiting parser..." << endl << endl;
					outFile.close();

					cout << "Error occured please check 'output.txt for details" << endl;
					exit(EXIT_FAILURE);

				}
			}

			// non-terminal, so check which rule to use and push to stack 
			else
			{
				row = getRow(currentTop);
				col = getCol(currentLexeme);

				if (row == -1) {
					outFile << "Error: unexpected nonterminal" << endl;
				}
				if (col == -1) {
					outFile << "Error: unexpected terminal" << endl;
				}

				tableStack.pop();
				printRule(parserTable[row][col], outFile);

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
					tableStack.push("SEL");
				}
				else if (parserTable[row][col] == "12")
				{
					tableStack.push(")");
					tableStack.push("E");
					tableStack.push("(");
					tableStack.push("select");
				}
				
			}
		}
		outFile << "\n---------- END OF STATEMENT ----------\n" << endl;
	}

	outFile.close();
	return 0;
}

//Displays which production rule is used
void printRule(string ruleNum, ofstream &output)
{
	if (ruleNum == "1")
	{
		// S -> A
		output << "<Statement>\t->\t<Assignment>" << endl;
	}
	else if (ruleNum == "2")
	{
		//A -> id = E
		output << "<Assignment>\t->\t<id> = <Expression>;" << endl;
	}
	else if (ruleNum == "3")
	{
		//E -> TE'
		//cout << "<Expression>\t->\t<Term> <Expression>'" << endl;
		output << "<Expression>\t->\t<Term> <ExpressionPrime>" << endl;
	}
	else if (ruleNum == "4")
	{
		//E' -> +TE'
		//cout << "<Expression>\t->\t<Expression> + <Term>" << endl;
		output << "<Expression>\t->\t+ <Term> <ExpressionPrime>" << endl;
	}
	else if (ruleNum == "5")
	{
		//E' -> -TE'
		//cout << "<Expression>\t->\t<Expression> - <Term>'" << endl;
		output << "<Expression>\t->\t- <Term> <ExpressionPrime>" << endl;
	}
	else if (ruleNum == "6")
	{
		//T -> FT'
		//cout << "<Term>\t\t->\t<Factor> <Term>'" << endl;
		output << "<Term>\t\t->\t<Factor> <TermPrime>" << endl;
	}
	else if (ruleNum == "7")
	{
		//T' -> *FT'
		//cout << "<Term>\t\t->\t<Term> * <Factor>" << endl;
		output << "<Term>\t\t->\t* <Factor> <TermPrime>" << endl;
	}
	else if (ruleNum == "8")
	{
		//T' -> /FT'
		//cout << "<Term>\t\t->\t<Term> / <Factor>" << endl;
		output << "<Term>\t\t->\t/ <Factor> <TermPrime>" << endl;
	}
	else if (ruleNum == "9")
	{
		//F -> (E)
		output << "<Factor>\t->\t( <Expression> )" << endl;
	}
	else if (ruleNum == "10")
	{
		//F -> id
		output << "<Factor>\t->\tid" << endl;
	}
	else if (ruleNum == "11")
	{
		//S -> SEL
		output << "<Statement>\t->\t<Select>" << endl;
	}
	else if (ruleNum == "12")
	{
		//SEL -> select ( E )
		output << "<Select>\t->\tselect( <Expression> )" << endl;
	}
	else if (ruleNum == "e")
	{
		//epsilon
		//Not necessary since we don't know need to point this out
		output << "";
	}
	/*else // error message and exit program
	{
		output << "Unexpected Error...Incorrect rule usage" << endl;
		output << "Exiting parser..." << endl;
		output.close();

		cout << "Error occured please check 'output.txt for details" << endl;
		exit(EXIT_FAILURE);
	}*/
}

////Determines if string is a terminal
bool isTerminal(string check)
{
	if (check == "id" || check == "=" || check == "+" || check == "-" || check == "*" || check == "/" || check == "(" || check == ")" || check == "$" || check == ";" || check == "select")
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
	else if (check == "SEL")
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
	else if (check == "select")
		return 11;
	else
		return -1;
}