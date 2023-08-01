#include"cards.h"
#include"input.h"
#include<iostream>
#include<random>
#include<vector>
#include<chrono>
#include<thread>

using std::random_device; using std::uniform_int_distribution;
using std::cout;
using std::vector;

// Functions

void clearAndTitle(string& title){
    #ifdef _WIN32 // For Windows
        system("cls");
    #else // For Linux
        system("clear");
    #endif
    cout<<title;
}

void oddToEven(int& n){ // Sum 1 if the number is odd, in order to convert it to even
    if(n%2!=0){
        n++;
    }
}

vector<int> shuffleCards(int numberCards, int players){
    random_device rd; // Random Number Generator

    int x, n, random, number;
    vector<int> shuffledCards, cards;

    // The number of cards divided by the number of players has a remainder
    if(numberCards%players!=0){
        cout<<"\n\t- Code Error: Cannot equally share cards";
    }

    for(x=0;x<numberCards;x++){
        cards.push_back(x);
    }

    for(x=0;x<numberCards;x++){
        n = 1+x;
        uniform_int_distribution<int> dist(0,numberCards-n);
        random = dist(rd);
        number = cards[random];
        cards.erase(cards.begin() + random);
        shuffledCards.push_back(number);
    }
    return shuffledCards;
}

// Methods

void Board::boardSize(int x, int y){
    oddToEven(x);
    oddToEven(y);

    xSize = x;
    ySize = y;

    xCenter = (xSize/2); // We're counting position 0
    yCenter = (ySize/2);

    boardCellsOccupied.resize(ySize);
    boardCellsContent.resize(ySize);

    for(y=0;y<ySize;y++){
        boardCellsOccupied[y].resize(xSize);
        boardCellsContent[y].resize(xSize);
    }
}

void Board::printTitleBar(string& title, int numberPlayers){
    clearAndTitle(title);

    if(numberPlayers==3){
        cout<<"\n"<<players[0]<<" ("<<points[0]<<") vs "<<players[1]<<" ("<<points[1]<<") vs "<<players[2]<<" ("<<points[2]<<")\n";
    }else if(numberPlayers==4){
        cout<<"\nTeam "<<players[0]<<" ("<<points[0]<<") vs Team "<<players[1]<<" ("<<points[1]<<")\n";
    }
}

void Board::printBoard(){
    int x, y, cellContent;

    cout<<"\n";

    for(y=0;y<ySize;y++){
        for(x=0;x<xSize;x++){
            if(boardCellsOccupied[y][x]==false){
                cout<<"   ";
            }else{
                cellContent = boardCellsContent[y][x];

                if(cellContent<7){
                    cout<<"["<<cellContent<<"]";
                }else if(cellContent==7){
                    cout<<" | ";
                }else if(cellContent==8){
                    cout<<" - ";
                }
            }
        }
        cout<<"\n";
    }
}

void Board::changeCellContent(bool equalTiles, bool lieDown, int row, int col, int sideFree, int free, int sideConnected, int connected, int separator){
    int x, a, b, c, d, e, f, n;
    
    if(equalTiles==true){
        if(lieDown==false){
            a = free; b = 1; c = col ; d = 0; n = 8;
        }else{
            a = row; b = 0; c = free ; d = 1; n = 7;
        }
        boardCellsContent[a][c] = sideFree;
        boardCellsContent[a+b][c+d] = n;
        boardCellsContent[a+2*b][c+2*d] = sideFree;

        for(x=0;x<3;x++){
            boardCellsOccupied[a+b*x][c+d*x] = true;
        }

    }else{
        if(lieDown==true){
            a = row; b = free; c = row; d = connected; e = row; f = separator; n = 7;
        }else{
            a = free; b = col; c = connected; d = col; e = separator; f = col; n = 8;
        }
        boardCellsContent[a][b] = sideFree;
        boardCellsOccupied[a][b] = true;
        boardCellsContent[c][d] = sideConnected;
        boardCellsOccupied[c][d] = true;

        if(separator!=-1){
            boardCellsContent[e][f] = n;
            boardCellsOccupied[e][f] = true;
        }
    }   
}

