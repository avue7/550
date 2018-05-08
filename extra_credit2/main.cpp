/**
 * @file main.cpp
 *
 * @brief
 *    Bob Johnson has some land to sell. He owns a land that is 1 x N square
 *    miles. He wants to obtain the maximium revenue for the land that he owns
 *    by breaking the land into pieces. The price of each piece will be
 *    specified in the standard input as well as the size N of the land. This
 *    program will take these things into consideration and use dynamic
 *    programming to solve this problem. We will try to achieve a time of
 *    O(n^2) for this problem.
 *
 * @author Athit Vue
 * @date 03/19/2018
 */
#include <iostream>
#define DEBUG 0
using namespace std;

// Prototyping
int calculate_max(int * current_market_prices, int * max_revenue, const int SIZE);

/**
 * int main
 *
 * This is the main function. It will dynamically allocated two arrays needed
 * for storing the current market prices of each n size of land, from standard
 * input, and the max revenue for each n size land. It will then call the
 * calculate_max function to get the total max revenue for the n size land
 * that Bob Johnson wants to sell.
 *
 * @return 0 The program executed without an error.
 * @return 1 The program exited with an error.
 */
int main(){
  // N, describes the length of 1 x N square miles of land
  int n;

  // Get the n square miles from standard input
  cin >> n;

  const int SIZE = n;

  // Allocate memory for current market prices.
  int * current_market_prices;
  current_market_prices = new int[SIZE];

  // Need to read in the current market prices for 1x1, 1x2, 1x3, ..., 1xN
  // square miles of land.
  for (int i = 0; i < SIZE; i++){
    cin >> current_market_prices[i];
  }

  // Check correctness of inputs...
  if (DEBUG){
    cout << "DEBUG:: current market prices are:" << endl;
    for(int i = 0; i < SIZE; i++){
      cout << "1 x <" << i+1 << "> :" << current_market_prices[i];
      cout << " " << endl;
    }
  }

  // Allocate memory to store the maximum revenue that Bob Johnson could
  // obtain. Let max_revenue[i] be the max revenue for land of 1 x i.
  int * max_revenue;
  max_revenue = new int[SIZE];

  // Store the total max revenue
  int total_max_revenue;
  total_max_revenue = calculate_max(current_market_prices, max_revenue, SIZE);

  // Print out the max revenue found
  cout << total_max_revenue << endl;

  // Delete all allocated resources
  delete[] current_market_prices;
  delete[] max_revenue;

  return 0;
}

/**
 * int calculate_max
 *
 * This function calculates the total maximum revenue for the size of land
 * of 1 x n.
 * Recurrence relation:
 *  max_revenue[i] = Price[i]
 *  max_revenue[i] = max(Price_i, max_revenue[j] + max_revenue[i-j-1]),
 *  where 0 <= j < i, thus our index starts from 0 to n-1
 *
 * @param current_market_prices Pointer to current_market_prices.
 * @param max_revenue Pointer to max_revenue.
 * @param SIZE The size of the land.
 * @return max_revenue[SIZE] The total max revenue found for N size.
 */
int calculate_max(int* current_market_prices, int* max_revenue, const int SIZE){
  for (int i = 0; i < SIZE; i++){
    // We want to start recurrence at index 1.
    if (i == 0){
      max_revenue[0] = current_market_prices[0];
      continue;
    }

    // Need to store the max price found
    int max_found = current_market_prices[i];

    for (int j = 0; j < i; j++){
      max_found = max(max_found, max_revenue[j] + max_revenue[i-j-1]);

      /* if (DEBUG){
      cout << "DEBUG:: max_found is: " << max_found << endl;
      cout << "DEBUG:: max_revenue" << max_revenue[j] << endl;
      cout << "DEBUG:: max_revenue [i-j]" << max_revenue[i-j] << endl;
      }
      */
    }

    // Check each max_found for correctness
    if (DEBUG){
      cout << "DEBUG:: max_found is: " << max_found << endl;
    }

    max_revenue[i] = max_found;
  }
  // Return the total max revenue
  return (int) max_revenue[SIZE-1];
}
