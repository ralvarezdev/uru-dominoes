#include"cards.h"
#include<iostream>
#include<random>
#include<tuple>
#include<vector>

using std::random_device;
using std::uniform_int_distribution;
using std::cout;
using std::tuple;
using std::vector;


vector<int> shuffleCards(int numberCards, int players){
    random_device rd; // Random Number Generator
    uniform_int_distribution<int> dist(0,numberCards-1);

    bool exist;
    int x, y, n, random, doubles;
    vector<int> shuffledCards;
    vector<bool> generatedCards;

    // The number of cards divided by the number of players has a remainder
    x = numberCards%players;
    
    if(x!=0){
        cout<<"\n\t- Code Error: Cannot equally share cards";
    }

    for(x=0;x<numberCards;x++){
        generatedCards.push_back(false);
    }

    for(n=0;n<numberCards;n++){
        exist = false;

        while(exist==false){
            random = dist(rd);

            if(generatedCards[random]==false){
                generatedCards[random] = true;
                exist = true;
                break;
            }
        }
        shuffledCards.push_back(random);
    }
    return shuffledCards;
}