void Board::addCard(Dominoes& domino){
    bool answer, equalTiles, lieDown;
    char cardPosition;
    int separator = -1;
    int x, y, row, col, cardPlayed, sideA, sideB, sideConnected, sideFree, freePosition, connectedPosition;  
    
    sideA = domino.sideA; sideB = domino.sideB;
    if(numberCardsPlayed!=0){
        if((cardUp==cardDown)||(sideA==cardUp&&sideB==cardDown)||(sideB==cardUp&&sideA==cardDown)){
            answer = booleanQuestion("Where do you want to put the card 'Yes' for Up/Left, 'No' for Down/Right?");

            if(answer==true){
                cardPosition = 'u';
            }else{
                cardPosition = 'd';
            }
        }else if(sideA==cardUp||sideB==cardUp){
            cardPosition = 'u';
        }else{
            cardPosition = 'd';
        }
    }else{
        upSegment = 1; downSegment = 1; upSegmentC = 0; downSegmentC = 0;
        col = xCenter; colLeft = col; colRight = col;
        row = yCenter; rowUp = row; rowDown = row;
        cardUp = sideA; cardDown = sideB;

        for(y=0;y<ySize;y++){
            for(x=0;x<xSize;x++){
                boardCellsOccupied[y][x] = false;
                boardCellsContent[y][x] = -1;
            }
        }
    }

    if(sideA==sideB){
        equalTiles = true;
        sideFree = sideA;
    }else{
        if(cardPosition=='u'){
            cardPlayed = cardUp;
        }else{
            cardPlayed = cardDown;
        }

        if(sideA==cardPlayed){
            sideConnected = sideA;
            sideFree = sideB;
        }else{
            sideConnected = sideB;
            sideFree = sideA;
        }
    }

    if(numberCardsPlayed==0){
        colLeft--;
        colRight++;

        if(equalTiles==false){
            colRight++;
            lieDown = true;
        }
    }else{
        if(cardPosition=='u'){
            checkUpPosition(equalTiles, row, col, lieDown);
            cardUp = sideFree;
        }else{
            checkDownPosition(equalTiles, row, col, lieDown);
            cardDown = sideFree;
        }   
    }

    if(equalTiles==true){
        if(lieDown==false){
            changeCellContent(true, false, row, col, sideFree, row-1);
        }else{
            changeCellContent(true, true, row, col, sideFree, col-1);
        }

        equalTilesUp = true;
        equalTilesDown = true;

        if(cardPosition=='u'){
            equalTilesDown = false;
        }else if(cardPosition=='d'){
            equalTilesUp = false;
        }
    }else{
        if(numberCardsPlayed==0){
            equalTilesUp = false;
            equalTilesDown = false;
            changeCellContent(false, true, row, col, sideA, xCenter, sideB, xCenter+1);
        }else{
            if(lieDown==false){
                freePosition = row;
            }else{
                freePosition = col;
            }

            if(cardPosition=='u'){
                if(upSegment<3){
                    x = 1;
                }else{
                    x = -1;
                }

                connectedPosition = freePosition+x;
                if(equalTilesUp==false){
                    separator = freePosition+x*2;
                }else{
                    equalTilesUp = false;
                }

            }else if(cardPosition=='d'){
                if(downSegment<3){
                    x = -1;
                }else{
                    x = 1;
                }

                connectedPosition = freePosition+x;
                if(equalTilesDown==false){
                    separator = freePosition+2*x;
                }else{
                    equalTilesDown = false;
                }
            }
            changeCellContent(false, lieDown, row, col, sideFree, freePosition, sideConnected, connectedPosition, separator);        
        }
    }
    numberCardsPlayed++;
}

void Board::checkUpPosition(bool equalTiles, int& row, int& col, bool& lieDown){
    int compare, x;

    while(true){
        row = rowUp;
        col = colLeft;
        upSegmentC++;

        if(upSegment==4){
            cout<<"\nError: Cannot add more dominoes in one of the sides. Please increase the size of the board on the cpp...";
            pressEnterToContinue();
        }
        switch(upSegment){
            case 1:
                compare = col;
                break;
            case 2:
                compare = row;
                break;
            case 3:
                compare = xSize-col;
                break;
        }

        if(equalTilesUp==false){
            compare--;
        }else if(upSegment!=1&&upSegmentC==1){
            equalTilesUp = false;
        }

       if(compare>1&&equalTiles==true){
            switch(upSegment){
                case 1:
                    colLeft--;
                    break;
                case 2:
                    rowUp--;
                    lieDown = true;
                    break;
                case 3:
                    colLeft++;
                    break;
            }
            break;
            
        }else if(compare>3&&equalTiles==false){
            if(equalTilesUp==false){
                x = 2;
            }else{
                x = 1;
            }

            switch(upSegment){
                case 1:
                    col-=x;
                    colLeft-=x+1;
                    lieDown = true;
                    break;
                case 2:
                    row-=x;
                    rowUp-=x+1;
                    break;
                case 3:
                    col+=x;
                    colLeft+=x+1; 
                    lieDown = true;               
                    break;
            }
            break;

        }else{
            colLeft++;
            if(upSegment==1){
                rowUp--;
            }else{
                rowUp++;
            }
            
            while(boardCellsOccupied[rowUp][colLeft]==true){
                if(upSegment==1){
                    rowUp--;
                }else{
                    colLeft++;
                }
            }
            upSegment++;
            upSegmentC = 0;
        }
    }
}

