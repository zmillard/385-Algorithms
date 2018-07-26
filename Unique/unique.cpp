/*******************************************************************************
 * Name        : unique.cpp
 * Author      : Zoe Millard
 * Date        : 05-28-16
 * Description : Determining uniqueness of chars with int as bit vector.
 * Pledge      : "I pledge my honor that I have abided my the Stevens honor System" -zm
 ******************************************************************************/
#include <iostream>
#include <cctype>
#include <string>
using namespace std;
/*
 * Purpose: This method verifies that all elements in the string are lowercase letters - not uppercase, numbers or symbols
 *          which is done by checking the ascii values for a and z to make sure it falls within those lines.
 * Input:   string &s    contains letters
 * Output:  bool         true if all elements are lowercase letters, false if otherwise
 */
bool is_all_lowercase(const string &s) {
	int size = s.size();
    for(int i = 0; i < size; i++){
    	char c = (char)s[i];
    	int v = (c & 32);
    	int v2 = (char)s[i];
    	if (v <= 0){ //bit comparison to see if not lowercase
    		return false;
    	}
    	if (v2 > 122 || v2 < 97){
    		return false;
    	}					//another if statement to test for strings
    }
	return true;
}
/*
 * Purpose: This method verifies that all elements in the string are unique. This is done by using an unsigned int to
 *          track the a, b, c... etc values to ensure each is only used one time. Values are loaded in using bitshifting and bitwise operations.
 * Input:   string &s    contains letters
 * Output:  bool         true if all elements are unique letters, false if otherwise
 */
bool all_unique_letters(const string &s) {
    // TODO: returns true if all letters in string are unique, that is
    // no duplicates are found; false otherwise.
    // You may use only a single int for storage and work with bitwise
    // and bitshifting operators.
    // No credit will be given for other solutions.
	int size = s.length();
	unsigned int v = 0;
	for (int j = 0; j < size; j++){
		int temp = (char)s[j] - 97;
		if(((v >> temp) & 1) == 1){
			return false;
		}else{
			v = (1 << temp) | v;
		}
	}
	return true;
}
/*
 * Purpose: Main method takes input and outputs results, error checking procedures are in place to ensure proper values and no spaces between letters.
 * Input:   string pointer argv    contains letters
 *          int argc               contains the int size of the pointer(should be 2 for proper input)
 * Output:  -
 */
int main(int argc, char * const argv[]) {
    // TODO: reads and parses command line arguments.
    // Calls other functions to produce correct output.
	if(argc != 2){
		cout << "Usage: ./unique <string>" << endl;
	} else{
		string s(argv[1]);
		if (!is_all_lowercase(s)){
			cout << "Error: String must contain only lowercase letters." << endl;
		} else if (!all_unique_letters(s)){
			cout << "Duplicate letters found." << endl;
		} else if (all_unique_letters(s)) {
			cout << "All letters are unique." << endl;
		}
	}
}
