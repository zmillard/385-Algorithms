/*
 * sieve.cpp
 *
 *  Created on: May 23, 2016
 *      Author: O236
 */




/*******************************************************************************
 * Name        : sieve.cpp
 * Author      : Zoe Millard
 * Date        : 5/25/16
 * Description : Sieve of Eratosthenes
 * Pledge      : "I pledge my honor that I have abided by the Stevens Honor System" -ZM
 ******************************************************************************/
#include <cmath>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <math.h>

using namespace std;

class PrimesSieve {
public:

    PrimesSieve(int limit) : is_prime_(new bool[limit + 1]), limit_(limit){
        sieve();
    }

    ~PrimesSieve() {
        delete [] is_prime_;
    }

    inline int num_primes() const {
        return count_num_primes();
    }

    void display_primes() const {
        // TODO: write code to display the primes in the format specified in the
        // requirements document.
        int max_prime_width = num_digits(max_prime_); //given formula for columns length
    	int primes_per_row = 80 / (max_prime_width + 1);
    	int line_count = 0; //counts lines to catch a spacing error
    	for(int i = 0; i <= limit_; i++){
    		if(is_prime_[i]){
    			if(line_count % primes_per_row > 0 || num_digits(limit_) <= 2){ //spacing for items with 1-2 digits as max
					if(i != 2 && num_digits(limit_) <= 2){
						cout << " ";
					}else if(num_digits(limit_) > 2){ //calculates spacing of digits to keep table
						for(int j=0; num_digits(limit_)-num_digits(i) >= j; j++){
							cout << " ";
						}
					}
    			}
    			if(line_count % primes_per_row  == 0 && num_digits(limit_) > 2){ //moves to new line if needed based on given formula
    				if (line_count > 0){
    					cout<<endl;
    				}
    				for(int j=0; num_digits(limit_)-num_digits(i) > j; j++){ //spacing for new line elements
    					cout << " ";
    				}
    			}
    			line_count++;
    			cout << i;
    		}
    	}
    }

private:
    bool * const is_prime_;
    const int limit_;
    int prime_count;
    int num_primes_, max_prime_;


    int count_num_primes() const {
    	// TODO: write code to count the number of primes found
    	int x = 0;
    	for(int i = 0; i <= limit_; i++){ //counts true values. I did it this way to combat an error I found
			if(is_prime_[i]){
				x++;
			}
		}
    	return x;
    }

    int num_digits(int num) const {
        // TODO: write code to determine how many digits are in an integer
        // Hint: No strings are needed. Keep dividing by 10.
    	int count = 0;
    	for(int i = num; i >= 1; ){ //calculates the number of places in a number
    		i/=10;
    		count++;
    	}
    	return count;
    }

    void sieve() {
        // TODO: write sieve algorithm
    	max_prime_ = limit_; //given sieve algorithm
    	prime_count = 0;
    	int n = limit_;
    	for(int k = 2; k <= n; k++){
    		is_prime_[k] = true;
    	}
    	for(int i = 2; i < sqrt(n); i++){
    		if (is_prime_[i] == true){
    			for(int j = pow(i, 2); j <= n; j+=i){
    				is_prime_[j] = false;
    			}
    		}
    	}

    }
};

int main(void) {
    cout << "**************************** " <<  "Sieve of Eratosthenes" <<
            " ****************************" << endl;
    cout << "Search for primes up to: ";
    string limit_str;
    cin >> limit_str;
    int limit;

    // Use stringstream for conversion. Don't forget to #include <sstream>
    istringstream iss(limit_str);

    // Check for error.
    if ( !(iss >> limit) ) {
        cerr << "Error: Input is not an integer." << endl;
        return 1;
    }
    if (limit < 2) {
        cerr << "Error: Input must be an integer >= 2." << endl;
        return 1;
    }
    cout << "" << endl;
    PrimesSieve prime (limit);
    cout << "Number of primes found: " << prime.num_primes() << endl;
    cout << "Primes up to " << limit << ":" << endl;
    prime.display_primes();

    // TODO: write code that uses your class to produce the desired output.
    return 0;
}
