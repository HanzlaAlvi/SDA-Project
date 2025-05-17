#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <map>
#include <functional>
using namespace std;

// Student Structure
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

// Grade Strategy Interface
class IGradeStrategy
{
public:
    virtual char calculateGrade(float percentage) = 0;
    virtual ~IGradeStrategy() {}
};

// Default Grading Strategy (Example)
class DefaultGradeStrategy : public IGradeStrategy
{
public:
    char calculateGrade(float percentage) override
    {
        if (percentage >= 90)
            return 'A';
        else if (percentage >= 80)
            return 'B';
        else if (percentage >= 70)
            return 'C';
        else if (percentage >= 60)
            return 'D';
        else
            return 'F';
    }
};

// Grade Calculator Using Strategy
class GradeCalculator
{
    IGradeStrategy *strategy;

public:
    GradeCalculator(IGradeStrategy *strat) : strategy(strat) {}
    void calculateGrade(Student &s)
    {
        float total = 0;
        for (int i = 0; i < 5; ++i)
            total += s.marks[i];
        s.percentage = total / 5;
        s.grade = strategy->calculateGrade(s.percentage);
    }
};

// File Handling (Load/Save)
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

    static void loadFromFile(vector<Student> &students, GradeCalculator &gradeCalc)
    {
        ifstream file("students.txt");
        students.clear();
        Student s;
        while (file >> s.name >> s.rollNo >> s.studentClass >> s.age >> s.gender >> s.marks[0] >> s.marks[1] >> s.marks[2] >> s.marks[3] >> s.marks[4] >> s.attendance)
        {
            gradeCalc.calculateGrade(s);
            students.push_back(s);
        }
        file.close();
    }
};

// Authentication
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

// Attendance Management
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

// Marks Management
class MarksManager
{
    GradeCalculator &gradeCalc;

public:
    MarksManager(GradeCalculator &gc) : gradeCalc(gc) {}

    void enterMarks(vector<Student> &students)
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
                gradeCalc.calculateGrade(s);
                cout << "Marks entered and grade updated.\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    void calculateGPA(const vector<Student> &students)
    {
        for (const Student &s : students)
        {
            cout << s.name << " GPA: " << s.percentage / 20 << "\n";
        }
    }
};

// Exporter Interface & CSV Exporter (for OCP)
class IExporter
{
public:
    virtual void exportData(const vector<Student> &students) = 0;
    virtual ~IExporter() {}
};

class CSVExporter : public IExporter
{
public:
    void exportData(const vector<Student> &students) override
    {
        ofstream file("students.csv");
        file << "Roll,Name,Class,Age,Gender,Percentage,Grade,Attendance\n";
        for (const Student &s : students)
        {
            file << s.rollNo << "," << s.name << "," << s.studentClass << "," << s.age << "," << s.gender << "," << s.percentage << "," << s.grade << "," << s.attendance << "\n";
        }
        file.close();
        cout << "Exported to students.csv\n";
    }
};

// Report Management
class ReportManager
{
    IExporter *exporter;

public:
    ReportManager(IExporter *exp) : exporter(exp) {}

    void classReport(const vector<Student> &students)
    {
        string cls;
        cout << "Enter class to view report: ";
        cin >> cls;
        for (const Student &s : students)
        {
            if (s.studentClass == cls)
            {
                cout << s.name << " (" << s.rollNo << ") - Grade: " << s.grade << "\n";
            }
        }
    }

    void exportReport(const vector<Student> &students)
    {
        exporter->exportData(students);
    }
};

// Student Management
class StudentManager
{
    GradeCalculator &gradeCalc;

public:
    StudentManager(GradeCalculator &gc) : gradeCalc(gc) {}

