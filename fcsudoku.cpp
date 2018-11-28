#include <iostream>
#include <cstdlib>
#include <string>

using namespace std;

int sudoku[9][9];
int ins;

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

void Solve() {

}

int main() {
    int row = 0, column = 0;
    string comm;

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
                cout << "Needs more hints in cell.\n";
                continue;
            }
            cout << "SOLVE!!!!";
            exit(0);
        }
    }

    return 0;
}
