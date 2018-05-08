/**
 * @file
 *
 * @brief
 *      This program implements the SAIS algorithm that calculates Suffix
 *      Array for a given string T of length n in linear time, O(n). It will
 *      read the text T from an input file, then calculate Suffix Array for
 *      T. This program is part 2 and it fully implements the SAIS algorithm.
 *      This program will finish calculating the Suffix Array in linear time.
 *
 * @reference
 *      SAIS_Student_Version_Elena_simplified.pdf
 *
 * @author Athit Vue
 * @date 5/01/2018
 */
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#define DEBUG 0

using namespace std;

// Prototyping:
void assign_index_to_T(vector<int> &T_array, string inputted_string, int size_of_string);
int get_number_of_occurences(vector<int> &T_array, vector<int> &number_of_occurences);
void get_head_tail_indexes(vector<int> &number_of_occurences, vector<int> &bucket_head,
  vector<int> &bucket_tail);
void calculate_S_type(vector<int>&T_array, vector<int>&SA_array,
  vector<int>&S_type_array, vector<int>&bucket_head, vector<int>&bucket_tail);
void induce_sort(vector<int>&T_array, vector<int>&SA_array, vector<int>&S_type_array,
  vector<int>&L_type_array, vector<int>&bucket_head, vector<int>&bucket_tail,
  vector<int>&number_of_occurences);
void print_SA_array(vector<int> &SA_array);
void calculate_T1_array(vector<int>&T_array, vector<int>&SA_array, vector<int>&S_type_array,
  vector<int>&T1_array, vector<int>&X_array, vector<int>&L_type_array);
int compare_lms_substrings(vector<int> &S_type_array, vector<int> &T_array,
  int previous, int p);
void run_SAIS(vector<int> &SA_array, vector<int> &T_array_param, int size_of_string,
  int &recursion_counter);
void print_BWT(vector<int> &SA_array, string substring);

int main(){
  string inputted_string;
  int size_of_string;
  int recursion_counter = 0;

  // Read string until EOF. Use ostringstream to concatenate any ostrings
  // with a space...etc.
  ostringstream stream;
  string read_line;
  while (getline (cin, read_line)){
    stream << read_line;
  }

  inputted_string = stream.str();

  // See if ostringstream will suffice
  if (DEBUG){
    cout << endl;
    cout << "#################### DEBUGGING STARTS ####################" << endl;
    cout << endl;
    cout << "Inputted_string: < " << inputted_string << " >" << endl;
    cout << endl;
  }

  // Don't forget the dollar sign.
  size_of_string = inputted_string.size() + 1;

  if (DEBUG){
    cout << "Size of inputed string is: <" << size_of_string << ">" << endl;
    cout << endl;
  }
  // Allocate the T array size is of string. This will contain each char of
  // the string that we have concatenated from input or file. Don't forget
  // the dollar sign, add one to size of string.
  vector<int> T_array(size_of_string);

  // Declare the SA_array we will need and initialize it to -1.
  vector<int> SA_array(size_of_string, -1);

  // Call the function to break down each character and give its index to each.
  assign_index_to_T(T_array, inputted_string, size_of_string);

  // Run the SAIS algorithm
  run_SAIS(SA_array, T_array, size_of_string, recursion_counter);

  // Success, induction is done, now we can print the SA_array to stdout.
  //print_SA_array(SA_array);

  // Once the SAIS algorithm is done then print BWT
  print_BWT(SA_array, inputted_string);

  return 0;
}

/**
 * void run_SAIS
 *
 * Had to add this function so that we may recursively call this function to
 * calculate the suffix array on SA1 for T1. Migrated the function calls in main
 * that deals with the SAIS algorithm to here.
 *
 * @param SA_array The address to the SA array.
 * @param T_array The address to the T array.
 */
