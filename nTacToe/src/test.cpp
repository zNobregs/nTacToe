#include <iostream>
#include <vector>
#include <string>

using namespace std;


void displayBoard(vector<vector<int>> &board)
{
    int n = static_cast<int>(board.size());
    // display board;
    for (int i = 0; i < n; i++)
    {
        for(int j = 0; j < n; j++)
        {
            cout << board[i][j] << " ";
        }
        cout <<  endl;
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

void setWinMasks(vector<vector<int>> &diagonalPos, vector<vector<int>> &diagonalNeg, vector<vector<int>> &row, vector<vector<int>> &column, int winSize)
{
    int diagonalPosCount = 0;
    int diagonalNegCount = 0;
    int rowCount = 0;
    int columnCount = 0;

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
            if(j == 0 && columnCount < winSize)
            {
                column[i][j] = 1;
                columnCount++;
            }
            if(i == 0 && rowCount < winSize)
            {
                row[i][j] = 1;
                rowCount++;
            }
        }
    }
}


bool checkWinConditions(vector<vector<int>> &board, vector<vector<int>> &diagonalPos, vector<vector<int>> &diagonalNeg, vector<vector<int>> &row, vector<vector<int>> &column)
{
    size_t boardSize = board.size();
    int winSize = static_cast<int>(diagonalPos.size());
    for (size_t i = 0; i < boardSize - winSize; i++)
    {
        for (size_t j = 0; j < boardSize - winSize; j++)
        {
            if(checkTinyWin(board, diagonalPos, j, i),
               checkTinyWin(board, diagonalNeg, j, i),
               checkTinyWin(board, row, j, i),
               checkTinyWin(board, column, j, i)
               )
            {
                return true;
            }
        }
    }
    return false;
}
int main()
{
    int n;
    int winSize;
    cin >> n;
    cin >> winSize;
    vector<vector<int>> board(n, vector<int>(n,3));
    // these n can be any length to allow for a board of size 7x7 but a win condition of 4 in a row
    vector<vector<int>> diagonalPos(winSize, vector<int>(winSize,0)), diagonalNeg(winSize, vector<int>(winSize,0)), row(winSize, vector<int>(winSize,0)), column(winSize, vector<int>(winSize,0));
    setWinMasks(diagonalPos, diagonalNeg, row, column, winSize);
    if(checkWinConditions(board, diagonalPos, diagonalNeg, row, column))
    {
        cout << "has win!" << endl;
        displayBoard(board);
    }
    else
    {
        cout << "No win!" << endl;
        displayBoard(board);
    }
    board = {{3,0,0,3,3},
            {0,3,1,3,3},
            {3,3,1,1,3},
            {0,3,1,3,3},
            {3,0,3,3,3}};
    if(checkWinConditions(board, diagonalPos, diagonalNeg, row, column))
    {
        cout << "has win!" << endl;
        displayBoard(board);
    }
    else
    {
        cout << "No win!" << endl;
        displayBoard(board);
    }

    /*
    cout << "board: " << endl;
    displayBoard(board);
    cout << "Positive Diagonal: " << endl;
    displayBoard(diagonalPos);
    cout << "Negative Diagonal: " << endl;
    displayBoard(diagonalNeg);
    cout << "Row: " << endl;
    displayBoard(row);
    cout << "Column: " << endl;
    displayBoard(column);
    */

    return 0;
}