void Board::checkDownPosition(bool equalTiles, int& row, int& col, bool& lieDown){
    int compare, x;

    while(true){
        row = rowDown;
        col = colRight;
        downSegmentC++;

        if(downSegment==4){
            cout<<"\nError: Cannot add more dominoes in one of the sides. Please increase the size of the board on the cpp...";
            pressEnterToContinue();
        }
        switch(downSegment){
            case 1:
                compare = xSize-col;
                break;
            case 2:
                compare = ySize-row;
                break;
            case 3:
                compare = col;
                break;
        }

        if(equalTilesDown==false){
            compare--;
        }else if(downSegment!=1&&downSegmentC==1){
            equalTilesDown = false;
        }

        if(compare>1&&equalTiles==true){
            switch(downSegment){
                case 1:
                    colRight++;
                    break;
                case 2:
                    rowDown++;
                    lieDown = true;
                    break;
                case 3:
                    colRight--;
                    break;
            }
            break;

        }else if(compare>3&&equalTiles==false){
            if(equalTilesDown==false){
                x = 2;
            }else{
                x = 1;
            }

            switch(downSegment){
                case 1:
                    col+=x;
                    colRight+=x+1;
                    lieDown = true;
                    break;
                case 2:
                    row+=x;
                    rowDown+=x+1;
                    break;
                case 3:
                    col-=x;
                    colRight-=x+1;
                    lieDown = true;               
                    break;
            }
            break;
        
        }else{
            colRight--;
            if(downSegment==1){
                rowDown++;
            }else{
                rowDown--;
            }
            
            while(boardCellsOccupied[rowDown][colRight]==true){
                if(downSegment==1){
                    rowDown++;
                }else{
                    colRight--;
                }
            }
            downSegment++;
            downSegmentC = 0;
        }
    }
}

bool Dominoes::convertIntToCard(bool newGame, int numberPlayers, int cardNumber){
    int a = 0, b = 0, x = 0, y = 7, n = 0, c = 0;
    
    for(x=0;x<=cardNumber;x++){
        if(x==n||x==0){
            b = n - c;
            n += y - c;
            a = c;
            c++;
        }
    }

    sideA = a;
    sideB = cardNumber - b;
    if(numberPlayers==3&&cardNumber==0){
        sideA = 6;
        sideB = 6;
    }

    if(newGame==true&&sideA==6&&sideB==6){
        return true;
    }else{
        return false;
    }
}

void Player::addCards(bool startsGame, vector<int>& shuffledCards, int numberPlayers, int cardsPerPlayer){    
    int y = 0;
    int x, cardNumber;
    Dominoes domino;

    cards.resize(cardsPerPlayer);
    cardsPlayed.resize(cardsPerPlayer);
    
    for(x=0;x<cardsPerPlayer;x++){
        cardNumber = shuffledCards[x];
        domino.convertIntToCard(false, numberPlayers, cardNumber);
        cards[y] = domino;
        cardsPlayed[y] = false;
        y++;
    }

    if(numberPlayers==3){
        cout<<"\nPlayer "<<nickname<<" deck:\n";
    }else if(numberPlayers==4){
        cout<<"\nTeam "<<nickname<<", Player "<<teamNumber<<" deck:\n";
    }

    if(startsGame==true){
        cout<<"- Starts the Game\n";
    }
    
    for(x=0;x<cards.size();x++){
        cout<<"\n\t("<<x+1<<")"<<" -> ["<<cards[x].sideA<<"|"<<cards[x].sideB<<"]";
    }
}

bool Player::canPlay(int cardUp, int cardDown){
    int x, sideA, sideB;

    for(x=0;x<cardsPlayed.size();x++){
        sideA = cards[x].sideA;
        sideB = cards[x].sideB;

        if(cardsPlayed[x]==false){
            if(sideA==cardUp||sideA==cardDown||sideB==cardUp||sideB==cardDown){
                return true;
            }
        }
    }
    return false;
}

