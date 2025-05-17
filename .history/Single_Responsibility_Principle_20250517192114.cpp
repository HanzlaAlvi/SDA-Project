#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

// ==================== Data Structure ====================
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

// ==================== File Handler ====================
class FileHandler {
public:
    static void saveToFile(const vector<Student>& students, const string& filename) {
        ofstream file(filename);
        for (const Student& s : students) {
            file << s.name << " " << s.rollNo << " " << s.studentClass << " " 
                 << s.age << " " << s.gender;
            for (float mark : s.marks) file << " " << mark;
            file << " " << s.attendance << "\n";
        }
        file.close();
    }

    static vector<Student> loadFromFile(const string& filename) {
        vector<Student> students;
        ifstream file(filename);
        Student s;
        while (file >> s.name >> s.rollNo >> s.studentClass >> s.age >> s.gender >> 
               s.marks[0] >> s.marks[1] >> s.marks[2] >> s.marks[3] >> s.marks[4] >> s.attendance) {
            students.push_back(s);
        }
        file.close();
        return students;
    }
};

// ==================== Grade Calculator ====================
class GradeCalculator {
public:
    static void calculateGrade(Student &s) {
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

    static void showGPA(const vector<Student>& students) {
        for (const Student& s : students) {
            cout << s.name << " GPA: " << s.percentage / 20 << "\n";
        }
    }
};

// ==================== Student Manager ====================
class StudentManager {
private:
    vector<Student> students;

public:
    StudentManager() {
        students = FileHandler::loadFromFile("students.txt");
        for (Student& s : students) {
            GradeCalculator::calculateGrade(s);
        }
    }

    void addStudent() {
        Student s;
        cout << "Enter name: "; 
        cin.ignore();
        getline(cin, s.name);
        cout << "Enter roll number: "; cin >> s.rollNo;
        cout << "Enter class: "; 
        cin.ignore();
        getline(cin, s.studentClass);
        cout << "Enter age: "; cin >> s.age;
        cout << "Enter gender: "; 
        cin.ignore();
        getline(cin, s.gender);
        s.attendance = "Not Marked";
        for (int i = 0; i < 5; ++i) s.marks[i] = 0;
        GradeCalculator::calculateGrade(s);
        students.push_back(s);
        cout << "Student added successfully.\n";
    }

    void viewAllStudents() const {
        cout << left << setw(10) << "Roll" << setw(20) << "Name" << setw(10) << "Class" 
             << setw(6) << "Age" << setw(10) << "Gender" << setw(12) << "Percent" 
             << setw(8) << "Grade" << "Attendance\n";
        for (const Student& s : students) {
            cout << setw(10) << s.rollNo << setw(20) << s.name << setw(10) << s.studentClass 
                 << setw(6) << s.age << setw(10) << s.gender << setw(12) << fixed << setprecision(2) 
                 << s.percentage << setw(8) << s.grade << s.attendance << "\n";
        }
    }

