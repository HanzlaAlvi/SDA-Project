// Student Management System in C++ with 15 Features
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

struct Student {
    string name;
    int rollNo;
    string studentClass;
    int age;
    string gender;
    float marks[5];
    float percentage;
    char grade;
    string attendance;
};

vector<Student> students;

// ---------- Function Prototypes ----------
void addStudent();
void viewAllStudents();
void searchStudent();
void updateStudent();
void deleteStudent();
void markAttendance();
void enterMarks();
void calculateGrade(Student &s);
void calculateGPA();
void classReport();
void saveToFile();
void loadFromFile();
void login();
void exportToCSV();
void sortStudents();
void menu();

// ---------- Helper Functions ----------
void calculateGrade(Student &s) {
    float total = 0;
    for (int i = 0; i < 5; ++i) {
        total += s.marks[i];
    }
    s.percentage = total / 5;
    if (s.percentage >= 90) s.grade = 'A';
    else if (s.percentage >= 80) s.grade = 'B';
    else if (s.percentage >= 70) s.grade = 'C';
    else if (s.percentage >= 60) s.grade = 'D';
    else s.grade = 'F';
}

void saveToFile() {
    ofstream file("students.txt");
    for (Student s : students) {
        file << s.name << " " << s.rollNo << " " << s.studentClass << " " << s.age << " " << s.gender;
        for (float mark : s.marks) file << " " << mark;
        file << " " << s.attendance << "\n";
    }
    file.close();
}

void loadFromFile() {
    ifstream file("students.txt");
    students.clear();
    Student s;
    while (file >> s.name >> s.rollNo >> s.studentClass >> s.age >> s.gender >> s.marks[0] >> s.marks[1] >> s.marks[2] >> s.marks[3] >> s.marks[4] >> s.attendance) {
        calculateGrade(s);
        students.push_back(s);
    }
    file.close();
}

// ---------- Feature Functions ----------
void addStudent() {
    Student s;
    cout << "Enter name: "; cin >> s.name;
    cout << "Enter roll number: "; cin >> s.rollNo;
    cout << "Enter class: "; cin >> s.studentClass;
    cout << "Enter age: "; cin >> s.age;
    cout << "Enter gender: "; cin >> s.gender;
    s.attendance = "Not Marked";
    for (int i = 0; i < 5; ++i) s.marks[i] = 0;
    calculateGrade(s);
    students.push_back(s);
    cout << "Student added successfully.\n";
}

void viewAllStudents() {
    cout << left << setw(10) << "Roll" << setw(15) << "Name" << setw(10) << "Class" << setw(6) << "Age" << setw(10) << "Gender" << setw(12) << "Percent" << setw(8) << "Grade" << "Attendance\n";
    for (Student s : students) {
        cout << setw(10) << s.rollNo << setw(15) << s.name << setw(10) << s.studentClass << setw(6) << s.age << setw(10) << s.gender << setw(12) << s.percentage << setw(8) << s.grade << s.attendance << "\n";
    }
}

void searchStudent() {
    int roll;
    cout << "Enter roll number to search: "; cin >> roll;
    for (Student s : students) {
        if (s.rollNo == roll) {
            cout << "Name: " << s.name << ", Class: " << s.studentClass << ", Age: " << s.age << ", Gender: " << s.gender << ", Grade: " << s.grade << "\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void updateStudent() {
    int roll;
    cout << "Enter roll number to update: "; cin >> roll;
    for (Student &s : students) {
        if (s.rollNo == roll) {
            cout << "Enter new name: "; cin >> s.name;
            cout << "Enter new class: "; cin >> s.studentClass;
            cout << "Enter new age: "; cin >> s.age;
            cout << "Enter new gender: "; cin >> s.gender;
            cout << "Updated successfully.\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void deleteStudent() {
    int roll;
    cout << "Enter roll number to delete: "; cin >> roll;
    for (auto it = students.begin(); it != students.end(); ++it) {
        if (it->rollNo == roll) {
            students.erase(it);
            cout << "Deleted successfully.\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void markAttendance() {
    for (Student &s : students) {
        cout << "Mark attendance for " << s.name << " (P/A): ";
        char a; cin >> a;
        s.attendance = (a == 'P' || a == 'p') ? "Present" : "Absent";
    }
}

void enterMarks() {
    int roll;
    cout << "Enter roll number to enter marks: "; cin >> roll;
    for (Student &s : students) {
        if (s.rollNo == roll) {
            cout << "Enter 5 subject marks: ";
            for (int i = 0; i < 5; ++i) cin >> s.marks[i];
            calculateGrade(s);
            cout << "Marks entered and grade updated.\n";
            return;
        }
    }
    cout << "Student not found.\n";
}

void calculateGPA() {
    for (Student s : students) {
        cout << s.name << " GPA: " << s.percentage / 20 << "\n";
    }
}

void classReport() {
    string cls;
    cout << "Enter class to view report: "; cin >> cls;
    for (Student s : students) {
        if (s.studentClass == cls) {
            cout << s.name << " (" << s.rollNo << ") - Grade: " << s.grade << "\n";
        }
    }
}

void exportToCSV() {
    ofstream file("students.csv");
    file << "Roll,Name,Class,Age,Gender,Percentage,Grade,Attendance\n";
    for (Student s : students) {
        file << s.rollNo << "," << s.name << "," << s.studentClass << "," << s.age << "," << s.gender << "," << s.percentage << "," << s.grade << "," << s.attendance << "\n";
    }
    cout << "Exported to students.csv\n";
    file.close();
}

void sortStudents() {
    sort(students.begin(), students.end(), [](Student a, Student b) {
        return a.name < b.name;
    });
    cout << "Sorted by name.\n";
}

void login() {
    string user, pass;
    cout << "Enter username: "; cin >> user;
    cout << "Enter password: "; cin >> pass;
    if (user != "admin" || pass != "1234") {
        cout << "Access Denied!\n";
        exit(0);
    }
}

// ---------- Main Menu ----------
void menu() {
    loadFromFile();
    int choice;
    do {
        cout << "\n--- Student Management System ---\n";
        cout << "1. Add Student\n2. View All Students\n3. Search Student\n4. Update Student\n5. Delete Student\n6. Mark Attendance\n7. Enter Marks\n8. Calculate GPA\n9. Class Report\n10. Export to CSV\n11. Sort Students\n12. Save & Exit\nChoice: ";
        cin >> choice;
        switch (choice) {
            case 1: addStudent(); break;
            case 2: viewAllStudents(); break;
            case 3: searchStudent(); break;
            case 4: updateStudent(); break;
            case 5: deleteStudent(); break;
            case 6: markAttendance(); break;
            case 7: enterMarks(); break;
            case 8: calculateGPA(); break;
            case 9: classReport(); break;
            case 10: exportToCSV(); break;
            case 11: sortStudents(); break;
            case 12: saveToFile(); cout << "Exiting...\n"; break;
            default: cout << "Invalid choice.\n";
        }
    } while (choice != 12);
}

int main() {
    login();
    menu();
    re
}
