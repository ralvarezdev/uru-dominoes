#include"input.h"
#include<iostream>
#include<string>

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
    bool answer, wrongInput;
    int x;
    string input;
    
    do{
        cout<<"\n\t"<<question<<" [Y/n] ";
        cin>>input;

        input = lowercaseString(input);

        if(input=="y"||input=="yes"){
            answer = true;
            break;
        }else if(input=="n"||input=="no"){
            break;
        }else{
            wrongInput = true;
        }

        if(wrongInput==true){
            cout<<"\t- Wrong Input: Type 'y', 'yes' or 'n', 'no'"<<"\n";
        }
    }while(wrongInput==true);

    return answer;
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
    bool wrongInput;
    string input;

    do{
        wrongInput = false;

        cout<<"\n\t"<<message<<": ";
        cin>>input;

        if(input!=charRequested){
            wrongInput=true;
        }
    }while(wrongInput==true);
}