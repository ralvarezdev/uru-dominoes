#include"input.h"
#include<iostream>
#include<string>
#include<cstdio>

using std::cout; using std::cin;
using std::string;
using std::stoi;


string lowercaseString(string input){
    int x;
    
    for(x=0;x<input.length();x++){
        input[x] = tolower(input[x]);
    }
    return input;
}

bool booleanQuestion(string question){
    char c;
    int x;
    string input;
    
    do{
        cout<<"\n\t"<<question<<" [y/N] ";
        cin>>input;
        c = tolower(input[0]);

        if(c=='y'){
            return true;
        }else if(c=='n'){
            return false;
        }else{
            cout<<"\t- Wrong Input: Type 'y', 'Y' or 'n', 'N'"<<"\n";
        }
    }while(true);
}

int intQuestion(string question, int startRange, int endRange){
    bool wrongInput;
    int x, answer;
    string input;

    do{
        wrongInput = false;
        
        cout<<"\n\t"<<question<<" ["<<startRange<<"-"<<endRange<<"] ";
        cin>>input;

        try{
            answer = stoi(input);

            if(answer<startRange||answer>endRange){
                wrongInput = true;
            }
        }catch(std::invalid_argument){
            wrongInput = true;
        }

        if(wrongInput==true){
            cout<<"\t- Wrong Input: It must be a number between "<<startRange<<" and "<<endRange<<"\n";
        }
    }while(wrongInput==true);

    return answer;
}

string stringQuestion(string message){
    string input;

    cout<<"\n\t"<<message<<": ";
    cin>>input;

    return input;
}

void enterCharToContinue(string message, string charRequested){
    string input;

    do{
        cout<<"\n"<<message<<": ";
        cin>>input;

        if(input==charRequested){
            break;
        }
    }while(true);
}

void pressEnterToContinue(){
    cout<<"\n\nPress 'Enter' to Continue: ";
    cin.get();
}