    void searchStudent() const {
        int roll;
        cout << "Enter roll number to search: "; cin >> roll;
        for (const Student& s : students) {
            if (s.rollNo == roll) {
                cout << "\nStudent Found:\n";
                cout << "Name: " << s.name << "\nClass: " << s.studentClass 
                     << "\nAge: " << s.age << "\nGender: " << s.gender 
                     << "\nPercentage: " << s.percentage << "\nGrade: " << s.grade << "\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    void updateStudent() {
        int roll;
        cout << "Enter roll number to update: "; cin >> roll;
        for (Student& s : students) {
            if (s.rollNo == roll) {
                cout << "Enter new name: "; 
                cin.ignore();
                getline(cin, s.name);
                cout << "Enter new class: "; 
                getline(cin, s.studentClass);
                cout << "Enter new age: "; cin >> s.age;
                cout << "Enter new gender: "; 
                cin.ignore();
                getline(cin, s.gender);
                GradeCalculator::calculateGrade(s);
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
        for (Student& s : students) {
            cout << "Mark attendance for " << s.name << " (P/A): ";
            char a; cin >> a;
            s.attendance = (toupper(a) == 'P') ? "Present" : "Absent";
        }
    }

    void enterMarks() {
        int roll;
        cout << "Enter roll number to enter marks: "; cin >> roll;
        for (Student& s : students) {
            if (s.rollNo == roll) {
                cout << "Enter marks for  subjects (separated by space): ";
                for (int i = 0; i < 5; ++i) cin >> s.marks[i];
                GradeCalculator::calculateGrade(s);
                cout << "Marks entered and grade updated.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    void classReport() const {
        string cls;
        cout << "Enter class to view report: "; 
        cin.ignore();
        getline(cin, cls);
        cout << "\nClass Report for " << cls << ":\n";
        cout << "---------------------------------\n";
        bool found = false;
        for (const Student& s : students) {
            if (s.studentClass == cls) {
                cout << s.name << " (Roll: " << s.rollNo << ") - Grade: " << s.grade 
                     << " (" << s.percentage << "%)\n";
                found = true;
            }
        }
        if (!found) cout << "No students found in this class.\n";
    }

    void exportToCSV() const {
        ofstream file("students.csv");
        file << "Roll No,Name,Class,Age,Gender,Percentage,Grade,Attendance\n";
        for (const Student& s : students) {
            file << s.rollNo << "," << s.name << "," << s.studentClass << "," << s.age 
                 << "," << s.gender << "," << s.percentage << "," << s.grade 
                 << "," << s.attendance << "\n";
        }
        cout << "Data exported to students.csv\n";
        file.close();
    }

    void sortStudents() {
        sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.name < b.name;
        });
        cout << "Students sorted alphabetically.\n";
    }

    void saveData() const {
        FileHandler::saveToFile(students, "students.txt");
        cout << "Data saved successfully.\n";
    }

    const vector<Student>& getStudents() const { return students; }
};

// ==================== Authentication ====================
class Authenticator {
public:
    static bool login() {
        string user, pass;
        cout << "==== Login ====\n";
        cout << "Username: "; cin >> user;
        cout << "Password: "; cin >> pass;
        return (user == "admin" && pass == "admin123");
    }
};

// ==================== Menu System ====================
class MenuSystem {
private:
    StudentManager manager;

    void displayMenu() {
        cout << "\n==== Student Management System ====\n";
        cout << "1. Add New Student\n";
        cout << "2. View All Students\n";
        cout << "3. Search Student\n";
        cout << "4. Update Student Details\n";
        cout << "5. Delete Student\n";
        cout << "6. Mark Attendance\n";
        cout << "7. Enter Marks\n";
        cout << "8. Calculate GPA\n";
        cout << "9. Class Report\n";
        cout << "10. Export to CSV\n";
        cout << "11. Sort Students\n";
        cout << "12. Save & Exit\n";
        cout << "Enter your choice (1-12): ";
    }

public:
    void run() {
        if (!Authenticator::login()) {
            cout << "Access Denied! Invalid credentials.\n";
            return;
        }

        int choice;
        do {
            displayMenu();
            cin >> choice;
            handleChoice(choice);
        } while (choice != 12);
    }

private:
    void handleChoice(int choice) {
        switch (choice) {
            case 1: manager.addStudent(); break;
            case 2: manager.viewAllStudents(); break;
            case 3: manager.searchStudent(); break;
            case 4: manager.updateStudent(); break;
            case 5: manager.deleteStudent(); break;
            case 6: manager.markAttendance(); break;
            case 7: manager.enterMarks(); break;
            case 8: GradeCalculator::showGPA(manager.getStudents()); break;
            case 9: manager.classReport(); break;
            case 10: manager.exportToCSV(); break;
            case 11: manager.sortStudents(); break;
            case 12: manager.saveData(); break;
            default: cout << "Invalid choice! Please try again.\n";
        }
    }
};

// ==================== Main Function ====================
int main() {
    MenuSystem system;
    system.run();
    cout << "Thank you for using Student Management System!\n";
    return 0;
}