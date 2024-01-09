#include <string>
#include <vector>

using std::string;
using std::vector;

#ifndef CARDS_H
#define CARDS_H

class Dominoes
{
public:
    int sideA, sideB;

    bool convertIntToCard(bool newGame, int numberPlayers, int cardNumber);
};

class Board
{
public:
    bool teams = false;
    bool equalTilesUp, equalTilesDown;
    int upSegmentC = 0, downSegmentC = 0, numberCardsPlayed = 0;
    int xSize, ySize, xCenter, yCenter, cardUp, cardDown, colRight, colLeft, upSegment, downSegment, rowUp, rowDown;
    vector<int> points;
    vector<string> players;
    vector<vector<bool>> boardCellsOccupied;
    vector<vector<int>> boardCellsContent;

    void boardSize(int x, int y);
    void printTitleBar(string *title, int numberPlayers);
    void printBoard();
    void addCard(Dominoes *domino);
    void changeCellContent(bool equalTiles, bool lieDown, int row, int col, int sideFree, int free, int sideConnected = -1, int connected = -1, int separator = -1);
    void checkUpPosition(bool equalTiles, int *row, int *col, bool *lieDown);
    void checkDownPosition(bool equalTiles, int *row, int *col, bool *lieDown);
};

class Player
{
public:
    int points = 0, numberCardsPlayed = 0, teamNumber = -1;
    string nickname;
    vector<Dominoes> cards;
    vector<bool> cardsPlayed;

    void addCards(bool startsGame, vector<int> *shuffledCards, int numberPlayers, int cardsPerPlayer);
    bool canPlay(int cardPlayedUp, int cardPlayedDown);
    void playCard(Board *dominoesBoard, int cardsPerPlayer, int sixSixPosition = -1);
    int countPointsLeft();
    void printCardsLeft(int numberPlayers);
    void playerWinRound(bool teams, int points);
    void playerWinGame(Board *dominoesBoard);
};

vector<int> shuffleCards(int numberCards, int players);

#endif