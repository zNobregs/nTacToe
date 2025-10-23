#include <iostream>
#include <chrono>
#include "graphicsFramework.h"
#include <vector>
using namespace std;

const double TICK = 0.20;
const int SCREENWIDTH = 80;
const int SCREENHEIGHT = 25;
const unsigned char ESCAPE = 27;
const double CURSORBLINKSPEED = 0.5;
char piece(int &x);
const Color XCOLOR = Color::BrightMagenta;
const Color OCOLOR = Color::BrightCyan;

void drawBorder(char headers, char sides, Color color = Color::Default)
{
    string header(SCREENWIDTH, headers);
    for (int i = 0; i < SCREENHEIGHT; i++)
    {
        if(i == 0 || i == SCREENHEIGHT-1)
        {
            putStringAt(0, i, header, color);
        }
        for (int j = 0; j < SCREENWIDTH; j++)
        {
            if (j == 0 || j == SCREENWIDTH-1)
            {
                putCharAt(j, i, sides, color);
            }
        }
    }
    return;
}

void mainMenuControls()
{
    char c = 0;
    while (c != ESCAPE)  //frame loop
    {
        ;
    }
}

char piece(int &x)
{
    if (x == -1)
    {
        return ' ';
    }
    if (x == 0)
    {
        return 'X';
    }
    if (x == 1)
    {
        return 'O';
    }
}
char piece(bool &x)
{
    if (x == false)
    {
        return 'X';
    }
    if (x == true)
    {
        return 'O';
    }
}

int elementWiseSum(vector<vector<int>> matrix)
{
    int sum = 0;
    for (size_t i = 0; i < matrix.size(); i++)
    {
        for (size_t j = 0; j < matrix.size(); j++)
        {
            sum += matrix[i][j];
        }
    }
    return sum;
}

//Hadamard product
vector<vector<int>> elementWiseMulti(vector<vector<int>> &board, vector<vector<int>> &mask, int x, int y)
{
    int maskSize = static_cast<int>(mask.size());
    vector<vector<int>> resultant(maskSize, vector<int>(maskSize, 0));
    for (size_t i = 0; i < mask.size(); i++)
    {
        for (size_t j = 0; j < mask.size(); j++)
        {
            resultant[i][j] = board[i + y][j + x] * mask[i][j];
        }
    }
    return resultant;
}

bool checkTinyWin(vector<vector<int>> &board, vector<vector<int>> &mask, int x, int y)
{
    int winSize = static_cast<int>(mask.size());
    vector<vector<int>> resultant(winSize, vector<int>(winSize, 0));
    resultant = elementWiseMulti(board, mask, x, y);
    if(elementWiseSum(resultant) == static_cast<int>(resultant.size()) || elementWiseSum(resultant) == 0)
    {
        return true;                                                                               // this is a win condition
    }
    else
    {
        return false;                                                                              // did not find a win condition
    }
}

bool checkTinyWin(vector<vector<int>> &board, vector<int> &mask, int x, int y)
{
    int winSize = static_cast<int>(mask.size());
    vector<vector<int>> resultant(winSize, vector<int>(winSize, 0));
    resultant = elementWiseMulti(board, mask, x, y);
    if(elementWiseSum(resultant) == static_cast<int>(resultant.size()) || elementWiseSum(resultant) == 0)
    {
        return true;                                                                               // this is a win condition
    }
    else
    {
        return false;                                                                              // did not find a win condition
    }
}


void setWinMasks(vector<vector<int>> &diagonalPos, vector<vector<int>> &diagonalNeg, int winSize)
{
    int diagonalPosCount = 0;
    int diagonalNegCount = 0;

    for (int i = 0; i < winSize; i++)
    {
        for(int j = 0; j < winSize; j++)
        {
            if (i == j && diagonalPosCount < winSize)
            {
                diagonalPos[i][j] = 1;
                diagonalPosCount++;
            }
            if (i + j == winSize - 1 && diagonalNegCount < winSize)
            {
                diagonalNeg[i][j] = 1;
                diagonalNegCount++;
            }
        }
    }
}


