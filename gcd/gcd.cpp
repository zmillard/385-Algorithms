/*
 * gcd.cpp
 *
 *  Created on: May 19, 2016
 *   Zoe Millard
 * "I pledge my honor that I have abided by the Stevens Honor System."-ZM
 */

#include <iostream>
#include <sstream>
using namespace std;

int gcd_recursive(int m, int n){
	if(n == 0){
		return m;
	}
	return (gcd_recursive(n, m%n));
}
int gcd_iterative(int m, int n){
	int i;
	while(n != 0){
		i=m%n;
		m=n;
		n=i;
	}
	return m;
}
int main(int argc, char *argv[]){
	if(argc != 3){
		cerr << "Usage: " << argv[0] << " <integer m> <integer n>" << endl;
		return 1;
	}

	int m, n;
	istringstream iss;

	iss.str(argv[1]);
	if (!(iss >> m)){
		cerr << "Error: The first argument is not a valid integer." << endl;
		return 1;
	}

	iss.clear(); //resets the var
	iss.str(argv[2]);
	if (!(iss >> n)){
		cerr << "Error: The second argument is not a valid integer." << endl;
		return 1;
	}
	cout << "Iterative: gcd(" << m << ", " <<n<< ") = "<< gcd_iterative(m,n) << endl;
	cout << "Recursive: gcd(" << m << ", " <<n<< ") = " << gcd_recursive(m,n) << endl;


	return 0;

}




