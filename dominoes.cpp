#include"input.h"
#include"cards.h"
#include<iostream>
#include<string>
#include<vector>
#include<algorithm>

using std::cout; using std::cin;
using std::string;
using std::vector;
using std::count; using std::distance; using std::find; using std::sort;

int main(){
    const int nicknameMax = 10;

    Board dominoesBoard;
    vector<Player> players;
    dominoesBoard.boardSize(37, 21); // Must be an odd number

    bool newGame = true;
    bool answer, startsGame, gameEnded, roundEnded;
    int roundWinner = -1;
    int x, y, n, input, numberPlayers, numberCards, cardsPerPlayer, gameWinner,
        maxDoubles, points, pointsToWin, playerTurn, hand, sixSixPosition,
        startRange, endRange;
    vector<int> shuffledCards;

    string title = "*** Welcome to the Minimalist Dominoes Game ***\nmade by: blackb0x\n";
    for(x=0;x<2;x++){
        clearAndTitle(title);

        if(x==0){
            cout<<"\nRules to win a Round:\n\n\t1. Be the first player to use all of his cards\n"
                <<"\t2. If the game is blocked, be the player with the less points in his deck\n"
                <<"\t3. If the 2nd criteria applies to two players, the winner would be:\n"
                <<"\t\ta. The one who started the game or the one is closer to his turn\n"
                <<"\t\tb. ONFLY APPLIES FOR 3 PLAYERS MODE: The one who is next to the player who started the game\n"
                <<"\nRules to win a Game:\n\n\t1. Reach the ammount of points to win first\n";

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
        }
    }

    players.resize(numberPlayers);
    for(x=0;x<numberPlayers;x++){     
        bool rightInput;
        string nickname;
        
        clearAndTitle(title);
        do{
            rightInput = true;
            if(numberPlayers==3){
                cout<<"\nPlayer "<<x+1;
            }else if(numberPlayers==4){
                cout<<"\nTeam "<<x+1;
            }

            nickname = stringQuestion("Nickname");
            if(x>0){
                rightInput = find(dominoesBoard.players.begin(), dominoesBoard.players.end(), nickname)==dominoesBoard.players.end();
            }

            if(nickname.size()>nicknameMax){
                cout<<"\t- Nickname too long: It must have "<<nicknameMax<<" or less characters\n";
                rightInput = false;
            }else if(rightInput==true){
                rightInput = booleanQuestion("Are you sure");
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
    }

    do{        
        roundEnded = false;
        points = 0;
        dominoesBoard.numberCardsPlayed = 0;
        sixSixPosition = -1;

        clearAndTitle(title);
        do{
            Dominoes domino;

            cout<<"\n*** Shuffling cards ***\n";
            shuffledCards = shuffleCards(numberCards, numberPlayers);

            for(x=0;x<numberCards;x++){
                if((x+1)%cardsPerPlayer==0||x==0){
                    n = 0;
                }

                startsGame = domino.convertIntToCard(newGame, numberPlayers, shuffledCards[x]);
                if(startsGame==true){
                    playerTurn = x/cardsPerPlayer;
                    sixSixPosition = x%cardsPerPlayer;
                }

                if(domino.sideA==domino.sideB){
                    n++;
                }

                if(n>maxDoubles){
                    cout<<"\n\t- Too much doubles on a player deck. Shuffling cards...\n";
                    break;
                }
            }
        }while(n>maxDoubles);

        enterCharToContinue("Press '1' to Start the Game", "1");
        cin.ignore();

        for(x=0;x<numberPlayers;x++){
            clearAndTitle(title);
            cout<<"\n*** Shuffling cards ***\n";

            startsGame = (newGame==true&&x==playerTurn)||roundWinner==x;
            players[x].addCards(startsGame, shuffledCards, numberPlayers, cardsPerPlayer);
            shuffledCards.erase(shuffledCards.begin(), shuffledCards.begin()+cardsPerPlayer);
            pressEnterToContinue();
        } 

        do{
            dominoesBoard.printTitleBar(title, numberPlayers);
            dominoesBoard.printBoard();

            if(dominoesBoard.numberCardsPlayed==0){
                n = 0;
                if(newGame==true){
                    players[playerTurn].playCard(dominoesBoard, cardsPerPlayer, sixSixPosition);
                    newGame = false;
                    pressEnterToContinue();
                }else{
                    playerTurn = roundWinner;
                    roundWinner = -1;
                    hand = playerTurn;
                    players[playerTurn].playCard(dominoesBoard, cardsPerPlayer);
                    cin.ignore();
                }
            }else{
                if(numberPlayers==3){
                    cout<<"\n\tPlayer "<<players[playerTurn].nickname<<" turn:\n";

                }else if(numberPlayers==4){
                    cout<<"\n\tTeam "<<players[playerTurn].nickname<<", Player "<<players[playerTurn].teamNumber<<" turn:\n";
                }

                answer = players[playerTurn].canPlay(dominoesBoard.cardUp, dominoesBoard.cardDown);

                if(answer==true){
                    n = 0;
                    players[playerTurn].playCard(dominoesBoard, cardsPerPlayer);
                    cin.ignore();
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
                    cout<<players[roundWinner].nickname<<" doesn't have cards left!\n";

                    if(x!=playerTurn){
                        dominoesBoard.printTitleBar(title, numberPlayers);
                        dominoesBoard.printBoard();
                        players[x].printCardsLeft(numberPlayers);
                        pressEnterToContinue();

                        points += players[x].countPointsLeft();
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

                int t, minPoints, index;
                vector<int> playersPoints, sortedPlayersPoints;
                vector<int>::iterator itr;

                for(x=0;x<numberPlayers;x++){
                    dominoesBoard.printTitleBar(title, numberPlayers);
                    dominoesBoard.printBoard();
                    players[x].printCardsLeft(numberPlayers);
                    pressEnterToContinue();

                    playersPoints.push_back(players[x].countPointsLeft());
                    sortedPlayersPoints.push_back(playersPoints[x]);
                    points += playersPoints[x];
                }
                sort(sortedPlayersPoints.begin(), sortedPlayersPoints.end());
                minPoints = sortedPlayersPoints[0];

                t = count(playersPoints.begin(), playersPoints.end(), minPoints);
                if(t==1){
                    itr = find(playersPoints.begin(), playersPoints.end(), minPoints);
                    index = itr - playersPoints.begin();
                }else{
                    y = numberPlayers - 1;
                    for(x=0;x<numberPlayers;x++){
                        if(playersPoints[x]==minPoints){
                            if(hand==x||(numberPlayers==4&&(hand==x+2||hand==x-2))){
                                index = x;
                                break;
                            }else if((hand!=y&&x==hand+1)||(hand==y&&x==0)){
                                index = x;
                            }
                        }
                    }
                }
                roundWinner = index;
            }
        }while(roundWinner==-1);

        dominoesBoard.printTitleBar(title, numberPlayers);

        if(numberPlayers==3){
            players[roundWinner].playerWinRound(false, points);
             
            players[roundWinner].points += points;
            dominoesBoard.points[roundWinner] = players[roundWinner].points;

        }else if(numberPlayers==4){
            players[roundWinner].playerWinRound(true, points);

            if(roundWinner==0||roundWinner==2){
                players[0].points += points;
                players[2].points += points;
                dominoesBoard.points[0] = players[0].points;
            }else{
                players[1].points += points;
                players[3].points += points;
                dominoesBoard.points[1] = players[1].points;
            }            
        }  
        gameEnded = players[roundWinner].points>=pointsToWin;
        gameWinner = roundWinner;

    }while(gameEnded==false);

    players[gameWinner].playerWinGame(dominoesBoard);
}
