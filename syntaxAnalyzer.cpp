/*
	Johanna Nguyen
	Gilbert Paderogo
	Richard Phan

	CPSC 323 Compilers and Languages
	Syntax Analyzer
	November 5, 2019
*/

#include "syntaxAnalyzer.h"
using namespace std;


int		main()
{
	ifstream			inFile;
	ofstream			outFile;
	stack<string>		tableStack;
	int					iterator = 0;
	int					row;
	int					col;
	string				stringCheck;
	string				currentTop;
	string				currentChar;


	string				parserTable[8][11] = { {"-1",  "id",   "=",   "+",   "-",   "*",  "/",   "(",   ")",   "$",   ";"},
											   {"S",    "1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1"},
											   {"A",    "2",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1",  "-1"},
											   {"E",    "3",  "-1",  "-1",  "-1",  "-1",  "-1",  "3",  "-1",  "-1",   "-1"},
											   {"E'",   "-1",  "-1",  "4",   "5",  "-1",  "-1",  "-1",   "e",   "e",  "e"},
											   {"T",    "6",  "-1",  "-1",  "-1",  "-1",  "-1",   "6",  "-1",  "-1",  "-1"},
											   {"T'",   "2",  "-1",   "e",  "e",  "7",   "8",    "-1",   "e",   "e",  "e"},
											   {"F",    "10",  "-1",  "-1",  "-1",  "-1",  "-1",  "9",  "-1",  "-1",  "-1"} };

	inFile.open("input2.txt");

	if (inFile.fail())
	{
		cout << "There was an error opening the file..." << endl;
		return 0;
	}


	while (!inFile.eof())
	{
		iterator = 0;

		// push "$" and starting "S" symbol
		tableStack.push("$");
		tableStack.push("S");

		// grab the test string 
		getline(inFile, stringCheck);


		// get rid of whitespace
		stringCheck.erase(remove(stringCheck.begin(), stringCheck.end(), ' '), stringCheck.end());

		// add "$" to the end of stringCheck 
		stringCheck.append("$");
		cout << stringCheck << endl;


		while (!tableStack.empty())
		{
			currentTop = tableStack.top();
			currentChar = stringCheck[iterator];

			if (isIdentifier(currentChar))
			{
				currentChar = "id";
			}

			// terminal at top of stack
			if (isTerminal(currentTop))
			{
				if (currentTop == currentChar)
				{
					cout << "MATCH FOUND:\t" << currentChar << endl << endl;
					tableStack.pop();
					++iterator;
				}
			}

			// non-terminal, so check which rule to use and push to stack 
			else
			{
				row = getRow(currentTop);
				col = getCol(currentChar);

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
			}

		}
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
	if (check == "id" || check == "=" || check == "+" || check == "-" || check == "*" || check == "/" || check == "(" || check == ")" || check == "$" || check == ";")
	{
		return true;
	}
	else
	{
		return false;
	}
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
	else
		return -1;
}


//Returns true if identifier
bool isIdentifier(const string& str)
{												//DFSM TABLE
	const int STATES = 4, INPUTS = 5;			//		l	d	$	.	other
	int dfsmTable[STATES][INPUTS] =				//0	|	0	0	0	0	0	(dead state)
	{ { 0, 0, 0, 0, 0 },						//1	|	2	0	0	0	0
	{ 2, 0, 0, 0, 0 },							//2	|	2	2	3	0	0
	{ 2, 2, 3, 0, 0 },							//3	|	0	0	0	0	0
	{ 0, 0, 0, 0, 0 } };						//Final states: 2, 3

	int state = 1;								//Initial state: 1
	int length = str.length();
	for (int i = 0; i < length; ++i)			//Traverses through lexeme
	{
		int input = columnNum(str[i]);			//Finds column number based on each character in the lexeme
		state = dfsmTable[state][input];		//Determines new state based on input
	}
	if (state == 2 || state == 3)
		return true;

	return false;
}

//Helper function for isIdentifier
int columnNum(char c)
{
	if (isalpha(c))			return 0;			//column 0 = letter
	else if (isdigit(c))	return 1;			//column 1 = digit
	else if (c == '$')		return 2;			//column 2 = $
	else if (c == '.')		return 3;			//column 3 = .
	return 4;									//column 4 = all other characters
}





