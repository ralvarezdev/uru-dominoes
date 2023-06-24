#include<string>
#include<vector>

using std::string;
using std::vector;


#ifndef CARDS_H
#define CARDS_H

class Dominoes{
    public:
        int sideA, sideB;
        bool played = false;
        int xPos, yPos;

        bool convertIntToCard(bool newGame, int numberPlayers, int cardNumber);
};

class Board{
    public:
        bool teams = false;
        int xSize, ySize;
        int xCenter, yCenter;
        int cardPlayedUp, cardPlayedDown;
        bool equalTilesUp, equalTilesDown;
        int colRight, colLeft, rowUp, rowDown;
        int upSegment, downSegment;
        int cardsPlayed = 0;
        vector<vector<bool>> boardCellsOccupied;
        vector<vector<int>> boardCellsContent;
        vector<string> players;
        vector<int> points;

        void boardSize(int x, int y);
        void printTitleBar(string title, int numberPlayers);
        void printBoard();
        void addCard(Dominoes domino);
        void changeCellContent(bool equalTiles, bool lieDown, int row, int col, int sideFree, int free, int sideConnected = -1, int connected = -1, int separator = -1);
        void checkUpPosition(bool equalTiles, int *row, int *col, bool *lieDow, bool *separatorn);
        void checkDownPosition(bool equalTiles, int *row, int *col, bool *lieDown, bool *separator);
};

class Player{
    public:
        int points = 0;
        int numberCardsPlayed = 0;
        int teamNumber = -1;
        string nickname;
        vector<Dominoes> cards;
        vector<int> cardsPlayed;

        int addCards(bool newGame, vector<int> shuffledCards, int players, int cardsPerPlayer, int startRange, int endRange);
        bool canPlay(int cardPlayedUp, int cardPlayedDown);
        void playCard(Board *dominoesBoard, int cardsPerPlayer, int sixSixPosition = -1);
        int countPointsLeft();
        void printCardsLeft(int numberPlayers);
        void playerWinRound(bool teams, int points);
        bool playerWinGame(bool teams, int pointsToWin);
        void winBackground(int numberPlayers);
};

vector<int> shuffleCards(int numberCards, int players);

#endif 