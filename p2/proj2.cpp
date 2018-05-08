/**
 * @file proj2.cpp
 *
 * @brief
 *      This program will ask for a string x and determine the optimal cuts
 *      of that string to produce valid palindromes of its substrings.
 *      After finding this, the program will output these newly cut
 *      palindrome substrings to stardard output. This implementation uses
 *      dynamic programming to achieve this.
 *
 * @author Athit Vue
 * @date 03/09/2018
 */
#include <iostream>
#include <limits.h> // For INT_MAX
#include <string.h> // For substr()
#include <vector>   // For matrix
#include <iomanip>
#define DEBUG 1     // Debugger: use 1 for dubugging
using namespace std;

// Prototypes
void fill_P_table(vector<vector<bool> > &P, string &x);
void fill_C_and_B_table(vector< vector<bool> > &P, vector<int> &C,
   vector<int> &B, string &x);
void backtrack(vector<int> &C, vector<int> &B, string &x, int index);
void debug_P(vector< vector<bool> > &P, string x);

/**
 * int main
 *
 * This function is the main function. This function will read in a string
 * and: 1. Fill in the P matrix, 2. Fill in the C table, 3. While filling
 * the C table, fill in the B table, 4. Call the backtracking function to
 * print out the palindromes that have been cut.
 *
 * @return 0 The program executed without an error.
 * @return 1 The program returned with an error status 1.
 */
int main(){
  string x;

  // Read in a string from standard input
  cin >> x;

  // Get the size of string x
  int x_size = x.size();

  ///////// Create the matrix that will hold P, C, and B tables ///////
  // 1. P[i, j]. True if i = j; is palinedrome. Nested two-dem vectors.
  vector <vector <bool> > P(x_size, vector <bool>(x_size));
  // 2. C[i]: number of cuts in the best palindrome partition from 0...i of x.
  vector <int> C(x_size);
  // 3. B[i]: will contain the backtracking needed to print the palindromes.
  vector <int> B(x_size);
  ////////////////////////////////////////////////////////////////////

  // 1. Fill in the P table.
  fill_P_table(P, x);

  // Check the table for correctness.
  debug_P(P, x);

  // 2. Fill in the C table, and 3. Fill in the B table.
  fill_C_and_B_table(P, C, B, x);

  // Check the cut for correctness.
  if (DEBUG){
    cout << endl;
    cout << "DEBUG:: minimum cuts is: " << C[x_size-1] << endl;
    cout << endl;
    cout << "Solution is:" << endl;
  }

  // 4. Call the backtracking function to print the cutted palindromes.
  int start_index = x_size - 1;
  backtrack(C, B, x, start_index);
}

/**
 * void fill_P_table
 *
 * This function will fill in the P table with true:1 or false:0 while checking
 * these conditions:
 *   P[i][j] = {
 *                a_i = a_j and P[i+1, j-1]    if i < j - 1,
 *                a_i = a_j                    if i = j - 1,
 *                True                         if i = j
 *             }
 *
 * @param P The address to vector P.
 * @param X The address to the inputted string x.
 */
void fill_P_table(vector< vector<bool> > &P, string &x){
  // Get the size of the input string x
  int x_size = (int) x.size();

  // Every substring with length of one is a palindrome.
  for (int row = 0; row < x_size; row++){
    for (int col = 0; col < x_size; col++){
      if (row == col){
        P[row][col] = true;
      }
    }
  }

  // For palindrome with more than one character
  for (int j = 0; j < x_size; j++){
    for (int i = 0; i < x_size; i++){
      if ( i == (j-1)){
        P[i][j] = (x[i] == x[j]);
      }
      else if (i < (j-1)){
        P[i][j] = ((x[i] == x[j]) && (P[i+1][j-1]));
      }
    }
  }
}

/**
 * void fill_C_and_B_table
 *
 * This function fills in the C and B tables. The B table is filled while
 * filling the C table; it will contain the index k for which is the best
 * choice of C[i]. The C table is filled in with the following
 * conditions:
 *    C[i] = {
 *              min_k[0,i-1]{C[k]+1: P[k+1,i] = TRUE}    if i > 0,
 *              0                                        if i = 0
 *           }
 *
 * @param P The address to vector P.
 * @param C The address to vector C.
 * @param B The address to vector B.
 * @param x The address to string x.
 */