void Player::playCard(Board& dominoesBoard, int cardsPerPlayer, int sixSixPosition){
    bool canPlay;
    int numberCards, cardNumber, sideA, sideB, cardUp, cardDown;
    Dominoes domino;

    if(dominoesBoard.numberCardsPlayed==0){
        cout<<"\n\tPlayer "<<nickname<<" started the game!";
    }

    if(sixSixPosition!=-1){
        cardsPlayed[sixSixPosition] = true;
        domino.sideA = 6; domino.sideB = 6;
    }else{
        if(dominoesBoard.numberCardsPlayed==0){
            cardNumber = intQuestion("Select a card of your deck", 1, cardsPerPlayer);
            domino = cards[cardNumber-1];
        }else{
            cardUp = dominoesBoard.cardUp; cardDown = dominoesBoard.cardDown;
            do{
                cardNumber = intQuestion("Select a card of your deck unused", 1, cardsPerPlayer);
                domino = cards[cardNumber-1];
                sideA = domino.sideA; sideB = domino.sideB;

                if(cardsPlayed[cardNumber-1]==false&&(sideA==cardUp||sideA==cardDown||sideB==cardUp||sideB==cardDown)){
                    break;
                }else{
                    cout<<"\t- Cannot play card of position "<<cardNumber;
                }
            }while(true); 
        }
        cardsPlayed[cardNumber-1] = true;
    }
    dominoesBoard.addCard(domino);   
}

int Player::countPointsLeft(){
    int x, cardsPointsLeft = 0;

    for(x=0;x<cardsPlayed.size();x++){
        if(cardsPlayed[x]==false){
            cardsPointsLeft += cards[x].sideA + cards[x].sideB;
        }
    }
    return cardsPointsLeft;
}

void Player::printCardsLeft(int numberPlayers){
    int y = 1;
    int x;

    if(numberPlayers==3){
        cout<<"\nPlayer "<<nickname<<" cards left:\n";
        
    }else if(numberPlayers==4){
        cout<<"\nTeam "<<nickname<<", Player "<<teamNumber<<" cards left:\n";
    }

    for(x=0;x<cardsPlayed.size();x++){
        if(cardsPlayed[x]==false){
            cout<<"\n\t("<<y<<")"<<" -> ["<<cards[x].sideA<<"|"<<cards[x].sideB<<"]";
            y++;
        }
    }
}

void Player::playerWinRound(bool teams, int points){
    if(teams==false){
        cout<<"\nPlayer "<<nickname<<" won the round!";
    }else{
        cout<<"\nTeam "<<nickname<<" won the round!";
    }
    cout<<"\n- Points: "<<points;
    pressEnterToContinue();
}

void Player::playerWinGame(Board& dominoesBoard){ // Random numbers in background when someone win the game
    random_device rd;

    int c = 0, lines = 3, numberTabs = 3;
    int x, y, l, tab, range, temp, sizeMessage, maxNumbersPerRow;
    string message, separator;
    vector<bool> shuffledBool;

    tab = 8;
    if(dominoesBoard.teams==false){
        message = "Player ";
    }else{
        message = "Team ";
    }
    message.append(nickname);
    message.append(" won the round!");

    sizeMessage = message.size() + tab*2*numberTabs;
    if(sizeMessage%tab!=2){
        sizeMessage = (sizeMessage/tab)*tab + 1;
    }
    separator = string(sizeMessage, '-');
    maxNumbersPerRow = (sizeMessage+tab-1)/tab;

    range = 2*maxNumbersPerRow*lines;
    do{
        shuffledBool.clear();
        for(x=0;x<range;x++){
            uniform_int_distribution<int> dist(0, 1);
            shuffledBool.push_back(dist(rd));
        }

        system("cls");
        x = 0;  
        for(l=0;l<2*lines;l++){
            if(l==lines){
                cout<<separator<<"\n"<<string(tab*numberTabs, ' ')<<message<<"\n"<<separator<<"\n\n";
            }

            for(y=0;y<maxNumbersPerRow;y++){
                if(shuffledBool[x+y]==true){;
                    cout<<"*\t";
                }else{
                    cout<<"\t";
                }
            }
            cout<<"\n\n";
            x+=y;
        }
        c++;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    }while(c<100);
}
