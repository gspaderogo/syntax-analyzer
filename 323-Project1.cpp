// ============================================================================
// File: 323-Project1.cpp
// Date: 9/10/2019
// Programmer: Gilbert Paderogo
// Project: CPSC 323 Project 1 -- Lexical Analyzer
// Objective: Write a program that will read a file containing source code
//            and output tokens and lexemes in an output file/console
// ============================================================================

#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// types of inputs for state table
enum Inputs {LETTER = 1, 
             DIGIT,
             DECIMAL, 
             DOLLAR};


// types of Final States
enum States {START = 1, 
             IDENTIFIER, 
             INTEGER = 4, 
             REAL = 8};


// function prototypes
bool    isKeyWord(string key); 
bool    isOperator(string Op); 
bool    isSeperator(string seperator);
void    DFSM(string str, ofstream &outputFile);
int     charToCol(char ch);
bool    isPossibleSep(char seperator);



int     main()
{
    ifstream inFile;
    ofstream outFile;
    string   wordCheck;
    string   fileName;
    char     commentCheck;

    // open text file and check for errors
    cout << "Enter the name of the text file you want to open: ";
    cin >> fileName;

    inFile.open(fileName);
    if(inFile.fail())
    {
        cout << "There was an error opening the file...\n";
        return 0;
    }

    // open the output file
    outFile.open("output.txt");

    cout << "Writing output to output.txt...." << endl;

    outFile << endl << "TOKEN\t\t\t\t\t" << "LEXEME" << endl << endl;


    // start going through file to look for tokens and lexemes
    while(!inFile.eof())
    {
        // grab a word from source file
        inFile >> wordCheck;

        // check if it is a comment block; if so ignore it 
        if(wordCheck[0] == '!')
        {
            commentCheck = inFile.get();
            while(commentCheck != '!')
            {
                commentCheck = inFile.get();
            }
        }

        // check if it is a keyword
        else if(isKeyWord(wordCheck))
        {
            outFile << "KEYWORD\t\t\t\t\t" << wordCheck << endl;
        }
        // check if it is an operator
        else if(isOperator(wordCheck))
        {
            outFile << "OPERATOR\t\t\t\t" << wordCheck << endl;
        }
        // check if it is a seperator
        else if(isSeperator(wordCheck))
        {
            outFile << "SEPERATOR\t\t\t\t" << wordCheck << endl;
        }
        // check for identifiers, reals, and integers
        else
        {
            DFSM(wordCheck,outFile);
        }
    }

    cout << "Done."  << endl;

    // close input file
    inFile.close();

    // close output file
    outFile.close();

    return 0;
} // end of 'main'



// ==== isKeyWord =============================================================
// Function: isKeyWord
// Description: This function checks if the inputted string is a key word token
// IN: string key -- the word that is to be checked if it's a key word or not
// OUT: None
// Return: True if word is a key word otherwise, false
// ============================================================================
bool    isKeyWord(string key)
{
    // initialize the array with all possible keywords
    string keyWordArray[18] = {"int","float","bool","if","else","then","endif",
                               "while","whileend","do","doend","for","forend",
                               "input","output","and","or","function"};

    // compare the "key" to each key word
    for (int index = 0; index < 18; ++index)
    {
        if (key.compare(keyWordArray[index]) == 0) // "key" matches
        {
            return true;
        }
    }

    // "key" did not match with anything
    return false;

} // end of 'isKeyWord'


// ==== isOperator ============================================================
// Function: isOperator
// Description: This function checks if the inputted character is an operator
//              token
// IN: string Op -- the string that is to be checked if it's an operator or not
// OUT: None
// Return: True if string is an operator, otherwise false
// ============================================================================
bool    isOperator(string Op)
{
    // initialize array with all possible operators
    string    opArray[8] = {"*","+","-","=","/",">","<","%"};

    // compare "Op" with each operator and return true if there's a match
    for(int index = 0; index < 8; ++index)
    {
        if(opArray[index].compare(Op) == 0)
        {
            return true;
        }
    }

    // no match found
    return false;
} // end of 'isOperator'



// ==== isSeperator ===========================================================
// Function: isSeperator
// Description: This function checks if the received argument is a seperator
//              token
// IN: string seperator -- string to be checked if it's a token or not
// OUT: none
// Return: true if argument is a seperator, otherwise false
// ============================================================================
bool    isSeperator(string seperator)
{
    // initialize array with all possible seperators
    string    sepArray[11] = {"'","(",")","{","}","[","]",",",".",":",";"};

    // compare char argument with array to check for any matches and if so
    // return true
    for (int index = 0; index < 11; ++index)
    {
        if(seperator == sepArray[index])
        {
            return true;
        }
    }

    // no match found
    return false;

} // end of 'isSeperator'