void fill_C_and_B_table(vector< vector<bool> > &P, vector<int> &C,
  vector<int> &B, string &x){
  int x_size = x.size();

  // For string x[i...j]...
  for (int i = 0; i < x_size; i++){
    // Take care of the base case.
    if (i == 0){
      C[i] = 0; // If i = 0, then C[0] = 0;
      B[i] = 0; // Thus B[0] = 0.
      // Go to next iteration.
      continue;
    }
    // If string x[i...j] is palindrome, then c[i] is 0; cut = 0.
    // B[i] will also be 0.
    if (P[0][i] == true){
      C[i] = 0;
      B[i] = 0;
      // Continue to next iteration.
      continue;
    }

    // To keep track of the index where it is cut.
    int cut_index = 0;
    // Initialize our min to infinity, used for comparison later
    int min = INT_MAX;

    // Make a cut at every possible location from i to j.
    // Get the min cuts and set the cut index.
    for (int k = 0; k < i; k++){
      if ((C[k] + 1 < min) && (P[k+1][i])){
        min = C[k] + 1;
        cut_index = k;
      }
    }

    C[i] = min; // set the min cut to it's index.

    B[i] = cut_index; // Set the cut index.
  }

  // Check the cuts
  if (DEBUG){
    cout << endl;
    cout << "DEBUG:: the min cuts are:" << endl;
    int count_1 = 0;
    for (int i = 0; i < (int) P.size(); i++){
      count_1++;
      if (count_1 == 1){
        cout << " Index: 0" << i << " ";
      }
      else {
        if (i < 10){
          cout << "0" << i << " ";
        }
        else{
          cout << i << " ";
        }
      }
    }
    cout << endl;
    int count = 0;
    for (int i = 0; i < (int) x.size(); i++){
      count++;
      if (count == 1){
        cout << " Str_X:  " << x[i] << " ";
      }
      else {
        cout << " " << x[i] << " ";
      }
    }
    cout << endl;
    int count_2 = 0;
    for (int i = 0; i < (int) C.size(); i++){
      count_2++;
      if (count_2 == 1){
        cout << " C[i] : 0" << C[i] << " ";
      }
      else {
        if (C[i] < 10){
          cout << "0" << C[i] << " ";
        }
        else{
          cout << C[i] << " ";
        }
      }
    }
    cout << endl;
    int count_3 = 0;
    for (int i = 0; i < (int) B.size(); i++){
      count_3++;
      if (count_3 == 1){
        cout << " B[i] : 0" << B[i] << " ";
      }
      else {
        if (C[i] < 10){
          cout << "0" << B[i] << " ";
        }
        else{
          cout << B[i] << " ";
        }
      }
    }
    cout << endl;
  }
}

/**
 * void backtrack
 *
 * A recursive function to the solution to find print out the palindromes
 * starting from where the cuts were made to the next cut (if anymore).
 *
 * @param C The address to vector C.
 * @param B The address to vector B.
 * @param x The address to string x.
 * @param index The start index that of string x size - 1.
 */
void backtrack(vector<int> &C, vector<int> &B, string &x, int index){
  // If cut is 1 or bigger, than we need to recurse where the cuts are while
  // printing out each palindrome in the recursion.
  if (C[index] > 0) {
    // recursively call backtrack to get each palindrome where cut # > 0;
    backtrack(C, B, x, B[index]);

    // When we are here, we have reached the end of the cut and need to attach
    // each substring of the cut together before we return to the stack.
    // Substr(posIndex, endIndex) will copy specified position index to
    // end index of a string.
    cout << x.substr(B[index] + 1, index - B[index]) << endl;

    if (DEBUG){
      cout << "Done with cut #" << C[index] << endl;
      cout << "Recursively returning!" <<  endl;
    }
  }
  else {
    // Print out a copy of substring from position 0 to index+1
    cout << x.substr(0, index + 1) << endl;

    if (DEBUG){
      cout << "Done with cut #" << C[index] << endl;
    }
  }
}

/**
 * void debug_P
 *
 * This is a helper function used for debugging the P table. It prints
 * the P table to standard output.
 *
 * @param P The address to vector P.
 * @param x The address to string x.
 */
void debug_P(vector< vector<bool> > &P, string x){
  int x_size = x.size();

  if (DEBUG){
    cout << "DEBUG:: matrix P contains: " << endl;
    // Print each char of string x as the column.
    int count = 0;
    for (int col = 0; col < x_size; col++){
      count++;
      if (count == 1){
        cout << "    " << x[col] << " ";
      }
      else{
        cout << x[col] << " ";
      }
    }
    cout << endl;
    for(int row = 0; row < (int) P.size(); row++){
      // Print each char of string x as the row.
      cout << " " << x[row] << "| ";
      for(int col = 0; col < (int) P.size(); col++){
        cout << P[row][col] << " ";
      }
      cout << endl;
    }
  }
}
