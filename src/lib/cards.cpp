#include <iostream>
#include <random>
#include <sstream>
#include <vector>
#include <chrono>
#include <thread>

#include "cards.h"
#include "input.h"
#include "terminal.h"

using std::cout;
using std::ostringstream;
using std::random_device;
using std::uniform_int_distribution;
using std::vector;

// --- Function Prototypes
void oddToEven(int *n);
vector<int> shuffleCards(int numberCards, int players);

// --- Functions

// Increment Number by 1 if it's Odd
void oddToEven(int *n)
{
    *n += (*n % 2 != 0) ? 1 : 0;
}

// Function to Shuffle Cards
vector<int> shuffleCards(int numberCards, int players)
{
    random_device rd; // Random Number Generator

    int x, n, random, number;
    vector<int> shuffledCards, cards;

    // Check if Number of Cards Divided by Number of Players has a Remainder
    if (numberCards % players != 0)
        cout << "\n\t- Code Error: Cannot Equally Share Cards";

    for (x = 0; x < numberCards; x++)
        cards.push_back(x);

    n = 1;
    for (x = 0; x + 1 < numberCards; x++, n++)
    {
        uniform_int_distribution<int> dist(0, numberCards - n);
        random = dist(rd);

        number = cards[random];
        cards.erase(cards.begin() + random);
        shuffledCards.push_back(number);
    }
    shuffledCards.push_back(cards[0]);

    return shuffledCards;
}

// --- Methods

// Method to Get the Board Size
void Board::boardSize(int x, int y)
{
    oddToEven(&x);
    oddToEven(&y);

    xSize = x;
    ySize = y;

    xCenter = (xSize / 2); // We're Counting Position 0
    yCenter = (ySize / 2);

    boardCellsOccupied.resize(ySize);
    boardCellsContent.resize(ySize);

    for (y = 0; y < ySize; y++)
    {
        boardCellsOccupied[y].resize(xSize);
        boardCellsContent[y].resize(xSize);
    }
}

// Method to Print Title Bar
void Board::printTitleBar(string *title, int numberPlayers)
{
    clearAndPrint(title);

    if (numberPlayers == 3)
        cout << "\n"
             << players[0] << " (" << points[0] << ") vs " << players[1] << " (" << points[1] << ") vs " << players[2] << " (" << points[2] << ")\n";
    else if (numberPlayers == 4)
        cout << "\nTeam " << players[0] << " (" << points[0] << ") vs Team " << players[1] << " (" << points[1] << ")\n";
}

// Method to Print Board
void Board::printBoard()
{
    int x, y, cellContent;

    ostringstream stream;

    stream << '\n';

    for (y = 0; y < ySize; y++)
    {
        for (x = 0; x < xSize; x++)
            if (!boardCellsOccupied[y][x])
                stream << "   ";
            else
            {
                cellContent = boardCellsContent[y][x];

                if (cellContent < 7)
                    stream << '[' << cellContent << ']';
                else if (cellContent == 7)
                    stream << " | ";
                else if (cellContent == 8)
                    stream << " - ";
            }
        stream << '\n';
    }
    cout << stream.str();
}

// Method to Change Cell Content
void Board::changeCellContent(bool equalTiles, bool lieDown, int row, int col, int sideFree, int free, int sideConnected, int connected, int separator)
{
    // To Calculate Card X and Y Coordinates
    int x, a, b, c, d, e, f, n;

    a = (lieDown) ? row : free;
    n = (lieDown) ? 7 : 8;

    if (equalTiles)
    {
        c = (lieDown) ? free : col;
        b = (lieDown) ? 0 : 1;
        d = (lieDown) ? 1 : 0;

        boardCellsContent[a][c] = sideFree;
        boardCellsContent[a + b][c + d] = n;
        boardCellsContent[a + 2 * b][c + 2 * d] = sideFree;

        for (x = 0; x < 3; x++)
            boardCellsOccupied[a + b * x][c + d * x] = true;
    }
    else
    {
        b = (lieDown) ? free : col;
        ((lieDown) ? d : c) = connected;
        ((lieDown) ? f : e) = separator;

        if (lieDown)
            e = c = a;
        else
            f = d = b;

        boardCellsContent[a][b] = sideFree;
        boardCellsContent[c][d] = sideConnected;
        boardCellsOccupied[c][d] = boardCellsOccupied[a][b] = true;

        if (separator != -1)
        {
            boardCellsContent[e][f] = n;
            boardCellsOccupied[e][f] = true;
        }
    }
}