    void addStudent(vector<Student> &students)
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
        gradeCalc.calculateGrade(s);
        students.push_back(s);
        cout << "Student added successfully.\n";
    }

    void viewAllStudents(const vector<Student> &students)
    {
        cout << left << setw(10) << "Roll" << setw(15) << "Name" << setw(10) << "Class" << setw(6) << "Age" << setw(10) << "Gender" << setw(12) << "Percent" << setw(8) << "Grade" << "Attendance\n";
        for (const Student &s : students)
        {
            cout << setw(10) << s.rollNo << setw(15) << s.name << setw(10) << s.studentClass << setw(6) << s.age << setw(10) << s.gender << setw(12) << s.percentage << setw(8) << s.grade << s.attendance << "\n";
        }
    }

    void searchStudent(const vector<Student> &students)
    {
        int roll;
        cout << "Enter roll number to search: ";
        cin >> roll;
        for (const Student &s : students)
        {
            if (s.rollNo == roll)
            {
                cout << "Name: " << s.name << ", Class: " << s.studentClass << ", Age: " << s.age << ", Gender: " << s.gender << ", Grade: " << s.grade << "\n";
                return;
            }
        }
        cout << "Student not found.\n";
    }

    void updateStudent(vector<Student> &students)
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

    void deleteStudent(vector<Student> &students)
    {
        int roll;
        cout << "Enter roll number to delete: ";
        cin >> roll;
        auto it = remove_if(students.begin(), students.end(), [roll](const Student &s)
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

    void sortStudents(vector<Student> &students)
    {
        sort(students.begin(), students.end(), [](const Student &a, const Student &b)
             { return a.rollNo < b.rollNo; });
        cout << "Sorted by roll number.\n";
    }
};

// Menu with OCP friendly design
class Menu
{
    vector<Student> students;
    GradeCalculator gradeCalc;
    StudentManager studentMgr;
    MarksManager marksMgr;
    AttendanceManager attendanceMgr;
    ReportManager *reportMgr;
    CSVExporter csvExporter;

    map<int, function<void()>> actions;

public:
    Menu() : gradeCalc(new DefaultGradeStrategy()),
             studentMgr(gradeCalc),
             marksMgr(gradeCalc),
             reportMgr(nullptr)
    {
        reportMgr = new ReportManager(&csvExporter);
        FileHandler::loadFromFile(students, gradeCalc);

        actions[1] = [&]()
        { studentMgr.addStudent(students); };
        actions[2] = [&]()
        { studentMgr.viewAllStudents(students); };
        actions[3] = [&]()
        { studentMgr.searchStudent(students); };
        actions[4] = [&]()
        { studentMgr.updateStudent(students); };
        actions[5] = [&]()
        { studentMgr.deleteStudent(students); };
        actions[6] = [&]()
        { marksMgr.enterMarks(students); };
        actions[7] = [&]()
        { marksMgr.calculateGPA(students); };
        actions[8] = [&]()
        { attendanceMgr.markAttendance(students); };
        actions[9] = [&]()
        { reportMgr->classReport(students); };
        actions[10] = [&]()
        { reportMgr->exportReport(students); };
        actions[11] = [&]()
        { studentMgr.sortStudents(students); };
    }

    void run()
    {
        AuthManager::login();
        int choice;
        do
        {
            cout << "\n------ Student Management System ------\n";
            cout << "1. Add Student\n2. View All Students\n3. Search Student\n4. Update Student\n5. Delete Student\n6. Enter Marks\n7. Calculate GPA\n8. Mark Attendance\n9. Class Report\n10. Export to CSV\n11. Sort Students\n12. Save & Exit\nChoice: ";
            cin >> choice;
            if (choice == 12)
            {
                FileHandler::saveToFile(students);
                cout << "Data saved. Exiting...\n";
                break;
            }
            else if (actions.count(choice))
            {
                actions[choice]();
            }
            else
            {
                cout << "Invalid choice.\n";
            }
        } while (true);
    }

    ~Menu()
    {
        delete reportMgr;
    }
};

int main()
{
    Menu menu;
    menu.run();
    return 0;
}
