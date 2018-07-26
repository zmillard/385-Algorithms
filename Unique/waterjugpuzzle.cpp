/*******************************************************************************
 * Name        :waterjugpuzzle.cpp
 * Author      :Conor Manning
 * Date        :March 9, 2016
 * Description :Water Jug Puzzle
 * Pledge      :I pledge my honor that I have abided by the Stevens Honor System
 ******************************************************************************/

#include <iostream>
#include <vector>
#include <string>
#include <cctype>
#include <sstream>
#include <algorithm>

using namespace std;

typedef vector<int> state;

vector< state > previousStates;
vector< state > processQueue;
vector< state > allStates;
bool isPossible = false;
string jugNames[3] = {"A", "B", "C"};

int maxVal1, maxVal2, maxVal3;

void printJugs(state currentState)
{
	//Prints the contents of currentState in a readable format
	cout << "(" << currentState[0] << ", " << currentState[1] << ", " << currentState[2] << ")" << endl;
}

state pour(state currentState, int fromIndex, int toIndex)
{
	//Pours the contents of currentState[fromIndex] to currentState[toIndex]
	//DOES NOT CHANGE currentState. It does this by copying it and not touching anything in currentState.
	int maxVolume;
	state returnState = currentState;
	if(toIndex == 0)
	{
		maxVolume = maxVal1;
	}
	else if(toIndex == 1)
	{
		maxVolume = maxVal2;
	}
	else if(toIndex == 2)
	{
		maxVolume = maxVal3;
	}
	while(returnState[toIndex] < maxVolume && returnState[fromIndex] > 0)
	{
		returnState[toIndex]++;
		returnState[fromIndex]--;
	}
	return returnState;
}

bool canMake(state childState, state parentState)
{
	//Determines whether or not you can make childState with one pour from parentState
	for(int from = 0; from<3; from++)
	{
		for(int to = 0; to<3; to++)
		{
			if(from != to)
			{
				if(pour(parentState, from, to) == childState)
				{
					return true;
				}
			}
		}
	}
	return false;
}

vector< state > traceUpTree()
{
	//Goes through allStates to find the path that the breadth first search took to reach goalState
	vector< state > returnVector;
	int vectorSize = allStates.size()-1;
	state childState = allStates[vectorSize];  //starts with the final State and works backwards
	returnVector.push_back(childState);
	for(int index = vectorSize-1; index > 0; index--)
	{
		if(canMake(childState, allStates[index])) //if childState's parentState is allStates[index]
		{
			returnVector.push_back(allStates[index]);
			childState = allStates[index];
		}
	}
	return returnVector;
}

bool uniqueState(state newState)
{
	//Goes through previousStates to determine whether or not newState has been seen before.
	//Returns true if it is unique, false if it is not.
	for(unsigned int index = 0; index < previousStates.size(); index++)
	{
		state currentState = previousStates[index];
		if(currentState[0] == newState[0] && currentState[1] == newState[1])
		{
			return false;
		}
	}
	return true;
}

void findChildren(state currentState)
{
	//Finds all states that are attainable through one pour from currentState.
	//If that state is unique (aka not in previousStates) then it is added to 
	//both previousStates and processQueue.
	vector<state> allChildren;

	for(int i = 0; i <= 2; i++)
	{
		for(int j = 0; j <= 2; j++)
		{
			if(i!=j && uniqueState(pour(currentState, i, j)))
			{
				allChildren.push_back(pour(currentState, i, j));
			}
		}
	}
	for(unsigned int index = 0; index < allChildren.size(); index++)
	{
		state nextState = allChildren[index];
		if(uniqueState(nextState))
		{
			processQueue.push_back(nextState);
			previousStates.push_back(nextState);
		}
	}
}	

void printTheChange(state parentState, state childState)
{
	//Prints out a string telling the user what pour would have need to been made
	//in order to get from parentState to childState
	int fromIndex, toIndex;
	for(int index = 0; index < 3; index++)
	{
		if(childState[index] < parentState[index])
		{
			fromIndex = index;
		}
		if(parentState[index] < childState[index])
		{
			toIndex = index;
		}
	}
	int change = childState[toIndex] - parentState[toIndex]; //HOW MUCH toIndex INCREASED
	if(change == 1)
	{
		cout << "Pour 1 gallon from " << jugNames[fromIndex] << " to " << jugNames[toIndex] << ". ";
	}
	else
	{
		cout << "Pour " << change <<  " gallons from " << jugNames[fromIndex] << " to " << jugNames[toIndex] << ". ";
	}
}

