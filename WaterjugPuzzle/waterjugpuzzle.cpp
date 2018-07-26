/*
 * waterjug.cpp
 */

#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <vector>

using namespace std;

int cap_a, cap_b, cap_c, a_cont, b_cont, c_cont; //capactites and contents of the jugs
bool reached = false;
int answer[2]; //used to verify the conclusion we reach
string parse;

class state {

public:
    state(int a, int  b, int c, vector<string> adjacent);
    vector<string> order;
    int a_cont, b_cont, c_cont;
    void create_vector();
};

state::state(int a, int b, int c, vector<string> adjacent) {
    a_cont = a, b_cont = b, c_cont = c;
    order = adjacent;
}
/*
* Prints a vector for order[i]
*/
void state::create_vector() {
    for (unsigned int i = 0; i < order.size(); ++i) {
        cout << order[i] << endl;
    }
}
/*
*Verifies if the values we found match the desired conclusion
*/
bool answered(int a, int b) {
    if ((a == answer[0]) && (b == answer[1]))
        return true;
    return false;
}
/*
*Creates a string for the current state
*/
string string_builder(int a, int b, int c) {
    ostringstream oss;
    oss << "(" << a << ", " << b << ", " << c << ")";
    return oss.str();
}
/*
*Takes values for a, b & c, to build an initial state, returns the string vector storing the initial state
*/
vector<string> edge_builder(int a, int b, int c) {
    vector<string> order;
    ostringstream oss;
    oss << "Initial state. (" << a << ", " << b << ", " << c << ")";
    order.push_back(oss.str());
    return order;
}
queue<state> Q;
/*
* Inputs a jug and a corresponding capacity, outputs the lesser value and ensures letter is not negative
*/
int lesser(int letter, int cap){
    if (letter > cap){
        return cap;
    } else if (letter < 0){
        return 0;
    }
    return letter;
}
/*
*Implements BFS, takes a_cont, b_cont and c_cont (the contents of a, b and c)
*/
void breadthSearch(int a_cont, int b_cont, int c_cont) {
    bool **visited = new bool*[cap_a + 1];
    for (int i = 0; i < cap_a + 1; ++i) {
        visited[i] = new bool[cap_b + 1];
        fill(visited[i], visited[i] + cap_b + 1, false);
    }
    state states(a_cont, b_cont, c_cont, edge_builder(a_cont, b_cont, c_cont));
    Q.push(states);
    while (!Q.empty()) {
        a_cont = Q.front().a_cont;
        b_cont = Q.front().b_cont;
        c_cont = Q.front().c_cont;
        visited[a_cont][b_cont] = true;
        if (answered(a_cont, b_cont)) {
            Q.front().create_vector();
            reached = 1;
            break;
        }
        state top = Q.front();
        Q.pop();

        //C->A
        if (a_cont < cap_a) {
            int a = a_cont + c_cont;
            int b = b_cont;
            a = lesser(a, cap_a);
            int c = c_cont - (a - a_cont);
            b = lesser(b, cap_b);
            ostringstream oss, oss1;
            oss1 << a - a_cont;
            oss << "Pour " << oss1.str() << ((a - a_cont > 1) ? " gallons from C to A. " : " gallon from C to A. ");
            parse = oss.str();
            if (!visited[a][b]) { //checks to see if the pair was visited before
                vector<string> way = top.order;
                way.push_back(parse + string_builder(a, b, c));
                state c_to_a(a, b, c, way);
                Q.push(c_to_a);
            }
        }

        //B->A
        if (a_cont < cap_a && b_cont != 0) {
            int c = c_cont;
            int a = a_cont + b_cont;
            a = lesser(a, cap_a);
            int b = b_cont - (a - a_cont);
            b = lesser(b, cap_b);
            ostringstream oss, oss1;
            oss1 << b_cont - b;
            oss << "Pour " << oss1.str() << ((b_cont - b > 1) ? " gallons from B to A. " : " gallon from B to A. ");
            parse = oss.str();
            if (!visited[a][b]) {
                vector<string> way = top.order;
                way.push_back(parse + string_builder(a, b, c));
                state b_to_a(a, b, c, way);
                Q.push(b_to_a);
            }
        }
        //C->B
        if (b_cont < cap_b) {
            int b = c_cont + b_cont;
            int a = a_cont;
            b = lesser(b, cap_b);
            int c = c_cont - (b - b_cont);
            a = lesser(a, cap_a);
            ostringstream oss, oss1;
            oss1 << b - b_cont;
            oss << "Pour " << oss1.str() << ((b - b_cont > 1) ? " gallons from C to B. " : " gallon from C to B. ");
            parse = oss.str();
            if (!visited[a][b]) {
                vector<string> way = top.order;
                way.push_back(parse + string_builder(a, b, c));
                state c_to_b(a, b, c, way);
                Q.push(c_to_b);
            }
        }
        //A->B
        if (b_cont < cap_b && a_cont != 0) {
            int c = c_cont;
            int a;
            int b = a_cont + b_cont;
            if (b > cap_b) {
                b = cap_b;
                a = a_cont - (b - b_cont);
            } else
                a = 0;
            a = lesser(a, cap_a);
            ostringstream oss, oss1;
            oss1 << b - b_cont;
            oss << "Pour " << oss1.str() << ((b - b_cont > 1) ? " gallons from A to B. " : " gallon from A to B. ");
            parse = oss.str();
            if (!visited[a][b]) {
                vector<string> way = top.order;
                way.push_back(parse + string_builder(a, b, c));
                state a_to_b(a, b, c, way);
                Q.push(a_to_b);
            }
        }

        //B->C
        if (a_cont + b_cont < cap_c && b_cont != 0) {
            int a = a_cont;
            int c = c_cont + b_cont;
            c = lesser(c, cap_c);
            int b = c_cont - c;
            b = lesser(b, cap_b);
            a = lesser(a, cap_a);
            ostringstream oss, oss1;
            oss1 << b_cont - b;
            oss << "Pour " << oss1.str() << ((b_cont - b > 1) ? " gallons from B to C. " : " gallon from B to C. ");
            parse = oss.str();
            if (!visited[a][b]) {
                vector<string> way = top.order;
                way.push_back(parse + string_builder(a, b, c));
                state b_to_c(a, b, c, way);
                Q.push(b_to_c);
            }
        }
        //A->C
        if (a_cont + b_cont < cap_c && a_cont != 0) {
            int c = a_cont + c_cont;
            c = lesser(c, cap_c);
            int a = a_cont - (c - c_cont);
            int b = b_cont;
            a = lesser(a, cap_a);
            ostringstream oss, oss1;
            oss1 << a_cont-a;
            oss << "Pour " << oss1.str() << ((a_cont - a > 1) ? " gallons from A to C. " : " gallon from A to C. ");
            parse = oss.str();
            if (!visited[a][b_cont]) {
                vector<string> way = top.order;
                way.push_back(parse + string_builder(a, b, c));
                state c_to_a(a, b, c, way);
                Q.push(c_to_a);
            }
        }
    }
    if (!reached)
        cout << "No solution." << endl;

    for (int i = 0; i <= cap_a; ++i) {
        delete[] visited[i];
    }
    delete[] visited;
}
/*
 *Takes in arguments and checks to see if there are only 6 arguments
 *If so, checks to see if all arguments are positive integers.
 *If not, returns an error message.
 *If so, sets the initial state and calls the method to conduct a breadth search.
 */
