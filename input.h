#include<string>

using std::string;


#ifndef INPUT_H
#define INPUT_H

string lowercaseString(string input);
bool booleanQuestion(string question);
int intQuestion(string question, int startRange, int endRange);
string stringQuestion(string message);
void enterCharToContinue(string message, string charRequested);

#endif