// ==== DFSM ==================================================================
// Function: DFSM
// Description: This function utilizes a state transition table in order to
//              check for identifiers, real, and integer
// IN: string str -- string to be checked if it's a token or not
//     ofstream &outputFile -- the output file
// OUT: Ouput tokens and lexemes in the output file
// Return: None
// ============================================================================
void    DFSM(string str, ofstream &outputFile)
{
    int     stateTable[12][5] = { {0,            LETTER, DIGIT, DECIMAL, DOLLAR}, 
                                  {START,        2,      4,     10,      11    },
                                  {IDENTIFIER,   2,      2,     3,       2     },
                                  {3,            3,      3,     3,       3     },
                                  {INTEGER,      5,      4,     6,       5     },
                                  {5,            5,      5,     5,       5     },
                                  {6,            7,      8,     7,       7     },
                                  {7,            7,      7,     7,       7     },
                                  {REAL,         9,      8,     9,       9     },
                                  {9,            9,      9,     9,       9     },
                                  {10,           10,     10,    10,      10    },
                                  {11,           11,     11,    11,      11    } };

    char    currentChar;
    char    currentCol;
    int     currentState = START; // always start at state 1
    bool    possibleSep = false;
    int     sepIndex = 0;
    int     charIndex;

    // start going through the state table
    for( charIndex = 0; charIndex < str.length(); ++charIndex)
    {
        currentChar = str[charIndex];
        currentCol = charToCol(currentChar);
        currentState = stateTable[currentState][currentCol];

        // check if the string has a possible seperator 
        if(ispunct(currentChar))
        {
            if(isPossibleSep(currentChar))
            {
               possibleSep = true;
               sepIndex = charIndex;
            }
        }
    }

    // check for final states and output if necessary
    if (currentState == IDENTIFIER)
    {
        if(possibleSep)
        {
            // display any possible seperators
            if(sepIndex == charIndex - 1 )
            {
                outputFile << "IDENTIFIER\t\t\t\t" << str.substr(0, sepIndex) << endl; 
                outputFile << "SEPERATOR\t\t\t\t" << str[sepIndex] << endl;
            }
            
            else
            {
                outputFile << "IDENTIFIER\t\t\t\t" << str.substr(0, sepIndex) << endl;
                outputFile << "SEPERATOR\t\t\t\t" << str[sepIndex] << endl;
                outputFile << "IDENTIFIER\t\t\t\t" << str.substr(sepIndex + 1, charIndex) << endl; 

            }
        }

        else
        {
             outputFile << "IDENTIFIER\t\t\t\t" << str << endl;
        }
    }

    else if (currentState == INTEGER)
    {
        outputFile << "INTEGER\t\t\t\t" << str << endl;
    }

    else if (currentState == REAL)
    {
        outputFile << "REAL\t\t\t\t\t" << str << endl;
    }

    // went into other states that are not accepted 
    else
    {
        outputFile << "UKNOWN\t\t\t\t\t" << str << endl;
    }
    
} // end of 'DFSM'


// ==== charToCol =============================================================
// Function: charToCol
// Description: This function checks what kind of input the character argument
//              is
// IN: char ch -- the character to be checked 
// OUT: none
// Return: The type of input -- (LETTER, DIGIT, DECIMAL, or DOLLAR)
// ============================================================================
int     charToCol(char ch)
{
    if (isdigit(ch))
    {
        return DIGIT;
    }

    else if(isalpha(ch))
    {
        return LETTER;
    }

    else if(ch == '.')
    {
        return DECIMAL;
    }

    else // ch == '$'
    {
        return DOLLAR;
    }
} // end of 'charToCol'



// ==== isPossibleSep =============================================================
// Function: isPossibleSep
// Description: This function checks if the character from the string being used
//              in the state table is a possible seperator
// IN: char seperator -- character to be checked
// OUT: none
// Return: True if the char is a seperator otherwise false
// ============================================================================
bool    isPossibleSep(char seperator)
{
    // initialize array with all possible seperators
    char      posSepArray[10] = {'(',')','{','}','[',']',',','.',':',';'};

    // compare char argument with array to check for any matches and if so
    // return true
    for (int index = 0; index < 10; ++index)
    {
        if(seperator == posSepArray[index])
        {
            return true;
        }
    }

    // no match found
    return false;

} // end of 'isPossibleSep'