// Method to Add Card
void Board::addCard(Dominoes *domino)
{
    bool answer, equalTiles, lieDown;
    int cardPosition;
    int separator = -1;
    int x, y, row, col, cardPlayed, sideA, sideB, sideConnected, sideFree, freePosition, connectedPosition;

    sideA = (*domino).sideA;
    sideB = (*domino).sideB;
    if (numberCardsPlayed != 0)
    {
        if ((cardUp == cardDown) || (sideA == cardUp && sideB == cardDown) || (sideB == cardUp && sideA == cardDown))
        {
            answer = booleanQuestion("Where do you want to put the card 'Yes' for Up/Left, 'No' for Down/Right?");
            cardPosition = (answer) ? 'u' : 'd';
        }
        else
            cardPosition = (sideA == cardUp || sideB == cardUp) ? 'u' : 'd';
    }
    else
    {
        upSegment = downSegment = 1;
        upSegmentC = downSegmentC = 0;
        colLeft = colRight = col = xCenter;
        rowUp = rowDown = row = yCenter;
        cardUp = sideA;
        cardDown = sideB;

        for (y = 0; y < ySize; y++)
            for (x = 0; x < xSize; x++)
            {
                boardCellsOccupied[y][x] = false;
                boardCellsContent[y][x] = -1;
            }
    }

    equalTiles = (sideA == sideB);
    if (equalTiles)
        sideFree = sideA;
    else
    {
        cardPlayed = (cardPosition == 'u') ? cardUp : cardDown;
        sideConnected = (sideA == cardPlayed) ? sideA : sideB;

        sideFree = (sideA == cardPlayed) ? sideB : sideA;
        ((cardPosition == 'u') ? cardUp : cardDown) = sideFree;
    }

    if (numberCardsPlayed == 0)
    {
        colLeft--;
        colRight++;
        lieDown = !equalTiles;

        if (lieDown)
            colRight++;
    }
    else if (cardPosition == 'u')
        checkUpPosition(equalTiles, &row, &col, &lieDown);
    else
        checkDownPosition(equalTiles, &row, &col, &lieDown);

    // For Testing
    /*
     */
    cout << equalTiles << ' ' << sideA << ' ' << sideB;
    pressEnterToCont();

    if (equalTiles)
    {
        int sep = (lieDown ? col : row) - 1;
        // For Testing
        /*
        cout << lieDown << '\n'
             << row << '\n'
             << col << '\n'
             << sideFree << '\n'
             << sep;
        pressEnterToCont();
        */
        changeCellContent(true, lieDown, row, col, sideFree, sep);

        if (cardPosition == 'u')
            equalTilesUp = true;
        else
            equalTilesDown = true;
    }
    else if (numberCardsPlayed == 0)
    {
        equalTilesUp = equalTilesDown = false;
        changeCellContent(false, true, row, col, sideA, xCenter, sideB, xCenter + 1);
    }
    else
    {
        freePosition = (lieDown) ? col : row;

        if (cardPosition == 'u')
        {
            x = (upSegment != 3) ? 1 : -1;

            connectedPosition = freePosition + x;
            if (equalTilesUp)
                equalTilesUp = false;
            else
                separator = freePosition + x * 2;
        }
        else if (cardPosition == 'd')
        {
            x = (downSegment != 3) ? -1 : 1;

            connectedPosition = freePosition + x;
            if (equalTilesDown)
                equalTilesDown = false;
            else
                separator = freePosition + 2 * x;
        }

        // For Testing
        /*
        cout << lieDown << '\n'
             << row << '\n'
             << col << '\n'
             << sideFree << '\n'
             << freePosition << '\n'
             << sideConnected << '\n'
             << connectedPosition << '\n'
             << separator;
        pressEnterToCont();
        */
        changeCellContent(false, lieDown, row, col, sideFree, freePosition, sideConnected, connectedPosition, separator);
    }
    numberCardsPlayed++;
}

void Board::checkUpPosition(bool equalTiles, int *row, int *col, bool *lieDown)
{
    int compare, x;

    while (true)
    {
        *row = rowUp;
        *col = colLeft;
        upSegmentC++;

        if (upSegment == 4)
        {
            cout << "\nError: Cannot Add more Dominoes in One Side. Please Increase Board Size in main.cpp...";
            pressEnterToCont();
            return; // End this Function
        }

        switch (upSegment)
        {
        case 1:
            compare = *col;
            break;
        case 2:
            compare = *row;
            break;
        case 3:
            compare = xSize - *col;
            break;
        }

        if (!equalTilesUp)
            compare--;
        else if (upSegment != 1 && upSegmentC == 1)
            equalTilesUp = false;

        if (compare > 1 && equalTiles)
        {
            switch (upSegment)
            {
            case 1:
                colLeft--;
                break;
            case 2:
                rowUp--;
                *lieDown = true;
                break;
            case 3:
                colLeft++;
                break;
            }
        }
        else if (compare > 3 && !equalTiles)
        {
            x = (equalTilesUp) ? 1 : 2;
            *lieDown = (upSegment % 2 == 1);

            switch (upSegment)
            {
            case 1:
                *col -= x;
                colLeft -= x + 1;
                break;
            case 2:
                *row -= x;
                rowUp -= x + 1;
                break;
            case 3:
                *col += x;
                colLeft += x + 1;
                break;
            }
        }
        else
        {
            colLeft++;
            rowUp += (upSegment == 1) ? -1 : 1;

            while (boardCellsOccupied[rowUp][colLeft])
                if (upSegment == 1)
                    rowUp--;
                else
                    colLeft++;

            upSegment++;
            upSegmentC = 0;
            continue;
        }
        break;
    }
}