bool checkWinConditions(vector<vector<int>> &board, vector<vector<int>> &diagonalPos, vector<vector<int>> &diagonalNeg, vector<int> &row, vector<vector<int>> &column)
{
    int boardSize = static_cast<int>(board.size());
    int winSize = static_cast<int>(diagonalPos.size());
    for (int i = 0; i < boardSize - winSize; i++)
    {
        for (int j = 0; j < boardSize - winSize; j++)
        {
            if(checkTinyWin(board, diagonalPos, j, i) +
               checkTinyWin(board, diagonalNeg, j, i) +
               checkTinyWin(board, row, j, i) +
               checkTinyWin(board, column, j, i) > 0
               )
            {
                return true;
            }
        }
    }
    return false;
}


void drawBoard(vector<vector<int>> &board, vector<vector<int>> &allBoardChars, int x, int y, bool drawGrid)
{
    if(drawGrid == false){
        for (int i = 0; i < board.size(); i++)
        {
            for (int j = 0; j < board[0].size(); j++)
            {
                if(board[j][i] == 0 || board[j][i] == 1)
                {
                    putCharAt(x + j, y + i, piece(board[j][i]));
                    allBoardChars[y+i][x+j] = piece(board[j][i]);
                }
            }
        }
    }
}



void placePiece(vector<vector<int>> &board, bool &playerTurn, int x, int y)
{
    board[y][x] = playerTurn;
}

void cursorBlink(double dt, auto &currentTime, auto &previousTime, vector<vector<int>> &allBoardChars, int x, int y, bool &playerTurn, int &blinkCounter)
{
    if (dt > CURSORBLINKSPEED)
    {
        if(blinkCounter % 2 == 0)
        {
            if(playerTurn == false)
            {
                putCharAt(x, y, '#', XCOLOR);
            }
            else
            {
                putCharAt(x, y, '#', OCOLOR);
            }
        }
        else
        {
            putCharAt(x, y, char(allBoardChars[y][x]), Color::Default);
        }
        previousTime = currentTime;
        blinkCounter++;
    }
}

void placeTile(vector<vector<int>> &board, vector<vector<int>> &allBoardChars, bool &playerTurn, int x, int y, int boardXOffset, int boardYOffset)
{
    if(board[y][x] != 0 && board[y][x] != 1)
    {
        board[y][x] = playerTurn;
        if(playerTurn)
        {
            allBoardChars[y+boardYOffset][x+boardXOffset] = 79;                                                              // O
        }
        else
        {
            allBoardChars[y+boardYOffset][x+boardXOffset] = 88;                                                              // X
        }

        putCharAt(x+boardXOffset, y + boardYOffset, piece(playerTurn), Color::Default);
        playerTurn = !playerTurn;
    }
}

void debugPrintBoard(vector<vector<int>> &board)
{
    string temp = "";
    for (int i = 0; i < 3; i++)
    {
        //temp = char(allBoardChars[3][3]);
        //putStringAt(3, 15+i, temp);
    }
}

void debugPrintAllChars(vector<vector<int>> &allBoardChars)
{
    string temp = "";
    for (int i = 0; i < SCREENHEIGHT; i++)
    {
        for(int j = 0; j < SCREENWIDTH; j++)
        {
            putCharAt(j, i, char(allBoardChars[i][j]));
        }
    }
}

