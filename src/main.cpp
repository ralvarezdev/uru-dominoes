#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

#include "lib/cards.h"
#include "lib/input.h"
#include "lib/terminal.h"

using std::cin;
using std::count;
using std::cout;
using std::find;
using std::sort;
using std::string;
using std::vector;

int main()
{
    const int nicknameMax = 10;

    Board dominoesBoard;
    vector<Player> players;
    dominoesBoard.boardSize(37, 21); // Must be an Odd Number

    bool newGame = true;
    bool answer, startsGame, gameEnded, roundEnded;
    int roundWinner = -1;
    int x, y, n, numberPlayers, numberCards, cardsPerPlayer, gameWinner,
        maxDoubles, points, pointsToWin, playerTurn, hand, sixDoublePos,
        startRange, endRange;
    vector<int> shuffledCards;

    string title = "*** Welcome to the Minimalist Dominoes Game ***\nMade by: ralvarezdev\n";
    for (x = 0; x < 2; x++)
    {
        clearAndPrint(&title);

        if (x == 0)
        {
            cout << "\nRules to Win a Round:\n\n\t1. You must be the first player to use all of his cards\n"
                 << "\t2. If the game is blocked, you must be the player with the less points in his deck\n"
                 << "\t3. If the 2nd criteria applies to two players, the winner would be:\n"
                 << "\t\ta. The one who started the game or the one who is closer to his turn\n"
                 << "\t\tb. ONFLY APPLIES FOR 3 PLAYERS MODE: The one who is next to the player who started the game\n"
                 << "\nRules to Win a Game:\n\n\t1. Reach the amount of points to win first\n";

            enterCharToContinue("Press 'y' to AGREE TO THE TERMS AND CONDITIONS", 'y');
        }
        else
        {
            pointsToWin = intQuestion("How Much Points to Win?", 50, 200);
            numberPlayers = intQuestion("Number of Players?", 3, 4);

            if (numberPlayers == 3)
            {
                maxDoubles = 5;
                numberCards = 27;
            }
            else if (numberPlayers == 4)
            {
                dominoesBoard.teams = true;
                maxDoubles = 4;
                numberCards = 28;
            }
            cardsPerPlayer = numberCards / numberPlayers;
        }
    }

    players.resize(numberPlayers);
    for (x = 0; x < numberPlayers; x++)
    {
        bool rightInput;
        string nickname;

        clearAndPrint(&title);
        do
        {
            rightInput = true;

            if (numberPlayers == 3)
                cout << "\nPlayer " << x + 1;
            else if (numberPlayers == 4)
                cout << "\nTeam " << x + 1;

            nickname = stringQuestion("Nickname");
            if (x > 0)
                rightInput = find(dominoesBoard.players.begin(), dominoesBoard.players.end(), nickname) == dominoesBoard.players.end();

            if (rightInput && nickname.size() > nicknameMax)
            {
                cout << "\t- Nickname too Long: It must Have " << nicknameMax << " or Less Characters\n";
                rightInput = false;
            }

            if (!rightInput)
                continue;

            rightInput = booleanQuestion("Are you sure");
        } while (!rightInput);

        dominoesBoard.players.push_back(nickname);
        dominoesBoard.points.push_back(0);

        if (numberPlayers == 3)
            players[x].nickname = nickname;
        else if (numberPlayers == 4)
        {
            players[x].teamNumber = 1;
            players[x].nickname = players[x + 2].nickname = nickname;
            players[x + 2].teamNumber = 2;

            if (x != 0)
                break;
        }
    }

    do
    {
        roundEnded = false;
        points = 0;
        dominoesBoard.numberCardsPlayed = 0;
        sixDoublePos = -1;

        clearAndPrint(&title);

        do
        {
            Dominoes domino;

            cout << "\n*** Shuffling Cards ***\n";
            shuffledCards = shuffleCards(numberCards, numberPlayers);

            for (x = 0; x < numberCards; x++)
            {
                if ((x + 1) % cardsPerPlayer == 0 || x == 0)
                    n = 0;

                startsGame = domino.convertIntToCard(newGame, numberPlayers, shuffledCards[x]);

                if (startsGame)
                {
                    playerTurn = x / cardsPerPlayer;
                    sixDoublePos = x % cardsPerPlayer;
                }

                if (domino.sideA == domino.sideB)
                    n++;

                if (n > maxDoubles)
                {
                    cout << "\n\t- Too much Doubles on Player Deck. Shuffling Cards...\n";
                    break;
                }
            }
        } while (n > maxDoubles);

        enterCharToContinue("Press 's' to Start the Game", 's');
        cin.ignore();

        for (x = 0; x < numberPlayers; x++)
        {
            clearAndPrint(&title);
            cout << "\n*** Player Cards ***\n";

            startsGame = (newGame && x == playerTurn) || roundWinner == x;
            players[x].addCards(startsGame, &shuffledCards, numberPlayers, cardsPerPlayer);

            shuffledCards.erase(shuffledCards.begin(), shuffledCards.begin() + cardsPerPlayer);
            pressEnterToCont();
        }

        do
        {
            dominoesBoard.printTitleBar(&title, numberPlayers);
            dominoesBoard.printBoard();

            if (dominoesBoard.numberCardsPlayed == 0)
            {
                n = 0;

                if (newGame)
                {
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer, sixDoublePos);
                    newGame = false;
                    pressEnterToCont();
                }
                else
                {
                    playerTurn = roundWinner;
                    roundWinner = -1;
                    hand = playerTurn;
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer);
                    cin.ignore();
                }
            }
            else
            {
                if (numberPlayers == 3)
                    cout << "\n\tPlayer " << players[playerTurn].nickname << " Turn:\n";
                else if (numberPlayers == 4)
                    cout << "\n\tTeam " << players[playerTurn].nickname << ", Player "
                         << players[playerTurn].teamNumber << " Turn:\n";

                answer = players[playerTurn].canPlay(dominoesBoard.cardUp, dominoesBoard.cardDown);

                if (answer)
                {
                    n = 0;
                    players[playerTurn].playCard(&dominoesBoard, cardsPerPlayer);
                    cin.ignore();
                }
                else
                {
                    n++;

                    if (numberPlayers == 3)
                        cout << "\n\t- Player " << players[playerTurn].nickname << " cannot Play in any Side";
                    else if (numberPlayers == 4)
                    {
                        cout << "\n\t- Team " << players[playerTurn].nickname << ", Player "
                             << players[playerTurn].teamNumber << " cannot Play in any Side";
                    }
                    pressEnterToCont();
                }
            }

            if (players[playerTurn].countPointsLeft() == 0)
            {
                roundWinner = playerTurn;

                for (x = 0; x < numberPlayers; x++)
                {
                    cout << players[roundWinner].nickname << " doesn't have cards left!\n";

                    if (x != playerTurn)
                    {
                        dominoesBoard.printTitleBar(&title, numberPlayers);
                        dominoesBoard.printBoard();
                        players[x].printCardsLeft(numberPlayers);
                        pressEnterToCont();

                        points += players[x].countPointsLeft();
                    }
                }
            }
            else if (n != numberPlayers)
            {
                if (playerTurn == numberPlayers - 1)
                    playerTurn = 0;
                else
                    playerTurn++;
            }
            else
            {
                cout << "\nGame blocked: Checking who is the Winner";

                int t, minPoints, index;
                vector<int> playersPoints, sortedPlayersPoints;
                vector<int>::iterator itr;

                for (x = 0; x < numberPlayers; x++)
                {
                    dominoesBoard.printTitleBar(&title, numberPlayers);
                    dominoesBoard.printBoard();
                    players[x].printCardsLeft(numberPlayers);
                    pressEnterToCont();

                    playersPoints.push_back(players[x].countPointsLeft());
                    sortedPlayersPoints.push_back(playersPoints[x]);
                    points += playersPoints[x];
                }
                sort(sortedPlayersPoints.begin(), sortedPlayersPoints.end());
                minPoints = sortedPlayersPoints[0];

                t = count(playersPoints.begin(), playersPoints.end(), minPoints);
                if (t == 1)
                {
                    itr = find(playersPoints.begin(), playersPoints.end(), minPoints);
                    index = itr - playersPoints.begin();
                }
                else
                {
                    y = numberPlayers - 1;

                    for (x = 0; x < numberPlayers; x++)
                        if (playersPoints[x] == minPoints)
                            if (hand == x || (numberPlayers == 4 && (hand == x + 2 || hand == x - 2)))
                            {
                                index = x;
                                break;
                            }
                            else if ((hand != y && x == hand + 1) || (hand == y && x == 0))
                                index = x;
                }
                roundWinner = index;
            }
        } while (roundWinner == -1);

        dominoesBoard.printTitleBar(&title, numberPlayers);

        if (numberPlayers == 3)
        {
            players[roundWinner].playerWinRound(false, points);

            players[roundWinner].points += points;
            dominoesBoard.points[roundWinner] = players[roundWinner].points;
        }
        else if (numberPlayers == 4)
        {
            players[roundWinner].playerWinRound(true, points);

            if (roundWinner == 0 || roundWinner == 2)
                dominoesBoard.points[0] = players[0].points = players[2].points += points;
            else
                dominoesBoard.points[1] = players[1].points = players[3].points += points;
        }

        gameEnded = players[roundWinner].points >= pointsToWin;
        gameWinner = roundWinner;
    } while (gameEnded == false);

    players[gameWinner].playerWinGame(&dominoesBoard);
}