#include <string>

using std::string;

#ifndef INPUT_H
#define INPUT_H

// Functions
string lowercaseString(string input);
bool booleanQuestion(string question);
int intQuestion(string question, int startRange, int endRange);
string stringQuestion(string message);
void enterCharToContinue(string message, int charRequested);
void pressEnterToCont();

#endif