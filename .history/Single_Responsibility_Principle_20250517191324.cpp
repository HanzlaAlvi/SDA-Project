// Student Management System with Single Responsibility Principle
#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
using namespace std;

/* ==================== Data Structure ==================== */
// Contains only student data fields (no logic)
struct Student
{
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

/* ==================== File Handler Class ==================== */
// ONLY handles file operations (loading/saving student data)
class FileHandler
{
public:
    // Save student data to file
    static void saveToFile(const vector<Student> &students, const string &filename)
    {
        ofstream file(filename);
        for (const Student &s : students)
        {
            file << s.name << " " << s.rollNo << " " << s.studentClass << " "
                 << s.age << " " << s.gender;
            for (float mark : s.marks)
                file << " " << mark;
            file << " " << s.attendance << "\n";
        }
        file.close();
    }

    // Load student data from file
    static vector<Student> loadFromFile(const string &filename)
    {
        vector<Student> students;
        ifstream file(filename);
        Student s;
        while (file >> s.name >> s.rollNo >> s.studentClass >> s.age >> s.gender >>
               s.marks[0] >> s.marks[1] >> s.marks[2] >> s.marks[3] >> s.marks[4] >> s.attendance)
        {
            students.push_back(s);
        }
        file.close();
        return students;
    }
};

/* ==================== Grade Calculator Class ==================== */
// ONLY handles grade/GPA calculations
class GradeCalculator
{
public:
    // Calculate grade for single student
    static void calculateGrade(Student &s)
    {
        float total = 0;
        for (int i = 0; i < 5; ++i)
        {
            total += s.marks[i];
        }
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

    // Calculate GPA for all students
    static void calculateGPA(const vector<Student> &students)
    {
        for (const Student &s : students)
        {
            cout << s.name << " GPA: " << s.percentage / 20 << "\n";
        }
    }
};

/* ==================== Student Manager Class ==================== */
// ONLY handles student data management operations
class StudentManager
{
private:
    vector<Student> students;

public:
    // Constructor loads data automatically
    StudentManager()
    {
        students = FileHandler::loadFromFile("students.txt");
        for (Student &s : students)
        {
            GradeCalculator::calculateGrade(s);
        }
    }

    // Add new student
    void addStudent()
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
        s.attendance = "Not Marked";
        for (int i = 0; i < 5; ++i)
            s.marks[i] = 0;
        GradeCalculator::calculateGrade(s);
        students.push_back(s);
        cout << "Student added successfully.\n";
    }

    // View all students
    void viewAllStudents() const
    {
        cout << left << setw(10) << "Roll" << setw(15) << "Name" << setw(10) << "Class"
             << setw(6) << "Age" << setw(10) << "Gender" << setw(12) << "Percent"
             << setw(8) << "Grade" << "Attendance\n";
        for (const Student &s : students)
        {
            cout << setw(10) << s.rollNo << setw(15) << s.name << setw(10) << s.studentClass
                 << setw(6) << s.age << setw(10) << s.gender << setw(12) << s.percentage
                 << setw(8) << s.grade << s.attendance << "\n";
        }
    }

    // Other student operations (search, update, delete, etc.)
    // ... (implementation remains similar to original but uses the new structure)

    void saveData() const
    {
        FileHandler::saveToFile(students, "students.txt");
    }

    const vector<Student> &getStudents() const { return students; }
};

/* ==================== Authentication Class ==================== */
// ONLY handles login functionality
class Authenticator
{
public:
    static bool login()
    {
        string user, pass;
        cout << "Enter username: ";
        cin >> user;
        cout << "Enter password: ";
        cin >> pass;
        return (user == "admin" && pass == "1234");
    }
};

/* ==================== Menu Class ==================== */
// ONLY handles user interface and menu navigation
class Menu
{
private:
    StudentManager manager;

public:
    void showMenu()
    {
        int choice;
        do
        {
            displayOptions();
            cin >> choice;
            handleChoice(choice);
        } while (choice != 12);
    }

private:
    void displayOptions()
    {
        cout << "\n--- Student Management System ---\n";
        cout << "1. Add Student\n2. View All Students\n3. Search Student\n"
             << "4. Update Student\n5. Delete Student\n6. Mark Attendance\n"
             << "7. Enter Marks\n8. Calculate GPA\n9. Class Report\n"
             << "10. Export to CSV\n11. Sort Students\n12. Save & Exit\nChoice: ";
    }

    void handleChoice(int choice)
    {
        switch (choice)
        {
        case 1:
            manager.addStudent();
            break;
        case 2:
            manager.viewAllStudents();
            break;
        
        // ... (other cases)
        case 12:
            manager.saveData();
            cout << "Exiting...\n";
            break;
        default:
            cout << "Invalid choice.\n";
        }
    }
};

/* ==================== Main Function ==================== */
int main()
{
    if (!Authenticator::login())
    {
        cout << "Access Denied!\n";
        return 0;
    }

    Menu menu;
    menu.showMenu();
    return 0;
}