void run_SAIS(vector<int> &SA_array, vector<int> &T_array, int size_of_string,
  int &recursion_counter){
  int size_of_alphabet = 0;
  int tail = 0;
  vector<int> S_type_array((int) T_array.size(), 0);
  vector<int> L_type_array((int) T_array.size(), 0);
  vector<int> number_of_occurences;
  vector<int> T1_array;
  vector<int> SA1_array;
  vector<int> X_array;

  // Need to know how much a character occurs to properly index the
  // bucket.
  size_of_alphabet = get_number_of_occurences(T_array, number_of_occurences);

  // Need to check for termination of this recursive function:
  // Check if all characters of T1 are different (if the size of the alphabet
  // is smaller than the length of T, then some chars are repeated).
  if(size_of_alphabet == (int) T_array.size()){
    if(DEBUG){
      cout << "T_array index pushed to SA_array are:" << endl;
    }
    for(int i = 0; i < (int) SA_array.size(); i++){
      SA_array[T_array[i]] = i;
      if(DEBUG){
        cout << "Pushed index <" << i << "> to SA_array" << endl;
      }
    }
    if(DEBUG){
      cout << endl;
    }
    return;
  }

  // Create the bucket array.
  // At first I create one  array and referenced pointers to the beginning and ending
  // of all buckets but this is hard to maintain. Thus...
  vector<int> bucket_head(size_of_alphabet);
  vector<int> bucket_tail(size_of_alphabet);

  // Get head and tail indexes of each of the buckets
  get_head_tail_indexes(number_of_occurences, bucket_head, bucket_tail);

  // I was doing L-type first but instructions in the SAIS_Student_Version_Elena_simplified.pdf
  // says to look for S-type first. In class, we did L-type first. I am going to
  // try it this way, looking for S-type first. Then will induce, and look for L-type next.
  // NOTE: if it doesn't work, I will implement L-type first.
  calculate_S_type(T_array, SA_array, S_type_array, bucket_head, bucket_tail);

  // Step 1:
  // Induce-sort step. Sort all LMS-substrings of T and place each of the substrings
  // in their corresponding buckets in SA.
  induce_sort(T_array, SA_array, S_type_array, L_type_array, bucket_head, bucket_tail,
    number_of_occurences);

  // Step 2:
  // Give each LMS-substring of T a name and construct a shortened string T1, whose
  // alpabet consists of integer-names of LMS-substrings.
  calculate_T1_array(T_array, SA_array, S_type_array, T1_array, X_array, L_type_array);

  // Step 3:
  // Call SAIS recursively to calculate the suffix array SA1 for T1.

  // Resize the SA1_array to the size of T1_array and initialize all elements to -1.
  SA1_array.resize((int) T1_array.size(), -1);

  if(DEBUG){
    cout << "########## Recursion number: " << recursion_counter << " #############"<< endl;
    cout << endl;
    if(recursion_counter == 0){
      cout << "SA_array is (original):" << endl;
    }
    else{
      cout << "SA_array is:" << endl;
    }
    for(int i = 0; i < (int) SA_array.size(); i++){
      cout << SA_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }

  recursion_counter = recursion_counter + 1;
  // Call recursively SA-IS on T1 to calculate the suffix array SA1 for T1.
  run_SAIS(SA1_array, T1_array, size_of_string, recursion_counter);

  // Step 4:
  // After recursive call:
  // Induce SA from SA1.

  // Reset Bucket so that bucket_tail points to the end
  for(int i = 0; i < size_of_alphabet; i++){
    bucket_tail[i] = tail;
    if (size_of_alphabet -1 != i){
      tail = tail + number_of_occurences[i+1];
    }
  }

  if(DEBUG){
    cout << "BUCKET_TAIL ARRAY AFTER RESETTING: " << endl;
    for(int i = 0; i < (int) bucket_tail.size(); i++){
      cout << bucket_tail[i] << " ";
    }
    cout << endl;
    cout << endl;
  }

  // Reset SA[i]=-1, for all i=0,1,...,n.
  for(int i = 0; i < (int) T_array.size(); i++){
    SA_array[i] = -1;
  }

  // Now we can place positions of LMS-substrings in order of SA1. Scan from right to left
  for(int j = (int) SA1_array.size()-1; j >= 0; j--){
    int i = SA1_array[j];
    int p = X_array[i];

    SA_array[bucket_tail[T_array[p]]] = p;
    bucket_tail[T_array[p]] = bucket_tail[T_array[p]] - 1;
  }

  if(DEBUG){
    cout << "T1_array: " << endl;
    for(int i = 0; i < (int) T1_array.size(); i++){
      cout << T1_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }

  if(DEBUG){
    cout << "SA1_array: " << endl;
    for(int i = 0; i < (int) SA1_array.size(); i++){
      cout << SA1_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }

  if(DEBUG){
    cout << "BUCKET_TAIL ARRAY (after inducing SA from SA1): " << endl;
    for(int i = 0; i < (int) bucket_tail.size(); i++){
      cout << bucket_tail[i] << " ";
    }
    cout << endl;
    cout << endl;
  }

  // Resetting the head and tail indexes
  get_head_tail_indexes(number_of_occurences, bucket_head, bucket_tail);
  induce_sort(T_array, SA_array, S_type_array, L_type_array, bucket_head, bucket_tail,
    number_of_occurences);
}

/**
 * void assign_index_to_T
 *
 * Need a flag to check to see if chararcters exist in the string.
 * We need to give new name to characters starting from 1. 0 is automatically
 * (intuitively) assigned to $. This is the function to rename the
 * T array.
 *
 * @param T_array The address of the vector array T_array.
 * @param inputted_string The concatenated string from input.
 * @param size_of_string The size of the inputted_string.
 */
void assign_index_to_T(vector<int> &T_array, string inputted_string, int size_of_string){
  // First initialize the array that will check for each character.
  int is_in_array[256] = {0};

  // This is the important counter variable that will give the new name
  // to the T_array. Maintain this variable throughout assignment of T_array.
  int counter_index = 0;

  for(int i = 0; i < size_of_string; i++){
    is_in_array[(int) inputted_string[i]] = 1;
  }

  if (DEBUG){
    cout << "Is in array is: " << endl;
    for(int i = 0; i < size_of_string; i++){
      cout << i << ": " << inputted_string[i] << " = ";
      cout << is_in_array[(int) inputted_string[i]] << endl;
    }
    cout << endl;
  }

  // Must include the dollar sign to be included in T array
  T_array[(int) T_array.size() - 1] = 0;

  // New integer name array for T
  int new_name[256];
  // Initialize the new_name array to 0; this is already taking $ into
  // consideration
  for (int i = 0; i < 256; i++){
    new_name[i] = 0;
  }

  // For all possible characters, if cur char is in the array and have not
  // been accounted for then increment the name, if not then don't.
  for(int i = 0; i < 256; i++){
    if (is_in_array[i]){
      new_name[i] = counter_index;
      counter_index = counter_index + 1;
    }
  }

  // Finally, map the new name to vector T_array
  for(int i = 0; i < size_of_string; i++){
    T_array[i] = new_name[(int) inputted_string[i]];
  }

  if (DEBUG){
    cout << "T_array now contains:" << endl;
    for(int i = 0; i < size_of_string; i++){
      cout << "T_array[" << i << "] is: " << T_array[i] << endl;
    }
    cout << endl;
  }
}

/**
 * void get_number_of_occurences
 *
 * Gets the number of occurences of each of the characters thats in T_array.
 *
 * @param T_array The address to the T_array.
 * @param number_of_occurences The address to the number_of_occurences array.
 * @param size_of_string The size of the T_array.
 * @return number_of_occurences.size() The size of the array number_of_occurences.
 */
int get_number_of_occurences(vector<int> &T_array, vector<int> &number_of_occurences){
  // counter
  int temp_largest;
  temp_largest = 0;
  int size_of_T = (int) T_array.size();
  // Get the largest # occurrences; the size of all unique characters in the string.
  for(int i = 0; i < size_of_T; i++){
    if(temp_largest < T_array[i]){
      temp_largest = T_array[i];
    }
  }

  // Resize the occurrences array to store only the unique chars.
  // Dont forget to add a spot for $.
  number_of_occurences.resize(temp_largest + 1);

  // Count the number of occurences and store it.
  for(int i = 0; i < size_of_T; i++){
    number_of_occurences[T_array[i]] = number_of_occurences[T_array[i]] + 1;
  }

  if (DEBUG){
    cout << "Number of occurences Array:" << endl;
    for(int i = 0; i < (int) number_of_occurences.size(); i++){
      cout << i << ": " << number_of_occurences[i] << endl;
    }
    cout << endl;
  }

  // Return the size of number of occurences
  return (int) number_of_occurences.size();
}

/**
 * void get_head_tail_indexes
 *
 * Gets the indexes of each occurences head and tail.
 *
 * @param number_of_occurences The address of the number_of_occurences array.
 * @param bucket_head The address of the bucket_head array.
 * @param bucket_tail The address of the bucket_tail array.
 */
void get_head_tail_indexes(vector<int> &number_of_occurences, vector<int> &bucket_head,
  vector<int> &bucket_tail){
  int head, tail;
  int size_of_occurences;
  head = 0;
  tail = 0;

  size_of_occurences = (int) number_of_occurences.size();

  if(DEBUG){
    cout << "Bucket (head and tail):" << endl;
  }

  // The occurences array currently stores the number of occurences in order.
  // All we need to do here is increase from the number of starting index by
  // the number of occurences. Relative to that new index, repeat until we
  // have gone to the end of the index.
  for(int i = 0; i < size_of_occurences; i++){
    bucket_head[i] = head;  // bucket head and tail starts at index 0.
    bucket_tail[i] = tail;

    // Check to make sure the head and tail is set correctly
    if(DEBUG){
      cout << i << ": head is at <" << head << ">" << endl;
      cout << " " << ": tail is at <" << tail << ">" << endl;
    }

    // Increase the head pointer by the element (# of occurences) that happens
    // at the current index in the number_of_occurences array.
    head = head + number_of_occurences[i];

    // Need to set the tail pointer; tail must not overlap to next chars bucket
    if ((size_of_occurences - 1) != i){
      tail = tail + number_of_occurences[i+1];
    }
    // bucket_head[i] = head;  // bucket head and tail starts at index 0.
    // bucket_tail[i] = tail;
  }

  if (DEBUG){
    cout << endl;
  }
}

/**
 * void calculate_S_type
 *
 * Scan the T_array from right to left, while maintaining the S_type array.
 * If it is S type then set the current index to 1. If not then move on to the
 * next index (S_type array already initialized to 0 when created).
 *
 * Composition added (4/23/18): map S-type to SA_array while looking for
 * and setting S-type. Much more efficient this way.
 *
 * @param T_array The address to the T_array.
 * @param SA_array The address to the SA_array.
 * @param S_type_array The address of the S_type_array.
 * @param bucket_head The address to the bucket_head array.
 * @param bucket_tail The address to the bucket_tail array.
 */
void calculate_S_type(vector<int>& T_array, vector<int>& SA_array,
  vector<int> &S_type_array, vector<int> &bucket_head, vector<int>& bucket_tail){
  int S_type_size = (int) S_type_array.size();
  // The last index holds the $ set that as S-type first
  S_type_array[(S_type_size -1)] = 1;

  // The very next one to the left should be L-type since $ is the smallest.
  S_type_array[(int) S_type_array.size()-2] = 0;

  // Scan the T array from right to left
  for(int i = T_array.size() - 2; i >= 0; i--){
    // Check to see if right of current is bigger than or equal to current
    if (T_array[i+1] >= T_array[i]){
      // This means current is less so it's a S type
      if(T_array[i+1] > T_array[i]){
        S_type_array[i] = 1;
      }
      if(T_array[i+1] == T_array[i]){
        S_type_array[i] = S_type_array[i+1];
      }

    }
    else{
      // Current is L-type, check if right of current is of S-type...
      // If yes, then we found LMS starting. Map the LMS to SA_array. Append
      // at the end of the bucket for found char that is of LMS type.
      if (S_type_array[i+1] == 1){
        SA_array[bucket_tail[T_array[i+1]]] = (i+1);
        // Move the tail to left one.
        bucket_tail[T_array[i+1]] = bucket_tail[T_array[i+1]] - 1;
      }
    }
  }

  if (DEBUG){
    cout << "T_array mapped with type t:" << endl;
    cout << "I: ";
    for (int i = 0; i < (int) S_type_array.size(); i++){
      cout << i << " ";
    }
    cout << endl;
    cout << "T: ";
    for (int i = 0; i < (int) S_type_array.size(); i++){
      cout << T_array[i] << " ";
    }
    cout << endl;
    cout << "t: ";
    for (int i = 0; i < (int) S_type_array.size(); i++){
      cout << S_type_array[i] << " ";
    }
    cout << endl;
    cout << endl;

    cout << "SA_ARRAY before induce sort: " << endl;
    cout << "i : ";
    for(int i = 0; i < (int) SA_array.size(); i++){
      cout << i << " ";
    }
    cout << endl;
    cout << "SA: ";
    for(int i = 0; i < (int) SA_array.size(); i++){
      cout << SA_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }
}

/**
 * void calculate_L_type
 *
 * From referece:
 * "Induce-sort L-type suffixes using LMS-substrings, i.e. fill in SA[i]
 *  entries such that SA[i] = p, and p is the starting position of an
 *  L-type suffix"
 *
 * Scan SA_array from left to right and store each p in the front of each
 * bucket if p is of L-type.
 *
 * @param T_array Address to the T_array.
 * @param SA_array Address to the SA_array.
 * @param S_type_array Address to the S_type_array.
 * @param L_type_array Address to the L_type_array.
 * @param bucket_head Address to the bucket_head array.
 * @param bucket_tail Address to the bucket_tail_array.
 */
void induce_sort(vector<int>&T_array, vector<int>&SA_array, vector<int>&S_type_array,
  vector<int>&L_type_array, vector<int>&bucket_head, vector<int>&bucket_tail,
  vector<int>&number_of_occurences){
  int SA_size = (int) SA_array.size();
  //int S_type_size = (int) S_type_array.size();
  //int L_type_size = (int) L_type_array.size();
  int T_array_size = (int) T_array.size();
  int number_of_occurences_size = (int) number_of_occurences.size();

  // P is the starting position of an L-type suffix.
  int p;
  int temp_end_ptr = 0;

  // Start with L-type from left to right. Induce-sort L-type suffixes
  // using LMS-substrings.
  for(int i = 0; i < SA_size; i++){
    // Scan from SA[0] to SA[n].
    p = SA_array[i];

    /* Need this condition or else fails */
    if(p > 0){
      // If p = SA_array[i] - 1 is of L-type, then put p in front of T[p]-bucket.
      if(S_type_array[p-1] == 0){
        SA_array[bucket_head[T_array[p-1]]] = (p-1);
        // Increment the bucket_head pointer
        bucket_head[T_array[p-1]] = bucket_head[T_array[p-1]] + 1;
      }
    }
  } // Done with L-type

  if(DEBUG){
    cout << "SA_ARRAY_L_TYPE (after induce_sort): " << endl;
    cout << "i : ";
    for(int i = 0; i < SA_size; i++){
      cout << i << " ";
    }
    cout << endl;
    cout << "SA: ";
    for(int i = 0; i < SA_size; i++){
      cout << SA_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }

  if(DEBUG){
    cout << "Resetting bucket tail (MUST MATCH ABOVE TAIL):" << endl;
  }

  // Reset the values of Bucket_tail to point to end of "c-buckets"
  // (our number_of_occurences array); We are going to induce-sort S-type
  // suffixes next going from right to left so need to reset.
  for(int i = 0; i < number_of_occurences_size; i++){
    bucket_tail[i] = temp_end_ptr;

    // Check to make sure tail resetted correctly. Reference this back to
    // the "Bucket (head and tail)" debugging comments above.
    if(DEBUG){
      cout << i << " : tail is at <" << temp_end_ptr << ">" << endl;
    }

    // Make sure not to overlap char indexes
    if((number_of_occurences_size-1) != i){
      temp_end_ptr = temp_end_ptr + number_of_occurences[i+1];
    }
  } // Bucket_tail done resetting

  if(DEBUG){
    cout << endl;
  }

  // Now that bucket tail has been resetted correctly, we need to induce-sort
  // S-type suffixes from right to left.
  for(int j = (SA_size-1); j >= 0; j--){
    int p;
    p = SA_array[j];
    // Must address that (p-1) or (p-2), might occur out-of-range.
    int p2 = (p-2);
    if(p != 0){
      // If p is 0, circle around. This takes care of (p-1) out-of-range.
      if(p == 0){
        p = SA_size;
      }
      // P-1 is S-type if it equals 1
      if(S_type_array[p-1] == 1){
        SA_array[bucket_tail[T_array[p-1]]] = (p-1);
        // Must address (p-2)
        if(p2 < 0){
          // then use T.size - 1
          p2 = (T_array_size - 1);
        }
        // If after p2, we have L-type
        if(S_type_array[p2] == 0){
          // then store it in L-types bucket
          L_type_array[bucket_tail[T_array[p-1]]] = 1;
        }
        // Move the bucket_tail's one place
        bucket_tail[T_array[p-1]] = bucket_tail[T_array[p-1]] - 1;
      }
    }
  } // Done with S-type induce-sort

  // Debug all this craziness to make sure that it works.
  if(DEBUG){
    cout << "SA_ARRAY_S_TYPE (after induce_sort): " << endl;
    cout << "i : ";
    for(int i = 0; i < SA_size; i++){
      cout << i << " ";
    }
    cout << endl;
    cout << "SA: ";
    for(int i = 0; i < SA_size; i++){
      cout << SA_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }
}

/**
 * void print_SA_array
 *
 * Prints the SA_ARRAY
 *
 * @param SA_array The address to the SA_array.
 */
void print_SA_array(vector<int> &SA_array){
  int SA_size = (int) SA_array.size();

  for(int i = 0; i < SA_size; i++){
   cout << SA_array[i] << " ";
  }
  cout << endl;
}

/**
 * void calculate_T1_array
 *
 * This function calculates the T1 array. It is step 2 from the
 * SAIS_Student_Version_Elena_simplified instructions.
 *
 * @param T_array The address to T array.
 * @param SA_array The address to the SA array.
 * @param S_type_array The address to the S type array.
 * @param T1_array The address to the T1 array.
 * @param X_array The address to the X array
 */
void calculate_T1_array(vector<int>&T_array, vector<int>&SA_array, vector<int>&S_type_array,
  vector<int>&T1_array, vector<int>&X_array, vector<int>&L_type_array){
  // Need to maintain an array N (names of LMS substrings) of the same size as T)
  vector<int> N_array(T_array.size(), -1);
  // // N[n] should be equal to 0, this accounts for the dollar sign.
  N_array[(int) T_array.size()-1] = 0;
  // Keep track of current name
  int cur_name = 0;
  // Need temp pointer to one LMS-substring
  int previous;
  // Inially, previous is set to SA[0]
  previous = SA_array[0];

  if(DEBUG){
    cout << "Previous is set to: " << previous << endl;
    cout << endl;
  }

  if(DEBUG){
    cout << "L_type_array:" << endl;
    for(int i = 0; i < (int) L_type_array.size(); i++){
      cout << L_type_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }

  // Fill in N Array. Scan SA from left-to-right, and check if L[i] = 1. If true,
  // LMS-substring occurs at position p = SA[i] in T.
  for(int i = 0; i < (int) SA_array.size(); i++){
    int p = SA_array[i];
    if(L_type_array[i] == 1){
      // Check if substring equals
      int ret_val = compare_lms_substrings(S_type_array, T_array, previous, p);
      if(ret_val == 0){
        cur_name++;
      }

      if(DEBUG){
        cout << "Cur_name to be pushed is: " << cur_name << endl;
        cout << endl;
      }

      N_array[p] = cur_name;
      previous = p;
    }
  }

  // Now we can fill in T1. Scan array N from left to right and fill T1
  for(int p = 0; p < (int) N_array.size(); p++){
    if (N_array[p] < 0){
      continue;
    }
    else{
      X_array.push_back(p);
      T1_array.push_back(N_array[p]);
    }
  }

  if(DEBUG){
    cout << "T1_array (calculate_T()):" << endl;
    for(int i = 0; i < (int) T1_array.size(); i++){
      cout << T1_array[i] << " ";
    }
    cout << endl;
    cout << endl;
  }
}

/**
 * int compare_lms_substrings
 *
 * Helper function to compare the current LMS substring to the LMS substring
 * pointed by the previous pointer.
 *
 * @param S_type_array The address of the S_type_array.
 * @param T_array The address of the T_array.
 * @param previous The previous LMS-substring.
 * @param p The position of SA[i] in T.
 * @return 1 Returns a 1 if the LMS substrings are identical.
 * @return 0 Returns a 0 if the LMS substrings are not identical.
 */
int compare_lms_substrings(vector<int> &S_type_array, vector<int> &T_array, int previous, int p){
  bool started  = true;
  while(1){
    if((T_array[p] != T_array[previous]) || (S_type_array[previous] != S_type_array[p])){
      return 0;
    }
    else if((S_type_array[previous] == 1 && !started && S_type_array[previous-1] == 0) ||
      previous == (int) T_array.size()-1){
      break;
    }
    else if((S_type_array[p] == 1 && !started && S_type_array[p-1] == 0) ||
      p == (int) T_array.size() -1){
      break;
    }
    else{
      previous = previous + 1;
      p = p + 1;
      started = false;
    }
  }
  return 1;
}

/**
 * void print_BWT
 *
 * This function prints the BWT after the SAIS algorithm has finished
 * executing.
 *
 * @param SA_array The address to the SA array.
 * @param substring The inputted string, substring to be printed.
 */
void print_BWT(vector<int> &SA_array, string substring){
  int p = 0;
  for(int i = 0; i < (int) SA_array.size(); i++){
    p = SA_array[i];
    p = p - 1;
    if(p < 0){
      continue;
    }
    else{
      cout << substring[p];
    }
  }
  cout << endl;
}
