/*******************************************************************************
 * Name        : maxsumdescent.cpp
 * Author      : Zoe Millard
 * Version     : 1.0
 * Date        : 6/26/16
 * Description : Dynamic programming solution to max sum descent problem.
 * Pledge      : I pledge my honor that I have abided by the Stevens Honor System - ZM
 ******************************************************************************/
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

const char DELIMITER = ' ';

int **values, // This is your 2D array of values, read from the file.
    **sums;   // This is your 2D array of partial sums, used in DP.

int num_rows; // num_rows tells you how many rows the 2D array has.
              // The first row has 1 column, the second row has 2 columns, and
              // so on...

/**
 * Returns the number of digits in the non-negative integer num.
 */
int num_digits(int num) {
    int count = 1;
    while (num > 9) {
        num /= 10;
        ++count;
    }
    return count;
}

/**
 * Displays the 2D array of values read from the file in the format of a table.
 */
void display_table() {
    int max = 0;
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j <= i; j++) {
            if (values[i][j] > max) {
                max = values[i][j];
            }
        }
    }
    int max_digits = num_digits(max);
    for (int i = 0; i < num_rows; i++) {
        for (int j = 0; j <= i; j++) {
            cout << setw(max_digits) << values[i][j];
            if (j != i) {
                cout << " ";
            }
        }
        cout << endl;
    }
}

/**
 * Returns the maximum sum possible in the triangle of values.
 * By starting at the top of the triangle and moving to adjacent numbers on the
 * row below, this function uses dynamic programming to build up another table
 * of partial sums.
 */
int compute_max_sum() {
    // TODO

    // Populate the 2D array of partial sums. It should still work even if
    // num_rows is 0 (i.e. the file was blank).


	for(int row = 0; row < num_rows; row++){
		for(int col = 0; col <= row; col++){
			sums[row][col] = values[row][col]; //sets initial values for sums
		}
	}

	for(int i = 0; i < num_rows; i++){
		for(int j = 0; j <= i; j++){
			if(i == 0 && j == 0){ //if its the val at the top of the stack
				sums[i][j] = values[i][j];
			}else if(j == 0){ //if j cannot go left any further
				sums[i][j] = sums[i-1][j] + values[i][j];
			}else if(i == j){ //if j and i are a val on the edge of the triangle
				sums[i][j] = sums[i-1][j-1] + values[i][j];
			}else{
				sums[i][j] = std::max(sums[i-1][j-1] + values[i][j], sums[i-1][j] + values[i][j]); //records the larger val
			}
		}
	}

    // Loop over the last row to find the max sum.
	int max = 0; //was using first num to iterate thru
	for(int i = 0; i < num_rows; i++){
		if(max < sums[num_rows-1][i]){
			max = sums[num_rows-1][i];
		}
	}

    // Return the max sum.
    return max;
}

/**
 * Returns a vector of ints with the values from the top to the bottom of the
 * triangle that comprise the maximum sum.
 */
vector<int> backtrack_solution() {
    vector<int> solution;
    // TODO

    // Loop over the last row to find the max sum.
    int max = 0; //was using first num to iterate thru
    int i = 0;
    int i_val = 0;
    if(num_rows == 0){ //checks base cases for if the triangle is empty
    	return solution;
    }else if(num_rows == 1){ //checks to see if there is only 1 element
    	solution.push_back(values[0][0]);
    	return solution;
    }
	for(; i < num_rows; i++){ //finds the max val of the last row to begin trace
		if(sums[num_rows-1][i] > max){
			max = sums[num_rows-1][i];
			i_val = i; //location of max val, used to locate next max
		}
	} //go to last row, find whats greater
    // Loop over the remaining rows to find the max sum in each. Then put the
    // value at index[row][max_col] in the solution. Use only a single loop.
    // Do not check all the values in each row - it's unnecessary!
	//int max = sums[0][num_rows];
	//i--;

	for(int j = num_rows - 1; j >= 0; j--){
		if(i_val == 0){ //if the value in the col
			solution.push_back(values[j][i_val]); //push the val into the solution stack
		}else if (i_val == j){ //if the values are on a diagonal, it has to be added and go left one
			solution.push_back(values[j][i_val]);
			i_val--;
		}else if(sums[j][i_val]<sums[j][i_val-1]){ //if the left one left is greater than the one on the right/straight ahead
			solution.push_back(values[j][i_val-1]);
			i_val--;
		}else{
			solution.push_back(values[j][i_val]);//if the left one left is greater than the one on the right/straight ahead
		}
	}
    // Don't forget to reverse the solution!

    reverse(solution.begin(), solution.end()); //reverse
    return solution;
}

/**
 * Reads the contents of the file into the global 2D array 'values'. If
 * successful, the function also allocates memory for the 2D array 'sums'.
 */
bool load_values_from_file(const string &filename) {
    ifstream input_file(filename.c_str());
    if (!input_file) {
        cerr << "Error: Cannot open file '" << filename << "'." << endl;
        return false;
    }
    input_file.exceptions(ifstream::badbit);
    string line;
    vector<string> data;
    try {
        while (getline(input_file, line)) {
            data.push_back(line);
        }
        input_file.close();
    } catch (const ifstream::failure &f) {
        cerr << "Error: An I/O error occurred reading '" << filename << "'.";
        return false;
    }
    num_rows = data.size();
    values = new int*[num_rows];
    istringstream converter;
    for (int row = 0; row < num_rows; row++) {
        values[row] = new int[row + 1];
        string &s = data[row];
        int col = 0;
        size_t start = 0, end = s.find(DELIMITER);
        while (end != string::npos) {
            converter.str(s.substr(start, end - start));
            converter >> values[row][col];
            converter.clear();
            start = end + 1;
            end = s.find(DELIMITER, start);
            col++;
        }
        converter.str(s.substr(start, end));
        converter >> values[row][col];
        converter.clear();
    }

    // Create another 2D array of ints for storing partial sums.
    sums = new int*[num_rows];
    for (int row = 0; row < num_rows; row++) {
        sums[row] = new int[row + 1];
    }
    return true;
}

/**
 * Frees up the memory allocated for the 2D array of values and the 2D array of
 * partial sums.
 */
void cleanup() {
    for (int i = 0; i < num_rows; i++) {
        delete [] values[i];
        delete [] sums[i];
    }
    delete [] values;
    delete [] sums;
}

int main(int argc, char * const argv[]) {
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <filename>" << endl;
        return 1;
    }
    string filename(argv[1]);
    if (!load_values_from_file(filename)) {
        return 1;
    }
    display_table();
    cout << "Max sum: " << compute_max_sum() << endl;

    cout << "Values: [";
    vector<int> solution = backtrack_solution();
    for (vector<int>::iterator it = solution.begin();
            it != solution.end(); ++it) {
        if (it != solution.begin()) {
            cout << ", ";
        }
        cout << *it;
    }
    cout << "]" << endl;


    cleanup();
    return 0;
}
