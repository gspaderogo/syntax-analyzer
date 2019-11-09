/*
	Johanna Nguyen
	CPSC 323 Compilers and Languages
	Lexical Analyzer
	September 19, 2019
*/

#include "LexicalAnalyzer.h"

int main()
{
	ifstream inFile;
	string userInput;
	ofstream outFile("output.txt");		
	
	cout << "------------------------------------------------------------\n"
		<< "LEXICAL ANALYZER by Johanna Nguyen\n"
		<< "------------------------------------------------------------\n"
		<< "Enter a text file location and name.\n"
		<< "Remember to use double slashes!\n\n"
		<< "Example: C:\\\\Users\\\\Desktop\\\\input.txt\n"
		<< "------------------------------------------------------------\n"
		<< "Your file name: ";

	cin >> userInput;				//Saves file destination as string

	inFile.open(userInput);			//Opens file by passing string									

	if (inFile.is_open())
	{
		outFile << left << setw(40) << "TOKEN" << "LEXEME" << endl;

		while (!inFile.eof())
		{
			lexer(inFile, outFile);
		}

		cout << "------------------------------------------------------------\n"
			<< "File has successfully been processed.\n"
			<< "Your results have been printed to \"output.txt.\"\n\n";
	}
	else
		cerr << "------------------------------------------------------------\n"
			<<	"\nError - file could not be opened.\n\n";

	inFile.close();
	outFile.close();
	
	system("Pause");
	return 0;
}