void Board::checkDownPosition(bool equalTiles, int *row, int *col, bool *lieDown)
{
    int compare, x;

    while (true)
    {
        *row = rowDown;
        *col = colRight;
        downSegmentC++;

        if (downSegment == 4)
        {
            cout << "\nError: Cannot Add more Dominoes in One Side. Please Increase Board Size in main.cpp...";
            pressEnterToCont();
            return; // End this Function
        }

        switch (downSegment)
        {
        case 1:
            compare = xSize - *col;
            break;
        case 2:
            compare = ySize - *row;
            break;
        case 3:
            compare = *col;
            break;
        }

        if (!equalTilesDown)
            compare--;
        else if (downSegment != 1 && downSegmentC == 1)
            equalTilesDown = false;

        if (compare > 1 && equalTiles)
        {
            switch (downSegment)
            {
            case 1:
                colRight++;
                break;
            case 2:
                rowDown++;
                *lieDown = true;
                break;
            case 3:
                colRight--;
                break;
            }
            break;
        }
        else if (compare > 3 && !equalTiles)
        {
            x = (equalTilesDown) ? 1 : 2;
            *lieDown = (downSegment % 2 == 1);

            switch (downSegment)
            {
            case 1:
                *col += x;
                colRight += x + 1;
                break;
            case 2:
                *row += x;
                rowDown += x + 1;
                break;
            case 3:
                *col -= x;
                colRight -= x + 1;
                break;
            }
        }
        else
        {
            colRight--;
            rowDown += (downSegment == 1) ? 1 : -1;

            while (boardCellsOccupied[rowDown][colRight])
                if (downSegment == 1)
                    rowDown++;
                else
                    colRight--;

            downSegment++;
            downSegmentC = 0;
            continue;
        }
        break;
    }
}

bool Dominoes::convertIntToCard(bool newGame, int numberPlayers, int cardNumber)
{
    int prevMaxPoints, x = 0, nCards = 7, maxPoints = 0, level = 0;

    // To Get How Many Points are in Each Side of the Card
    for (x = 0; x <= cardNumber; x++)
        if (x == maxPoints)
        {
            prevMaxPoints = maxPoints;
            maxPoints += nCards - level; // Maximum Number of Points for the Next Triangular Numbers Level
            level++;                     // Increment Triangular Numbers Level
        }

    sideA = level - 1;
    sideB = cardNumber - prevMaxPoints + level - 1;

    if (numberPlayers == 3 && cardNumber == 0)
        sideA = sideB = 6;

    // For Testing
    /*
    cout << sideA << ' ' << sideB << ' ' << cardNumber;
    pressEnterToCont();
    */

    return newGame && sideA == 6 && sideB == 6;
}

void Player::addCards(bool startsGame, vector<int> *shuffledCards, int numberPlayers, int cardsPerPlayer)
{
    int x, cardNumber, y = 0;
    Dominoes domino;

    cards.resize(cardsPerPlayer);
    cardsPlayed.resize(cardsPerPlayer);

    for (x = 0; x < cardsPerPlayer; x++)
    {
        cardNumber = (*shuffledCards)[x];
        domino.convertIntToCard(false, numberPlayers, cardNumber);
        cards[y] = domino;
        cardsPlayed[y] = false;
        y++;
    }

    if (numberPlayers == 3)
        cout << "\nPlayer " << nickname << " deck:\n";
    else if (numberPlayers == 4)
        cout << "\nTeam " << nickname << ", Player " << teamNumber << " deck:\n";

    if (startsGame)
        cout << "- Starts the Game\n";

    for (x = 0; x < cards.size(); x++)
        cout << "\n\t(" << x + 1 << ")"
             << " -> [" << cards[x].sideA << "|" << cards[x].sideB << "]";
}