void standardGameLoop(int boardSize, int winSize)
{
    vector<vector<int>> allBoardChars(SCREENHEIGHT, vector<int>(SCREENWIDTH, 32));                 // 2d array of all pixels ascii characters in the scree space
    vector<vector<int>> board(boardSize* boardSize, vector<int>(boardSize * boardSize, 3));
    vector<vector<int>> diagonalPos(winSize, vector<int>(winSize,0)), diagonalNeg(winSize, vector<int>(winSize,0));
    vector<int> row(winSize,1);                                                                   // {1,1,1}
    vector<vector<int>> column(winSize, vector<int>(1,1));                                               // {{1},{1},{1}}
    setWinMasks(diagonalPos, diagonalNeg, winSize);

    bool playerTurn = false;                                                                       //  false is X's and true is O's
    int boardXOffset = 3;
    int boardYOffset = 3;
    int xPos = 0;
    int yPos = 0;
    int prevXPos = 0;
    int prevYPos = 0;
    char c = 0;
    int blinkCounter = 0;

    std::chrono::duration<float> d;
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto previousTime = std::chrono::high_resolution_clock::now();
    double dt;

    drawBoard(board, allBoardChars, boardXOffset, boardYOffset, false);
    while (c != ESCAPE)  //frame loop
    {
        //input///////////////////////////////////
        c = readKey();

        //update//////////////////////////////////
        //putCharAt(xPos, yPos, ' '); //clear out in case moved
        if (c == 'w' && yPos > 0)
        {
            putCharAt(xPos + boardXOffset, yPos + boardYOffset, allBoardChars[yPos + boardYOffset][xPos+ boardXOffset], Color::Default);
            prevYPos = yPos;
            yPos--;
        }
        else if (c == 's' && yPos < boardSize - 1)
        {
            putCharAt(xPos + boardXOffset, yPos + boardYOffset, allBoardChars[yPos + boardYOffset][xPos+ boardXOffset], Color::Default);
            prevYPos = yPos;
            yPos++;
        }
        else if (c == 'a' && xPos > 0)
        {
            putCharAt(xPos + boardXOffset, yPos + boardYOffset, allBoardChars[yPos + boardYOffset][xPos+ boardXOffset], Color::Default);
            prevXPos = xPos;
            xPos--;
        }
        else if (c == 'd' && xPos < boardSize - 1)
        {
            putCharAt(xPos + boardXOffset, yPos + boardYOffset, allBoardChars[yPos + boardYOffset][xPos+ boardXOffset], Color::Default);
            prevXPos = xPos;
            xPos++;
        }
        else if(c == 'f')
        {
            placeTile(board, allBoardChars, playerTurn, xPos, yPos, boardXOffset, boardYOffset);

        }
        if(checkWinConditions(board, diagonalPos, diagonalNeg, row, column))
        {
                putStringAt(20, 15, "WIN!!!!", Color::Green);
        }
        else
        {
            drawBoard(diagonalPos, allBoardChars, 10, 5, false);
            drawBoard(diagonalNeg, allBoardChars, 14, 5, false);
            drawBoard(row, allBoardChars, 18, 5, false);
            drawBoard(column, allBoardChars, 22, 5, false);
            putStringAt(20, 14, "playing...", Color::Green);
        }
        /*
        //check win condition
        if(graphics.getCharAt(xPos,yPos) == '?')
        {
            graphics.putStringAt(5, 5, "You win!!", Color::Green);
        }
        */
        //putCharAt(prevXPos, prevYPos, allBoardChars[prevYPos][prevXPos], Color::Default);



        //draw//////////////////////////////////


        //sleep//////////////////////////////////


        currentTime = std::chrono::high_resolution_clock::now();
        d = currentTime - previousTime;
        dt = d.count();
        if(c != 'f')
        {
            cursorBlink(dt, currentTime, previousTime, allBoardChars, xPos + boardXOffset, yPos + boardYOffset, playerTurn, blinkCounter);
        }
    }
}

void debugPrintAscii()
{
    for (int i = 0; i < 500; i++)
    {
        cout << i << ": " << char(i) << endl;
    }
}

int main() {
    char c = 0;
    hideCursor();

    //drawBorder('#', '#', Color::White);
    standardGameLoop(5, 5);

    /*
    while (c != ESCAPE)  //frame loop
    {
        //input///////////////////////////////////
        c = graphics.readKey();

        //update//////////////////////////////////
        graphics.putCharAt(i,j, ' '); //clear out in case moved
        if (c == 'w' && i > 0)
        {
            i--;
        }
        else if (c == 's' && i < graphics.height())
        {
            i++;
        }
        else if (c == 'a' && j > 0)
        {
            j--;
        }
        else if (c == 'd' && j < graphics.width())
        {
            j++;
        }

        //check win condition
        if(graphics.getCharAt(i,j) == '?')
        {
            graphics.putStringAt(5, 5, "You win!!", Color::Green);
        }

        graphics.putCharAt(i,j, 'o', Color::Red);

        //draw//////////////////////////////////
        graphics.draw();

        //sleep//////////////////////////////////
        graphics.sleepMs(33);  //30 fps
    }
    */
    return 0;
}
