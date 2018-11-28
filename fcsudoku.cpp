#include <iostream>
#include <cstdlib>
#include <string>

#define N 9

using namespace std;

int sudoku[N][N];
int ins;
int x[2];

void Display() {
    system("clear");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (sudoku[i][j] == 0) {
                cout << "_";
            } else {
                cout << sudoku[i][j];
            }
            cout << "\t";
            if (j == 2 || j == 5) {
                cout << "\t";
            }
        }
        cout << "\n";
        if (i == 2 || i == 5 || i == 8) {
            cout << "\n\n";
        }
    }
}

void Insert(int row, int column) {
    if (row < 0 || row > 8 || column < 0 || column > 8) {
        cout << "Invalid Cell Location\n";
        return;
    }
    while(1) {
        cout << "Number: ";
        cin >> sudoku[row][column];
        if (sudoku[row][column] > 0 && sudoku[row][column] < 10) {
            ins++;
            return;
        }
        cout << "Invalid Number. Try again\n";
    }
}

void Delete(int row, int column) {
    if (row < 0 || row > 8 || column < 0 || column > 8) {
        cout << "Invalid Cell Location";
        return;
    }
    sudoku[row][column] = 0;
    return;
}

void EmptyCell(int check[N][N]) {
    for(int i = 0; i < 9; i++) {
        for(int j = 0; j < 9; j++) {
            if (check[i][j] == 0) {
                x[0] = i;
                x[1] = j;
                return;
            }
        }
    }
    x[0] = 9;
    x[1] = 9;
}

bool CheckRow(int val, int S, int check[N][N]) {
    for (int col = 0; col < 9; col++) {
        if (check[S][col] == val) {
            return false; //there exists two of the same number
        }
    }
    return true;
}

bool CheckCol(int val, int S, int check[N][N]) {
    for (int row = 0; row < 9; row++) {
        if (check[row][S] == val) {
            return false; //there exists two of the same number
        }
    }
    return true;
}

bool CheckGrid(int val, int row, int col, int check[N][N]) {
    int nr, xr;
    int nc, xc;

    if (row < 3) {
        nr = 0;
        xr = 2;
    } else if (row < 6) {
        nr = 3;
        xr = 5;
    } else {
        nr = 6;
        xr = 8;
    }

    if (col < 3) {
        nc = 0;
        xc = 2;
    } else if (col < 6) {
        nc = 3;
        xc = 5;
    } else {
        nc = 6;
        xc = 8;
    }

    for(int i = nr; i <= xr; i++) {
        for(int j = nc; j <= xc; j++) {
            if (check[i][j] == val) {
                return false;
            }
        }
    }
    return true;
}

bool Solve(int puzzle[N][N]) {
    EmptyCell(puzzle);
    if (x[0] == 9 && x[1] == 9) {
        copy(&puzzle[0][0], &puzzle[0][0]+N*N,&sudoku[0][0]);
        return true;
    }
    int row = x[0];
    int col = x[1];
    for (int num = 1; num <= 9; num++) {
        cout << "Doing Row " << row << ", Column " << col << " with #" << num << ".\n";
        if (CheckRow(num, row, puzzle) == false) {
            continue;
        } else if (CheckCol(num, col, puzzle) == false) {
            continue;
        } else if (CheckGrid(num, row, col, puzzle) == false) {
            continue;
        } else {
            puzzle[row][col] = num;
            if(Solve(puzzle)) {
                copy(&puzzle[0][0], &puzzle[0][0]+N*N,&sudoku[0][0]);
                return true;
            }
            puzzle[row][col] = 0;
        }
    }
    return false;
}

int main() {
    int row = 0, column = 0;
    string comm;
    int puzzle[N][N];

    while(ins < 81) {
        Display();
        cout << "Would you like to insert (INS), delete (DEL), solve(SOLVE), or exit (EXIT)? ";
        cin >> comm;

        if (comm == "INS") {
            cout << "Row and Column (separated by space)? ";
            cin >> row >> column;
            Insert(row-1, column-1);
        } else if (comm == "DEL") {
            cout << "Row and Column (separated by space)? ";
            cin >> row >> column;
            Delete(row-1, column-1);
        } else if (comm == "EXIT") {
            system("clear");
            exit(0);
        } else if (comm == "SOLVE") {
            if (ins < 17) {
                cout << "Needs more hints in cells.\n";
                continue;
            }
            copy(&sudoku[0][0], &sudoku[0][0]+N*N,&puzzle[0][0]);
            if (Solve(puzzle) == true) {
                Display();
            } else {
                Display();
                cout << "No solution found!\n";
            };
        //    cout << "SOLVE!!!!";
            exit(0);
        }
    }

    return 0;
}
