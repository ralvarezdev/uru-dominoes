#include <iostream>
#include <string>

using std::cout;
using std::string;

// --- Function Prototypes
void clearAndPrint(string *title);

// Function to Clear Terminal and then Print a Message
void clearAndPrint(string *title)
{
#ifdef _WIN32 // For Windows
  system("cls");
#else // For Linux
  system("clear");
#endif
  cout << *title;
}