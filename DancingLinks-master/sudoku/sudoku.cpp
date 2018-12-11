/* sudoku.cpp : Solve sudoku using Dancing Links
 *
 * Author: Raziman T V
 *
 * License:
 * You may use this document for whatever purpose you desire, as long as the
 * following conditions are satisfied:
 * 1) You do not use it in a way that suggests that I endorse the use.
 * 2) If you redistribute this code, I would be grateful if you credit me as
 * the author of the code. This is not a requirement.
 * 3) If this code is modified and redistributed with credit to me as the
 * original author, the code should either contain a link to the source
 * where the unaltered code can be found, or state explicitly that the code
 * has been altered significantly since.
 */

#include "../DancingLinks/DancingLinks.h"
#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>


using namespace std;
int row = 0, column = 0;
int ins; //number of hints (filled out cells)
int board[9][9];
const int rows = 9 * 9 * 9;
const int columns = 9 * 9 * 4;
int ar1[rows], ar2[columns];
string comm;

void Display() {
    system("clear");
    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 9; j++) {
            if (board[i][j] == 0) {
                cout << "_";
            } else {
                cout << board[i][j];
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
    int i, j;
    if (row < 0 || row > 8 || column < 0 || column > 8) {
        cout << "Invalid Cell Location\n";
        return;
    }
    while(1) {
        cout << "Number: ";
        cin >> board[row][column];
        if (board[row][column] > 0 && board[row][column] < 10) {
            ins++;
	for (int i = 0; i < 9; i++) {
	        for (int j = 0; j < 9; j++) {
			for (int k = 0; k < 9; k++) {
			  // Other numbers can't end up in the same cell now
			  ar1[i * 81 + j * 9 + k] = 0;

			  // This number can't appear in another column in the same row
			  ar1[i * 81 + k * 9 + board[i][j] - 1] = 0;

			  // This number can't appear in another row in the same column
			  ar1[k * 81 + j * 9 + board[i][j] - 1] = 0;

			  // This number can't appear in another cell in the same box
			  ar1[((i / 3) * 3 + k / 3) * 81 + ((j / 3) * 3 + k % 3) * 9 +
			      board[i][j] - 1] = 0;
			}

			// Cell constraint satisfied
			ar2[i * 9 + j] = 0;

			// Row constraint satisfied
			ar2[81 + i * 9 + board[i][j] - 1] = 0;

			// Colum constraint satisfied
			ar2[162 + j * 9 + board[i][j] - 1] = 0;

			// Box constraint satisfied
			ar2[243 + (i / 3) * 27 + (j / 3) * 9 + board[i][j] - 1] = 0;
		      }

		    return;
		}
	cout << "Invalid Number. Try again\n";
    }

void Delete(int row, int column) {
    if (row < 0 || row > 8 || column < 0 || column > 8) {
        cout << "Invalid Cell Location";
        return;
    }
    board[row][column] = 0;
    return;
}



int main() {
	memset(ar1, 1, sizeof ar1);
	memset(ar2, 1, sizeof ar2);

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

            int invar1[rows];
		  int cum1 = 0, cum2 = 0;
		  for (int i = 0; i < rows; i++) {
		    if (ar1[i]) {
		      ar1[i] = cum1;
		      invar1[cum1] = i;
		      cum1++;
		    } else
		      ar1[i] = -1;
		  }
		  for (int i = 0; i < columns; i++) {
		    if (ar2[i])
		      ar2[i] = cum2++;
		    else
		      ar2[i] = -1;
		  }

		  // The number of rows and columns we really need to deal with are just cum1
		  // and
		  //  cum2 respectively
		  int R = cum1, C1 = cum2, C2 = 0, C = C1 + C2;

		  // Matrix to store the constraint adjacency matrix
		  char *mat;
		  mat = new (nothrow) char[R * C];
		  if (mat == NULL) {
		    printf("Memory allocation error\n");
		    return 1;
		  }
		  memset(mat, 0, R * C);

		  // For non-forbidden rows and columns, fill in elements in the matrix
		  for (int i = 0; i < 9; i++) {
		    for (int j = 0; j < 9; j++) {
		      for (int k = 0; k < 9; k++) {
			int r = i * 81 + j * 9 + k;
			if (ar1[r] == -1) continue;
			int c1 = i * 9 + j, c2 = 81 + i * 9 + k, c3 = 162 + j * 9 + k,
			    c4 = 243 + (i / 3) * 27 + (j / 3) * 9 + k;
			if (ar2[c1] != -1) mat[ar1[r] * C + ar2[c1]] = 1;
			if (ar2[c2] != -1) mat[ar1[r] * C + ar2[c2]] = 1;
			if (ar2[c3] != -1) mat[ar1[r] * C + ar2[c3]] = 1;
			if (ar2[c4] != -1) mat[ar1[r] * C + ar2[c4]] = 1;
		      }
		    }
		  }

		  // Now just solve it with dancing links
		  // Note that we are asking it to find only one solution
		  DancingLinks N;
		  N.Create(R, C1, C2, mat);
		  std::vector<std::vector<int> > all;
		  std::vector<int> cur;
		  N.solve(all, cur, false);

		  if (all.size() == 1) {
		    // Fill in the board using chosen subsets
		    for (int i = 0; i < all[0].size(); i++) {
		      int x = invar1[all[0][i]];
		      board[x / 81][(x / 9) % 9] = x % 9 + 1;
		    }

		    for (int i = 0; i < 9; i++) {
		      for (int j = 0; j < 9; j++) {
			printf("%d ", board[i][j]);
		      }
		      printf("\n");
		    }
		  }
		  delete[] mat;

			    cout << "Time to solve (in seconds): " << float( clock() - soltime)/CLOCKS_PER_SEC << "\n\n";
			    exit(0);
			}
		    }



  return 0;
}
