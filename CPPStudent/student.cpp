/*
k * student.cpp
 * Zoe Millard
 * "I pledge my honor that I have abided by the Stevens Honor System." -ZM
 */
#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
using namespace std;

class Student{
public:
	Student(const string &first_name, const string last_name, float gpa, int id):
		first_(first_name), last_(last_name), gpa_(gpa), id_(id){ }

	string full_name() const{
		return first_ +" "+ last_;
	}
	float get_gpa() const{
		return gpa_;
	}
	void print_info() const {

		cout << setprecision(2) << fixed;
		std::cout << first_ << " " << last_ <<", GPA: " << gpa_ * 1.00<<", ID: " << id_ <<std::endl;
	}
private:
	string first_;
	string last_;
	float gpa_;
	int id_;
};
vector<Student> find_failing_students(const vector<Student> &students) {
	vector<Student> failing_students;
	// Always make sure you use the pre-increment operator when working with
	// iterators. Declare iterator inside loop to reduce its scope.
	for (vector<Student>::const_iterator it = students.begin(); it != students.end(); ++it) {
		if(it->get_gpa() < 1){
			failing_students.push_back(*it);
		}
		// TODO
		// Use .push_back(*it) to add a student to the vector if his or her gpa is
		// less than 1.0.
// Remember 'it' is a pointer, so in order to get the gpa, you must use '->'
		// instead of '.'
		/*if
		.push_back(*it)*/
	}
	return failing_students;
}
void print_students(const vector<Student> &students) {
 // Same way we iterated through the vector in the previous function, except
 // calling print_info() for each student
	for(vector<Student>::const_iterator it = students.begin(); it != students.end(); ++it){
		it->print_info();
	}
}
int main() {
	string first_name, last_name;
	float gpa;
	int id;
	char repeat;
	vector<Student> students;
	do {
		cout << "Enter student's first name: ";
		cin >> first_name;
		cout << "Enter student's last name: ";
		cin >> last_name;
		gpa = -1;
		while (gpa < 0 || gpa > 4) {
			cout << "Enter student's GPA (0.0-4.0): ";
			cin >> gpa;
		}
		cout << "Enter student's ID: ";
		cin >> id;
		students.push_back(Student(first_name, last_name, gpa, id));
		cout << "Add another student to database (Y/N)? ";
		cin >> repeat;
	} while (repeat == 'Y' || repeat == 'y');
	cout << endl << "All students:" << endl;
	print_students(students);
	cout << endl << "Failing students:";
	// TODO
	if(find_failing_students(students).empty()){
		cout << " None" <<endl;
	}else{
		cout << "" << endl;
		print_students(find_failing_students(students));
	}
	// Print a space and the word 'None' on the same line if no students are
	// failing.
	// Otherwise, print each failing student on a separate line.
	return 0;
}



