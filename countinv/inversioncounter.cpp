/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      : Zoe Millard
 * Version     : 1.0
 * Date        : 6/16/16
 * Description : Counts the number of inversions in an array.
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System." - ZM
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;


long mergesort(int array[], int scratch[], int low, int high);
long merge(int array[], int scratch[], int low, int mid, int high);
/*
 * Counts the number of inversions in a given array using a double for loop
 * returns the long of the number of inversions
 * Theta n^2 complexity
 */
long count_inversions_slow(int array[], int length) {
	long counter = 0;
	for(int i = 0; i < length - 1; i++){
		for(int j = i+1; j < length; j++){
			if(array[i] > array[j]){
				counter++;
			}
		}
	}
	return counter;
}
/*
 * Counts the number of inversions in a given array by feeding it to mergesort
 * returns the long of the number of inversions, returned through mergesort
 * Theta nlogn complexity
 */
long count_inversions_fast(int array[], int length) { //will call mergesort
	int *scratch = (int *)malloc(sizeof(int)*length);
	return mergesort(array, scratch, 0, length - 1);
}
/*
 * Sorts the array, incrementing count to keep track of inversions
 * Recursively goes through array to look at smallest values
 * returns counter to count_inversions_fast
 */
long mergesort(int array[], int scratch[], int low, int high){
	int mid;
	long counter = 0;
	if (low < high){
		mid = low+((high - low)/2);
		counter = mergesort(array, scratch, low, mid);
		counter += mergesort(array, scratch, (mid+1), high);
		counter += merge(array, scratch, low, (mid+1), high);
	}
	return counter;
}
/*
 * Sorts the array, incrementing count to keep track of inversions
 * Goes through a portion of the array checking to see if numbers are in place, swaps when needed
 * returns counter to count_inversions_fast
 */
long merge(int array[], int scratch[], int low, int mid, int high){
	int L, H, k;
	long counter = 0;
	L = low;
	H = mid;
	k = low;
	//checks if the low or high marker is within bounds
	while ((L <= mid - 1) && (H <= high)){
		if (array[L] <= array[H]){ //if there is no inversion
			scratch[k++] = array[L++]; //place vals into scratch array
		}else{
			scratch[k++] = array[H++]; //if inversion place lower vals into scratch
			counter += (mid - L); //increment counter to reflect inversions
		}
	}
	while (L <= (mid - 1)){ //if the values at the front were not filled
		scratch[k++] = array[L++]; //fill the scratch array
	}
	while (H <= high){ //if the values at the back were not filled
		scratch[k++] = array[H++]; //fill the scratch array
	}
	for (L=low; L <= high; L++){
		array[L] = scratch[L]; //move scratch array to regular array
	}
	return counter;
}
/*
 * Performs necessary error checking operations
 */
int main(int argc, char** argv){
	if(argc == 2){
		string s(argv[1]);
		if (s.compare("slow")){ //checks for slow
			cerr << "Error: Unrecognized option '" << s << "'." << endl;
			return -1;
		}else if(s == "slow"){
			cout << "Enter sequence of integers, each followed by a space: " << flush;
			istringstream iss;
			int value, index = 0;
			std::vector<int> values;
			string str;
			str.reserve(11);
			char c;
			while (true) {
				c = getchar();
				const bool eoln = c == '\r' || c == '\n';
				if (isspace(c) || eoln) {
					if (str.length() > 0) {
						iss.str(str);
						if (iss >> value) {
							values.push_back(value);
						} else {
							cerr << "Error: Non-integer value '" << str
								 << "' received at index " << index << "." << endl;
							return 1;
						}
						iss.clear();
						++index;
					}
					if (eoln) {
						break;
					}
					str.clear();
				} else {
					str += c;
				}
			}
			if(values.size() == 0){ //checks for content
				cerr << "Error: Sequence of integers not received." << endl;
				return 0;
			}
			cout << "Number of inversions: ";
			cout << count_inversions_slow(&values[0], values.size());
			return 0;
		}
	}
	if(argc > 2){ //checks for multiple arguments
		cerr << "Usage: " << argv[0] << " [slow]";
		return -1;
	}else{ //procedure for fast
		cout << "Enter sequence of integers, each followed by a space: " << flush;
		istringstream iss;
		int value, index = 0;
		std::vector<int> values;
		string str;
		str.reserve(11);
		char c;
		while (true) {
			c = getchar();
			const bool eoln = c == '\r' || c == '\n';
			if (isspace(c) || eoln) {
				if (str.length() > 0) {
					iss.str(str);
					if (iss >> value) {
						values.push_back(value);
					} else {
						cerr << "Error: Non-integer value '" << str
							 << "' received at index " << index << "." << endl;
						return 1;
					}
					iss.clear();
					++index;
				}
				if (eoln) {
					break;
				}
				str.clear();
			} else {
				str += c;
			}
		}

		if(values.size() == 0){ //if there are no values :(
			cerr << "Error: Sequence of integers not received." << endl;
			return 0;
		}
		cout << "Number of inversions: " << count_inversions_fast(&values[0], values.size()) << endl;
		return 0;
	}
}

