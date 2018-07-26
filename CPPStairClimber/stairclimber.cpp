/*******************************************************************************
 * Name        : stairclimber.cpp
 * Author      : Zoe Millard
 * Date        : 6/5/2016
 * Description : Lists the number of ways to climb n stairs.
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System" - zm
 ******************************************************************************/
#include <iostream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <iomanip>

using namespace std;
/*
 * This method recursively generates a 2d vector containing a list of stair combinations
 * input  int num_stairs          the total number of stairs
 * return vector< vector<int> >   the number of stepping combinations for climbing the stairs
 *
 */
vector< vector<int> > get_ways(int num_stairs) {
    // TODO: Return a vector of vebactors of ints representing
    // the different combinations of ways to climb num_stairs
    // stairs, moving up either 1, 2, or 3 stairs at a time.
	std::vector< vector<int> > ways;
	std::vector< vector<int> > result;
	std::vector<int> way;
	if(num_stairs <= 0){
		ways.push_back(way);
	    return ways;
	}else{
		for(int i = 1; i < 4; i++){
			if(num_stairs >= i){
				result = get_ways(num_stairs - i);
				for(vector< vector<int> >::iterator it = result.begin(); it < result.end(); it++){
					it->push_back(i);
				}
				ways.insert(ways.end(), result.begin(), result.end());
			}
		}
	}
	return ways;
}
/*
 * Prints the stair combinations and info about the stairs
 * input  vector< vector<int> >     the total number of ways to climb stairs
 * return void
 *
 */
void display_ways(const vector< vector<int> > &ways) {
    // TODO: Display the ways to climb stairs by iterating over
    // the vector of vectors and printing each combination.
	std::vector < vector<int> >::const_iterator row;
	vector<int>::const_reverse_iterator col;
	std::vector < vector<int> >::const_iterator row2;
	vector<int>::const_iterator col2;
	int i = 1;
	int size = 0;
	string nums = "";
	int j = 0;

	for(row2 = ways.begin(); row2 != ways.end(); row2++, j++) {
		size = 0;
		for (col2 = row2->begin(); col2 != row2->end(); col2++) {
			size += *col2;
		}

	}
	if (j<2){
		cout << j << " way to climb " << size << " stair." <<endl;
	}else {
		cout << j << " ways to climb " << size << " stairs." <<endl;
	}

	if (true){
		for(row = ways.begin(); row != ways.end(); row++, i++) {
			if (i < 10 && j > 9){
				cout << " " << i << ". [";
			}else{
				cout << i << ". [";
			}
			for (col = row->rbegin(); col != row->rend(); col++) {
				std::cout << *col;
				if (col + 1 != row->rend()){
					cout << ", ";
				}
			}
			cout << "]" << endl;
		}
	}
}
/*
 * This method determines if an input is a number
 * input  string s   the input for num_stairs
 * return bool       true if it is a number, false otherwise
 *
 */
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
/*
 * Main method feeds input and generates error messages
 * input  int argc             the length of input
 * input  char * const argv[]  input
 *
 */
int main(int argc, char * const argv[]) {
	if(argc != 2){
		cerr << "Usage: ./stairclimber <number of stairs>" << endl;
		return 0;
	}
	string s(argv[1]);
	if (is_num(s)){
		istringstream buffer(s);
		int value;
		buffer >> value;
		display_ways(get_ways(value));
	}
}
