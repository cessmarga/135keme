#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>

#define N 9

using namespace std;

struct parameters{
     int num;
     int row;
     int col;
     int check[N][N];
};

int sudoku[N][N];
int ins, val, check_row, check_col, check_grid;
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

void CheckRow(parameters par) {

    for (int col = 0; col <= 9; col++) {
        if(col < 9){
           if (par.check[par.row][col] == par.num) {
               check_row = 0; //there exists two of the same number
           }
        } else if(col == 9)
            check_row = 1;
    }
}

void CheckCol(parameters par) {
     cout << "LSG_col\n";
    for (int row = 0; row <= 9; row++) {
        if(row < 9){
           if (par.check[row][par.col] == par.num) {
               check_col = 0; //there exists two of the same number
           }
      }else if(row == 9)
            check_col = 1;
    }
}

void CheckGrid(parameters par) {
     cout << "LSG_grid\n";
    int nr, xr;
    int nc, xc;

    if (par.row < 3) {
        nr = 0;
        xr = 2;
   } else if (par.row < 6) {
        nr = 3;
        xr = 5;
    } else {
        nr = 6;
        xr = 8;
    }

    if (par.col < 3) {
        nc = 0;
        xc = 2;
   } else if (par.col < 6) {
        nc = 3;
        xc = 5;
    } else {
        nc = 6;
        xc = 8;
    }

    for(int i = nr; i <= xr; i++) {
        for(int j = nc; j <= xc; j++) {
            if (par.check[i][j] == par.num) {
                check_grid = 0; //there exists two of the same number
            }
        }
    }
    check_grid = 1;
}

bool Solve(int puzzle[N][N]) {
    EmptyCell(puzzle);
    if (x[0] == 9 && x[1] == 9) {
        copy(&puzzle[0][0], &puzzle[0][0]+N*N,&sudoku[0][0]);
        return true;
    }
    int row = x[0];
    int col = x[1];
    for (val = 1; val <= 9; val++) {
        cout << "Doing Row " << row << ", Column " << col << " with #" << val << ".\n";

        parameters par = {val, row, col, **puzzle};
        thread t1(CheckRow, par);
        thread t2(CheckCol, par);
        thread t3(CheckGrid, par);
        t1.join();
        t2.join();
        t3.join();

        if ((check_row == 0) | (check_col == 0) | (check_grid == 0)) {
            continue;
       } else {
            puzzle[row][col] = val;
            if(Solve(puzzle)) {
                copy(&puzzle[0][0], &puzzle[0][0]+N*N,&sudoku[0][0]);
                return true;
            }
            puzzle[row][col] = 0;
        }
        cout << "Hello\n";
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
            clock_t soltime;
            soltime = clock();

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
            }

            cout << "Time to solve (in seconds): " << float( clock() - soltime)/CLOCKS_PER_SEC << "\n\n";
            exit(0);
        }
    }

    return 0;
}
