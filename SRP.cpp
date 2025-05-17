#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

// ===== Student Data Model =====
struct Student
{
    string name;
    int rollNo;
    string studentClass;
    int age;
    string gender;
    float marks[5] = {};
    float percentage = 0;
    char grade = 'F';
    string attendance = "Not Marked";
};

// ===== Managers =====
class GradeCalculator
{
public:
    static void calculateGrade(Student &s)
    {
        float total = 0;
        for (int i = 0; i < 5; ++i)
            total += s.marks[i];
        s.percentage = total / 5;
        if (s.percentage >= 90)
            s.grade = 'A';
        else if (s.percentage >= 80)
            s.grade = 'B';
        else if (s.percentage >= 70)
            s.grade = 'C';
        else if (s.percentage >= 60)
            s.grade = 'D';
        else
            s.grade = 'F';
    }
};

class FileHandler
{
public:
    static void saveToFile(const vector<Student> &students)
    {
        ofstream file("students.txt");
        for (Student s : students)
        {
            file << s.name << " " << s.rollNo << " " << s.studentClass << " " << s.age << " " << s.gender;
            for (float mark : s.marks)
                file << " " << mark;
            file << " " << s.attendance << "\n";
        }
        file.close();
    }

    static void loadFromFile(vector<Student> &students)
    {
        ifstream file("students.txt");
        students.clear();
        Student s;
        while (file >> s.name >> s.rollNo >> s.studentClass >> s.age >> s.gender >> s.marks[0] >> s.marks[1] >> s.marks[2] >> s.marks[3] >> s.marks[4] >> s.attendance)
        {
            GradeCalculator::calculateGrade(s);
            students.push_back(s);
        }
        file.close();
    }
};

class AuthManager
{
public:
    static void login()
    {
        string user, pass;
        cout << "Enter username: ";
        cin >> user;
        cout << "Enter password: ";
        cin >> pass;
        if (user != "admin" || pass != "1234")
        {
            cout << "Access Denied!\n";
            exit(0);
        }
    }
};

class AttendanceManager
{
public:
    static void markAttendance(vector<Student> &students)
    {
        for (Student &s : students)
        {
            cout << "Mark attendance for " << s.name << " (P/A): ";
            char a;
            cin >> a;
            s.attendance = (a == 'P' || a == 'p') ? "Present" : "Absent";
        }
    }
};

