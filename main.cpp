#include<iostream>
#include<string>
#include<vector>
//#include<windows.h>
#include"input.h"
#include"cards.h"

using std::cout; using std::cin;
using std::string;
using std::vector;


int main(){
    Board dominoesBoard; 
    dominoesBoard.boardSize(37, 21); // Must be an odd number

    bool answer, gameEnded, roundEnded;
    int x, y, n, input, 
        numberPlayers, numberCards, cardsPerPlayer, gameWinner,
        maxDoubles, points, pointsToWin,
        playerTurn, hand, sixSixPosition, startRange, endRange;
    int newGame = true;
    int roundWinner = -1;
    string nickname;
    vector<int> dominoesDoubles, shuffledCards;

    string title = "*** Welcome to the Minimalist Dominoes Game ***\nmade by: blackb0x\n";

    for(x=0;x<2;x++){
        clearAndTitle(title);

        if(x==0){
            cout<<"\nRules to win a Round:\n\n\t1. Be the first player to use all of his cards\n";
            cout<<"\t2. If the game is blocked, be the player with the less points in his deck\n";
            cout<<"\t3. If the 2nd criteria applies to two players, the winner would be:\n";
            cout<<"\t\ta. The one who started the game or the one is closer to his turn\n";
            cout<<"\t\tb. ONFLY APPLIES FOR 3 PLAYERS MODE: The one who is next to the player who started the game\n";

            cout<<"\nRules to win a Game:\n\n\t1. Reach the ammount of points to win first\n";

            enterCharToContinue("Press '1' to AGREE TO THE TERMS AND CONDITIONS", "1");
        }else{
            pointsToWin = intQuestion("How much points to win?", 50, 200);
            numberPlayers = intQuestion("Number of players?", 3, 4);

            if(numberPlayers==3){
                maxDoubles = 5;
                numberCards = 27;

            }else if(numberPlayers==4){
                dominoesBoard.teams = true;
                maxDoubles = 4;
                numberCards = 28;
            }

            cardsPerPlayer = numberCards/numberPlayers;
            dominoesDoubles.push_back(0);

            y = 0;
            for(x=7;x>1;x--){            
                y += x;

                if(numberPlayers!=3||(numberPlayers==3&&y!=27)){
                    dominoesDoubles.push_back(y);
                }
            }
        }
    }

    Player players[numberPlayers];

    x = 0;
    do{
        bool rightInput = false;

        clearAndTitle(title);

        do{
            if(numberPlayers==3){
                cout<<"\nPlayer "<<x+1;

            }else if(numberPlayers==4){
                cout<<"\nTeam "<<x+1;
            }

            nickname = stringQuestion("Nickname");

            switch(x){
                case 0:
                    rightInput = true;
                    break;
                case 1:
                    if(nickname!=players[0].nickname){
                        rightInput = true;
                    }
                    break;
                case 2:
                    if(nickname!=players[0].nickname&&nickname!=players[1].nickname){
                        rightInput = true;
                    }
                    break;
            }

            if(rightInput==true){
                answer = booleanQuestion("Are you sure");
                if(answer==false){
                    rightInput = false;
                }               
            }
            
        }while(rightInput==false);

        players[x].nickname = nickname;
        dominoesBoard.players.push_back(nickname);
        dominoesBoard.points.push_back(0);
                
        if(numberPlayers==4){
            if(x==0){
                players[0].teamNumber = 1;
                players[2].nickname = nickname;
                players[2].teamNumber = 2;
            }else{
                players[1].teamNumber = 1;
                players[3].nickname = nickname;
                players[3].teamNumber = 2;
                x = 3;
            }
        }
        x++;

    }while(x<numberPlayers);

    do{        
        roundEnded = false;
        points = 0;
        dominoesBoard.cardsPlayed = 0;

        clearAndTitle(title);

        do{
            cout<<"\n*** Shuffling cards ***\n";
            shuffledCards = shuffleCards(numberCards, numberPlayers);

            for(x=0;x<numberCards-1;x++){
                if(x%cardsPerPlayer==0){
                    n = 0;
                }

                for(y=0;y<dominoesDoubles.size();y++){
                    if(shuffledCards[x]==dominoesDoubles[y]){                        
                        n++;
                    }
                }

                if(n>maxDoubles){
                    cout<<"\n\t- Too much doubles on a player deck. Shuffling cards...\n";
                    break;
                }
            }
        }while(n>maxDoubles);

        enterCharToContinue("Press '1' to Start the Game", "1");

        for(x=0;x<numberPlayers;x++){
            int hasSixSix;

            clearAndTitle(title);
            cout<<"\n*** Shuffling cards ***\n";

            startRange = x*cardsPerPlayer;
            endRange = (x+1)*cardsPerPlayer;
            hasSixSix = players[x].addCards(newGame, shuffledCards, numberPlayers, cardsPerPlayer, startRange, endRange);

            if(hasSixSix!=-1){
                playerTurn = x;
                sixSixPosition = hasSixSix;
            }
           
            enterCharToContinue("Press '1' to Continue", "1");
        }

        do{
            dominoesBoard.printTitleBar(title, numberPlayers);
            dominoesBoard.printBoard();

            if(dominoesBoard.cardsPlayed==0){
                n = 0;
                if(newGame==true){
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer, sixSixPosition);
                    newGame = false;
                    pressEnterToContinue();
                }else{
                    if(numberPlayers==3){
                        playerTurn = roundWinner;

                    }else if(numberPlayers==4){
                        answer = booleanQuestion("Player 1 of Winner Team wants to Start?");
                            
                        if(roundWinner==0||roundWinner==2){
                            if(answer==true){
                                playerTurn = 0;
                            }else{
                                playerTurn = 2;
                            }
                        }else{
                            if(answer==true){
                                playerTurn = 1;
                            }else{
                                playerTurn = 3;
                            }
                        }
                    }
                    roundWinner = -1;
                    hand = playerTurn;
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer);
                }
            }else{
                if(numberPlayers==3){
                    cout<<"\n\tPlayer "<<players[playerTurn].nickname<<" turn:\n";

                }else if(numberPlayers==4){
                    cout<<"\n\tTeam "<<players[playerTurn].nickname<<", Player "<<players[playerTurn].teamNumber<<" turn:\n";
                }

                answer = players[playerTurn].canPlay(dominoesBoard.cardPlayedUp, dominoesBoard.cardPlayedDown);

                if(answer==true){
                    n = 0;
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer);
                }else{
                    n++;
                        
                    if(numberPlayers==3){
                        cout<<"\n\t- Player "<<players[playerTurn].nickname<<" cannot play in any of both sides";
                        
                    }else if(numberPlayers==4){
                        cout<<"\n\t- Team "<<players[playerTurn].nickname<<", Player "<<players[playerTurn].teamNumber<<" cannot play in any of both sides";
                    }
                    pressEnterToContinue();
                }
            }

            if(players[playerTurn].countPointsLeft()==0){
                roundWinner = playerTurn;

                for(x=0;x<numberPlayers;x++){
                    cout<<players[roundWinner].nickname<<" have no cards left!\n";

                    if(x!=playerTurn){
                        dominoesBoard.printTitleBar(title, numberPlayers);
                        dominoesBoard.printBoard();
                        players[x].printCardsLeft(numberPlayers);

                        points += players[x].countPointsLeft();
                        pressEnterToContinue();
                    }
                }
            }else if(n!=numberPlayers){
                if(playerTurn==numberPlayers-1){
                    playerTurn = 0;
                }else{
                    playerTurn++;
                }
            }else{
                cout<<"\nGame blocked: Checking who is the winner";

                bool draw; 
                int minPoints, playerPoints;
                vector<int> drawPlayers;

                for(x=0;x<numberPlayers;x++){
                    dominoesBoard.printTitleBar(title, numberPlayers);
                    dominoesBoard.printBoard();
                    players[x].printCardsLeft(numberPlayers);

                    playerPoints = players[x].countPointsLeft();
                    points += playerPoints;

                    if((x==0)||(playerPoints<minPoints)){
                        minPoints = playerPoints;
                        roundWinner = x;

                        if(draw==true){
                            draw = false;
                            drawPlayers.erase(drawPlayers.begin(), drawPlayers.end());
                        }
                            
                    }else if(playerPoints==minPoints){
                        if(draw==false){
                            draw = true;
                            drawPlayers.push_back(roundWinner);
                            roundWinner = -1;
                        }
                        drawPlayers.push_back(x);
                    }
                    pressEnterToContinue();
                }

                if(draw==true){
                    cout<<"\n\n\tDraw between ";

                    int y;
                    int drawPlayersSize = drawPlayers.size();
                    for(x=0;x<drawPlayersSize;x++){
                        y = drawPlayers[x];

                        if(roundWinner==-1){
                            if(y==hand){
                                roundWinner = y;
                            }else if(numberPlayers==4&&hand==y+2){
                                roundWinner = y;
                            }else if(x==drawPlayersSize-1){
                                roundWinner = hand + 1;
                            }
                        }
                            
                        if(x<drawPlayersSize-1){
                            cout<<players[y].nickname<<" and ";

                        }else{
                            cout<<players[y].nickname<<"\n";
                        }
                    }

                    for(x=0;x<drawPlayersSize;x++){
                        y = drawPlayers[x];
                        players[y].printCardsLeft(numberPlayers);
                    }
                }
            }
        }while(roundWinner==-1);

        dominoesBoard.printTitleBar(title, numberPlayers);

        if(numberPlayers==3){
            players[roundWinner].playerWinRound(false, points);
             
            players[roundWinner].points += points;
            dominoesBoard.points[roundWinner] = players[roundWinner].points;
            gameEnded = players[roundWinner].playerWinGame(false, pointsToWin);

        }else if(numberPlayers==4){
            players[roundWinner].playerWinRound(true, points);

            if(roundWinner==0||roundWinner==2){
                players[0].points += points;
                players[2].points += points;
                dominoesBoard.points[0] = players[0].points;

            }else if(roundWinner==1||roundWinner==3){
                players[1].points += points;
                players[3].points += points;
                dominoesBoard.points[1] = players[1].points;
            }
            
            gameEnded = players[roundWinner].playerWinGame(true, pointsToWin);
            gameWinner = roundWinner;
        }    
    }while(gameEnded==false);

    players[gameWinner].winBackground(numberPlayers);
}