bool Player::canPlay(int cardUp, int cardDown)
{
    int x, sideA, sideB;

    for (x = 0; x < cardsPlayed.size(); x++)
    {
        sideA = cards[x].sideA;
        sideB = cards[x].sideB;

        if (!cardsPlayed[x])
            if (sideA == cardUp || sideA == cardDown || sideB == cardUp || sideB == cardDown)
                return true;
    }
    return false;
}

void Player::playCard(Board *dominoesBoard, int cardsPerPlayer, int sixSixPosition)
{
    bool canPlay;
    int numberCards, cardNumber, sideA, sideB, cardUp, cardDown;
    Dominoes domino;

    if ((*dominoesBoard).numberCardsPlayed == 0)
        cout << "\n\tPlayer " << nickname << " started the game!";

    if (sixSixPosition != -1)
    {
        cardsPlayed[sixSixPosition] = true;
        domino.sideA = domino.sideB = 6;
    }
    else if ((*dominoesBoard).numberCardsPlayed == 0)
    {
        cardNumber = intQuestion("Select a Card from your Deck", 1, cardsPerPlayer);
        domino = cards[cardNumber - 1];
    }
    else
    {
        cardUp = (*dominoesBoard).cardUp;
        cardDown = (*dominoesBoard).cardDown;

        do
        {
            cardNumber = intQuestion("Select an Available Card from your Deck", 1, cardsPerPlayer);
            domino = cards[cardNumber - 1];
            sideA = domino.sideA;
            sideB = domino.sideB;

            if (!cardsPlayed[cardNumber - 1] && (sideA == cardUp || sideA == cardDown || sideB == cardUp || sideB == cardDown))
                break;
            else
                cout << "\t- Cannot Play Card of Position " << cardNumber;
        } while (true);
    }

    if (sixSixPosition == -1)
        cardsPlayed[cardNumber - 1] = true;

    (*dominoesBoard).addCard(&domino);
}

// Method that Counts Player Points from Cards
int Player::countPointsLeft()
{
    int x, cardsPointsLeft = 0;

    for (x = 0; x < cardsPlayed.size(); x++)
        if (!cardsPlayed[x])
            cardsPointsLeft += cards[x].sideA + cards[x].sideB;

    return cardsPointsLeft;
}

// Method that Prints Player's Deck Cards Left
void Player::printCardsLeft(int numberPlayers)
{
    if (numberPlayers == 3)
        cout << "\nPlayer " << nickname << " Cards Left:\n";
    else if (numberPlayers == 4)
        cout << "\nTeam " << nickname << ", Player " << teamNumber << " Cards Left:\n";

    for (int x = 0, y = 1; x < cardsPlayed.size(); x++)
        if (!cardsPlayed[x])
            cout << "\n\t(" << y++ << ")"
                 << " -> [" << cards[x].sideA << "|" << cards[x].sideB << "]";
}

// Method that Prints the Nickname of Player that Won the Round
void Player::playerWinRound(bool teams, int points)
{
    if (!teams)
        cout << "\nPlayer " << nickname << " Won the Round!";
    else
        cout << "\nTeam " << nickname << " Won the Round!";
    cout << "\n- Points: " << points;

    pressEnterToCont();
}

// Method that Prints Random Numbers in Background when Someone Wins the Game
void Player::playerWinGame(Board *dominoesBoard)
{
    random_device rd;

    int c = 0, lines = 3, numberTabs = 3;
    int x, y, l, tab, range, temp, sizeMessage, maxNumbersPerRow;
    string message, separator;
    vector<bool> shuffledBool;

    tab = 8;
    message = ((*dominoesBoard).teams) ? "Team " : "Player ";
    message.append(nickname);
    message.append(" Won the Game!");

    sizeMessage = message.size() + tab * 2 * numberTabs;
    if (sizeMessage % tab != 2)
        sizeMessage = (sizeMessage / tab) * tab + 1;

    separator = string(sizeMessage, '-');
    maxNumbersPerRow = (sizeMessage + tab - 1) / tab;

    range = 2 * maxNumbersPerRow * lines;
    do
    {
        shuffledBool.clear();

        for (x = 0; x < range; x++)
        {
            uniform_int_distribution<int> dist(0, 1);
            shuffledBool.push_back(dist(rd));
        }

        system("cls");

        x = 0;
        for (l = 0; l < 2 * lines; l++)
        {
            if (l == lines)
                cout << separator << "\n"
                     << string(tab * numberTabs, ' ') << message << "\n"
                     << separator << "\n\n";

            for (y = 0; y < maxNumbersPerRow; y++)
                if (shuffledBool[x + y])
                    cout << "*\t";
                else
                    cout << "\t";

            cout << "\n\n";
            x += y;
        }

        c++;
        std::this_thread::sleep_for(std::chrono::milliseconds(150));
    } while (c < 100);
}