class MarksManager
{
public:
    static void enterMarks(vector<Student> &students)
    {
        int roll;
        cout << "Enter roll number to enter marks: ";
        cin >> roll;
        for (Student &s : students)
        {
            if (s.rollNo == roll)
            {
                cout << "Enter 5 subject marks: ";
                for (int i = 0; i < 5; ++i)
                    cin >> s.marks[i];
                GradeCalculator::calculateGrade(s);
                cout << "Marks entered and grade updated.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    static void calculateGPA(const vector<Student> &students)
    {
        for (Student s : students)
        {
            cout << s.name << " GPA: " << s.percentage / 20 << "\n";
        }
    }
};

class ReportManager
{
public:
    static void classReport(const vector<Student> &students)
    {
        string cls;
        cout << "Enter class to view report: ";
        cin >> cls;
        for (Student s : students)
        {
            if (s.studentClass == cls)
            {
                cout << s.name << " (" << s.rollNo << ") - Grade: " << s.grade << "\n";
            }
        }
    }

    static void exportToCSV(const vector<Student> &students)
    {
        ofstream file("students.csv");
        file << "Roll,Name,Class,Age,Gender,Percentage,Grade,Attendance\n";
        for (Student s : students)
        {
            file << s.rollNo << "," << s.name << "," << s.studentClass << "," << s.age << "," << s.gender << "," << s.percentage << "," << s.grade << "," << s.attendance << "\n";
        }
        file.close();
        cout << "Exported to students.csv\n";
    }
};

class StudentManager
{
public:
    static void addStudent(vector<Student> &students)
    {
        Student s;
        cout << "Enter name: ";
        cin >> s.name;
        cout << "Enter roll number: ";
        cin >> s.rollNo;
        cout << "Enter class: ";
        cin >> s.studentClass;
        cout << "Enter age: ";
        cin >> s.age;
        cout << "Enter gender: ";
        cin >> s.gender;
        GradeCalculator::calculateGrade(s);
        students.push_back(s);
        cout << "Student added successfully.\n";
    }

    static void viewAllStudents(const vector<Student> &students)
    {
        cout << left << setw(10) << "Roll" << setw(15) << "Name" << setw(10) << "Class" << setw(6) << "Age" << setw(10) << "Gender" << setw(12) << "Percent" << setw(8) << "Grade" << "Attendance\n";
        for (Student s : students)
        {
            cout << setw(10) << s.rollNo << setw(15) << s.name << setw(10) << s.studentClass << setw(6) << s.age << setw(10) << s.gender << setw(12) << s.percentage << setw(8) << s.grade << s.attendance << "\n";
        }
    }

    static void searchStudent(const vector<Student> &students)
    {
        int roll;
        cout << "Enter roll number to search: ";
        cin >> roll;
        for (Student s : students)
        {
            if (s.rollNo == roll)
            {
                cout << "Name: " << s.name << ", Class: " << s.studentClass << ", Age: " << s.age << ", Gender: " << s.gender << ", Grade: " << s.grade << "\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    static void updateStudent(vector<Student> &students)
    {
        int roll;
        cout << "Enter roll number to update: ";
        cin >> roll;
        for (Student &s : students)
        {
            if (s.rollNo == roll)
            {
                cout << "Enter new name: ";
                cin >> s.name;
                cout << "Enter new class: ";
                cin >> s.studentClass;
                cout << "Enter new age: ";
                cin >> s.age;
                cout << "Enter new gender: ";
                cin >> s.gender;
                cout << "Updated successfully.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    static void deleteStudent(vector<Student> &students)
    {
        int roll;
        cout << "Enter roll number to delete: ";
        cin >> roll;
        auto it = remove_if(students.begin(), students.end(), [roll](Student s)
                            { return s.rollNo == roll; });
        if (it != students.end())
        {
            students.erase(it, students.end());
            cout << "Deleted successfully.\n";
        }
        else
        {
            cout << "Student not found.\n";
        }
    }

    static void sortStudents(vector<Student> &students)
    {
        sort(students.begin(), students.end(), [](Student a, Student b)
             { return a.name < b.name; });
        cout << "Sorted by name.\n";
    }
};

// ===== Menu Handler =====
class Menu
{
public:
    static void run()
    {
        vector<Student> students;
        FileHandler::loadFromFile(students);
        int choice;
        do
        {
            cout << "\n--- Student Management System ---\n";
            cout << "1. Add Student\n2. View All Students\n3. Search Student\n4. Update Student\n5. Delete Student\n6. Mark Attendance\n7. Enter Marks\n8. Calculate GPA\n9. Class Report\n10. Export to CSV\n11. Sort Students\n12. Save & Exit\nChoice: ";
            cin >> choice;
            switch (choice)
            {
            case 1:
                StudentManager::addStudent(students);
                break;
            case 2:
                StudentManager::viewAllStudents(students);
                break;
            case 3:
                StudentManager::searchStudent(students);
                break;
            case 4:
                StudentManager::updateStudent(students);
                break;
            case 5:
                StudentManager::deleteStudent(students);
                break;
            case 6:
                AttendanceManager::markAttendance(students);
                break;
            case 7:
                MarksManager::enterMarks(students);
                break;
            case 8:
                MarksManager::calculateGPA(students);
                break;
            case 9:
                ReportManager::classReport(students);
                break;
            case 10:
                ReportManager::exportToCSV(students);
                break;
            case 11:
                StudentManager::sortStudents(students);
                break;
            case 12:
                FileHandler::saveToFile(students);
                cout << "Exiting...\n";
                break;
            default:
                cout << "Invalid choice.\n";
            }
        } while (choice != 12);
    }
};

// ===== Main =====
int main()
{
    AuthManager::login();
    Menu::run();
    return 0;
}
