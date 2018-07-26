/*******************************************************************************
 * Name        : inversioncounter.cpp
 * Author      :
 * Version     : 1.0
 * Date        :
 * Description : Counts the number of inversions in an array.
 * Pledge      :
 ******************************************************************************/
#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <cstdio>
#include <cctype>
#include <cstring>

using namespace std;

// Function prototype.
static long mergesort(int array[], int scratch[], int low, int high);
static long merge(int array[], int scratch[], int low, int mid, int high);

/**
 * Counts the number of inversions in an array in theta(n^2) time.
 */
long count_inversions_slow(int array[], int length) { //implement nested loops version
    // TODO

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

/**
 * Counts the number of inversions in an array in theta(n lg n) time.
 */
long count_inversions_fast(int array[], int length) { //will call mergesort
	int *temp = (int *)malloc(sizeof(int)*length);
	return mergesort(array, temp, 0, length - 1);
}
static long mergesort(int array[], int scratch[], int low, int high) {
	int mid;
	long inv_count = 0;
	if (high > low){
		mid = (low + high)/2;
		inv_count = mergesort(array, scratch, low, mid);
		inv_count += mergesort(array, scratch, (mid+1), high);
		inv_count += merge(array, scratch, low, (mid + 1), high);
	}
	return inv_count;
}
static long merge(int array[], int scratch[], int low, int mid, int high) {

	long inv_count = 0;
	int i, j, k;
	i = low;
	j = mid;
	k = low;
	while((i <= mid - 1) && (j <= high)){
		//cout << "flag" << endl;
		//cout << i << " " << mid << " " << j << endl;
		if(array[i] <= array[j]){
			scratch[k++] = array[i++];
		}else{
			scratch[k++]=array[j++];
			inv_count = inv_count + (mid - i);
		//cout << inv_count << endl;
		}
	}
	//cout << inv_count << endl;
	while(i <= mid - 1){
		scratch[k++] = array[i++];
	}
	while(j <= high){
		scratch[k++] = array[i++];
	}

	for(i=low; i<= high; i++){
		array[i] = scratch[i];
	}

	return inv_count;
}
bool is_num(const string &s) {
	int size = s.length();
    for(int i = 0; i < size; i++){
    	int v2 = (char)s[i];
    	if (v2 > 57 || v2 < 48){

    		if ((v2 < 122 && v2 > 97) || v2 == 45){
    			cerr << "Error: Number of stairs must be a positive integer." << endl;
    			return false;
    		}
    		return false;
    	}
    }
	return true;
}
int main(int argc, char *argv[]) {
    // TODO: parse command-line argument
	if(argc > 2){
		cerr << "Usage: " << argv[0] << " [slow]";
		return -1;
	}
	if(argc == 2){
		string s(argv[1]);
		if (s.compare("slow")){
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
			if(values.size() == 0){
				cerr << "Error: Sequence of integers not received." << endl;
				return 0;
			}
			cout << "Number of inversions: ";
			cout << count_inversions_slow(&values[0], values.size());
			return 0;
		}
	}
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
	if(values.size() == 0){
		cerr << "Error: Sequence of integers not received." << endl;
		return 0;
	}
	cout << "Number of inversions: " << count_inversions_fast(&values[0], values.size()) << endl;
	return 0;
}