state makeState(int jug1, int jug2, int jug3)
{
	//Simply creates a state (vector) containing {jug1, jug2, jug3} and returns it
	state newState;
	newState.push_back(jug1);
	newState.push_back(jug2);
	newState.push_back(jug3);
	return newState;
}

int main(int argc, char *argv[])
{
	int cap1, cap2, cap3, goal1, goal2, goal3;

	if(argc!=7)
	{
		cout << "Usage: ./waterjugpuzzle <cap A> <cap B> <cap C> <goal A> <goal B> <goal C>" << endl;
		return -1;
	}

	istringstream iss1(argv[1]), iss2(argv[2]), iss3(argv[3]), iss4(argv[4]), iss5(argv[5]), iss6(argv[6]);

	if(!(iss1>>cap1))
	{
		cout << "Error: Invalid capacity '" << argv[1] << "' for jug A.";
		return -1;
	}
	if(!(iss2>>cap2))
	{
		cout << "Error: Invalid capacity '" << argv[2] << "' for jug B.";
		return -1;
	}
	if(!(iss3>>cap3))
	{
		cout << "Error: Invalid capacity '" << argv[3] << "' for jug C.";
		return -1;
	}
	if(!(iss4>>goal1))
	{
		cout << "Error: Invalid goal '" << argv[4] << "' for jug A.";
		return -1;
	}
	if(!(iss5>>goal2))
	{
		cout << "Error: Invalid goal '" << argv[5] << "' for jug B.";
		return -1;
	}
	if(!(iss6>>goal3))
	{
		cout << "Error: Invalid goal '" << argv[6] << "' for jug C.";
		return -1;
	}

	//IF IT REACHES THIS POINT, ALL OF THE INPUTS ARE NUMBERS.
	int inputArray[6] = {cap1, cap2, cap3, goal1, goal2, goal3};
	for(int index = 0; index < 3; index++)
	{
		if(inputArray[index] < 1)
		{
			cout << "Error: Invalid capacity '" << inputArray[index] << "' for jug " << jugNames[index] << "." << endl;
			return -1;
		}
	}
	for(int index = 3; index < 6; index++)
	{
		if(inputArray[index] < 0)
		{
			cout << "Error: Invalid goal '" << inputArray[index] << "' for jug " << jugNames[index-3] << "." << endl;
			return -1;
		}
	}
	for(int index = 0; index < 3; index++)
	{
		if(inputArray[index] < inputArray[index+3])
		{
			cout << "Error: Goal cannot exceed capacity of jug " << jugNames[index] << "." << endl;
			return -1;
		}
	}
	if(goal1+goal2+goal3 != cap3)
	{
		cout << "Error: Total gallons in goal state must be equal to the capacity of jug C."<<endl;
		return -1;
	}
	
	maxVal1 = cap1, maxVal2 = cap2, maxVal3 = cap3;
	state startState = makeState(0, 0, goal1+goal2+goal3);
	state goalState = makeState(goal1, goal2, goal3);
	if(startState==goalState)
	{
		cout << "Initial state. ";
		printJugs(startState);
		return 0;
	}
	processQueue.push_back(startState);
	while(processQueue.size()!=0)
	{
		state nextState = processQueue[0];
		allStates.push_back(nextState);
		if(nextState == goalState)
		{
			isPossible = true;
			break;
		}
		findChildren(nextState);
		processQueue.erase(processQueue.begin());
	}
	if(!isPossible)
	{
		cout << "No solution.";
		return 0;
	}

	//IF IT GETS HERE, THERE IS A PATH TO goalState
	cout << "Initial state. ";
	printJugs(startState);
	vector< state > treeTrace = traceUpTree();
	reverse(treeTrace.begin(), treeTrace.end()); //makes it so that treeTrace has the root at the top and goalState last
	treeTrace.erase(treeTrace.begin());			 //removes the root from treeTrace
	printTheChange(startState, treeTrace[0]);	 //displays the first move that was made
	printJugs(treeTrace[0]);	
	for(unsigned int index = 0; index<treeTrace.size()-1; index++)
	{
		printTheChange(treeTrace[index], treeTrace[index+1]);
		printJugs(treeTrace[index+1]);
	}
	return 0;
}