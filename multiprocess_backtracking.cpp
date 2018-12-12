#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <sys/wait.h>
#include <string>
#include <ctime>

#define N 9

using namespace std;

int sudoku[N][N];
int ins; //number of hints (filled out cells)
int x[2]; //row and column from Empty Cell checking

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
                x[0] = i; //row
                x[1] = j; //column
                return;
            }
        }
    }
    //all spaces filled
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
    int nr, xr; //minimum and maximum row numbers
    int nc, xc; //minimum and maximum column numbers

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
                return false; //there exists two of the same number within the grid
            }
        }
    }
    return true;
}

bool CheckProc (int num, int row, int col, int puzzle[N][N]) {
    bool a, b, c;
    int pip1[2], pip2[2];

    if (pipe(pip1) == -1 || pipe(pip2) == -1) {
        cout << "Pipe Error!\n";
        exit(1);
    }

    pid_t pid1, pid2;

    pid1 = fork();

    if (pid1 == -1) {
      cout << "1: Fork Error!\n";
      exit(1);
    }

    if (pid1 == 0) {
        close(pip1[0]); //close reading

        a = CheckGrid(num, row, col, puzzle);
        write (pip1[1], &a, sizeof(a)); //receive value of CheckRow&&CheckCol

        close (pip1[1]); //close writing
        exit(0);
    } else {
        close(pip1[1]); //close writing
        read(pip1[0], &a, sizeof(a)); //receive value of CheckGrid
        close(pip1[0]);
        wait(NULL);
        pid2 = fork();

        if (pid2 == -1) {
          cout << "2: Fork Error!\n";
          exit(1);
        }

        if (pid2 == 0) {

            close(pip2[0]); //close reading

            b = CheckRow(num, row, puzzle);
            write (pip2[1], &b, sizeof(b)); //send value of CheckRow

            close(pip2[1]); //close writing
            exit(0);

        }

        close(pip2[1]); //close writing

        c = CheckCol(num, col, puzzle);

        read(pip2[0], &b, sizeof(b)); //receive value of CheckGrid
        close(pip2[0]); //close reading

        wait(NULL);
        if (a == false || b == false || c == false) {
            a = false;
        } else {
            a = true;
        }

        return a;
    }
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
    //    cout << "Checking #" << num << " for " << row << "," << col << "\n";
        if (CheckProc(num, row, col, puzzle) == false) {
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
    FILE *f;

    f = fopen("multi_back_data.txt", "a");

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
          //  cout << "time0: " << clock() << "\n";

            if (ins < 17) {
                cout << "Needs more hints in cells.\n";
                continue;
            }
            copy(&sudoku[0][0], &sudoku[0][0]+N*N,&puzzle[0][0]);
            if (Solve(puzzle) == true) {
                Display();
                float tim;
                tim = float(clock() - soltime)/CLOCKS_PER_SEC;
                cout << "Time to solve (in seconds): " << tim << "\n\n";
                fprintf(f, "%lf\n", tim);
                fclose(f);
                exit(0);
            } else {
                Display();
                cout << "No solution found!\n";
                fclose(f);
                exit(0);
            };
        }
    }
}
