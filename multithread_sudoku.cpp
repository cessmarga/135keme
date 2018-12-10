#include <iostream>
#include <cstdlib>
#include <string>
#include <thread>

#define N 9

using namespace std;

struct parameters{
     int row;
     int col;
     int board[N][N];
};

int sudoku[N][N];
int ins, val, S;
int x[2]; // row and column from empty cell checking

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

void CheckRow(void *par){
     parameters *data = (parameters *) par;
     int row = par->row;
     int col = par->col;

     for(int i = row; i < N; ++i){
          int row[10] = {0}; // to check if the number is already in the puzzle
          for(int j = col; j < N; ++j){
               int value = data->board[i][j];
               if(row[val] != 0)
                    return (void *) 1;
               else
                    row[val] = 1;
          }
     }

    return (void *) 1;
}

void CheckCol(void *par){
     parameters *data = (parameters *) par;
     int row = par->row;
     int col = par->col;

     for(int i = row; i < N; ++i){
          int row[10] = {0}; // to check if the number is already in the puzzle
          for(int j = col; j < N; ++j){
               int value = data->board[i][j];
               if(row[val] != 0)
                    return (void *) 1;
               else
                    row[val] = 1;
          }
     }

     return (void *) 1;
}

void subsquare(){


}

int main(){
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

            cout << "Time to solve (in seconds): " << float( clock() - soltime)/CLOCKS_PER_SEC << "\n\n";
            exit(0);
        }
    }

     return 0;
}