int main(int argc, char * argv[ ]){
    if (argc != 7){
        cerr << "Usage: " << argv[0] << " <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>"  << endl;
        return 1;
    }
    istringstream issCA(argv[1]), issCB(argv[2]), issCC(argv[3]), issGA(argv[4]), issGB(argv[5]), issGC(argv[6]);

    if (!(issCA >> cap_a) || !(issCB >> cap_b)){
      cerr << "Error: Invalid capacity '" <<(!(issCA>>cap_a) ? argv[1] : argv[2]) << "' for jug" << (!(issCA>>a_cont) ? " A." : " B.") << endl;
      return 1;
    } else if (!(issCC >> cap_c)){
      cerr << "Error: Invalid capacity '" << argv[3] << "' for jug C." << endl;
      return 1;
    } else if (!(issGA >> a_cont) || !(issGB >> b_cont)){
      cerr << "Error: Invalid goal '" <<(!(issGA>>a_cont) ? argv[4] : argv[5]) << "' for jug" << (!(issGA>>a_cont) ? " A." : " B.") << endl;
      return 1;
    }else if (!(issGC >> c_cont)){
      cerr << "Error: Invalid goal '" << argv[6] << "' for jug C." << endl;
      return 1;
    } else if (!(a_cont >= 0) || !(b_cont >= 0)){
      cerr << "Error: Invalid goal '" <<(!(a_cont >= 0) ? argv[4] : argv[5]) << "' for jug" << (!(a_cont >= 0) ? " A." : " B.") << endl;
      return 1;
    } else if(!(c_cont >= 0)){
      cerr << "Error: Invalid goal '" << argv[6] << "' for jug C." << endl;
      return 1;
    } else if ((cap_a <= 0) || (cap_b <= 0)){
      cerr << "Error: Invalid capacity '" <<((cap_a <= 0) ? argv[1] : argv[2]) << "' for jug" << ((cap_a <= 0) ? " A." : " B.") << endl;
      return 1;
    } else if (cap_c <= 0){
      cerr << "Error: Invalid capacity '" << argv[3] << "' for jug C." << endl;
      return 1;
    } else if ((a_cont > cap_a) || (b_cont > cap_b)){
      cerr << "Error: Goal cannot exceed capacity of jug" <<((a_cont > cap_a) ? " A." : " B.") << endl;
      return 1;
    } else if (c_cont > cap_c){
      cerr << "Error: Goal cannot exceed capacity of jug C." << endl;
      return 1;
    }else   if ((a_cont + b_cont + c_cont) !=  cap_c){
      cerr << "Error: Total gallons in goal state must be equal to the capacity of jug C." << endl;
      return 1;
    }
    answer[0] = a_cont;
    answer[1] = b_cont;
    answer[2] = c_cont;
    breadthSearch(0, 0, cap_c);
    return 0;
}
