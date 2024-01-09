#include "input.h"
#include <iostream>
#include <string>
#include <cstdio>

using std::cin;
using std::cout;
using std::getline;
using std::stoi;
using std::string;

// --- Function Prototypes
string lowercaseString(string input);
bool booleanQuestion(string question);
string stringQuestion(string message);
void enterCharToContinue(string message, int charRequested);
void pressEnterToCont();

// Function to Get a String as Lowercase
string lowercaseString(string input)
{
    for (int x = 0; x < input.length(); x++)
        input[x] = tolower(input[x]);
    return input;
}

// Function to Ask Yes/No Questions
bool booleanQuestion(string question)
{
    char c;
    string input;

    while (true)
    {
        cout << "\n\t" << question << " [y/N] ";
        cin >> input;

        c = tolower(input[0]);
        if (c == 'y')
            return true;
        else if (c == 'n')
            return false;
        else
            cout << "\t- Wrong Input: Type 'y', 'Y' or 'n', 'N'\n";
    }
}

// Function to Ask for an Integer
int intQuestion(string question, int startRange, int endRange)
{
    bool wrongInput = false;
    int answer;
    string input;

    while (true)
    {
        if (wrongInput)
            cout << "\t- Wrong Input: It must be a Number between " << startRange << " and " << endRange << "\n";
        wrongInput = false;

        cout << "\n\t" << question << " [" << startRange << "-" << endRange << "] ";
        cin >> input;

        try
        {
            answer = stoi(input);

            if (answer < startRange || answer > endRange)
                throw(-1);
            return answer;
        }
        catch (...)
        {
            wrongInput = true;
        }
    }
}

// Function to Get a String from the User
string stringQuestion(string message)
{
    string input;

    cout << "\n\t" << message << ": ";
    cin >> input;

    return input;
}

// Function to Continue the Program Workflow if the User Types a Specific Character
void enterCharToContinue(string message, int charRequested)
{
    string input;

    while (true)
    {
        cout << "\n"
             << message << ": ";
        cin >> input;

        if (input[0] == charRequested)
            break;
    }
}

// Function to Stop the Program Flow while the User doesn't press the ENTER key
void pressEnterToCont()
{
    string temp;

    cout << "\n\nPress ENTER to Continue: ";
    getline(cin, temp);
}