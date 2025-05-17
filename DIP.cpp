#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>
#include <string>
#include <algorithm>
#include <map>
#include <functional>
#include <memory>
using namespace std;

// ==================== Base Classes (Following LSP) ====================

// Student Structure remains unchanged
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

// Interface for Grade Strategy (LSP: Base class)
class IGradeStrategy
{
public:
    virtual char calculateGrade(float percentage) const = 0;
    virtual ~IGradeStrategy() = default;
};

// Interface for Exporters (LSP: Base class)
class IExporter
{
public:
    virtual void exportData(const vector<Student> &students) const = 0;
    virtual ~IExporter() = default;
};

// Interface for Report Generators (LSP: Base class)
class IReportGenerator
{
public:
    virtual void generateReport(const vector<Student> &students) const = 0;
    virtual ~IReportGenerator() = default;
};

// Interface for GradeCalculator
class IGradeCalculator
{
public:
    virtual void calculateGrade(Student &s) const = 0;
    virtual ~IGradeCalculator() = default;
};

// ==================== Concrete Implementations ====================

// Default Grading Strategy (LSP: Substitutable for IGradeStrategy)
class DefaultGradeStrategy : public IGradeStrategy
{
public:
    char calculateGrade(float percentage) const override
    {
        if (percentage >= 90)
            return 'A';
        if (percentage >= 80)
            return 'B';
        if (percentage >= 70)
            return 'C';
        if (percentage >= 60)
            return 'D';
        return 'F';
    }
};

// Strict Grading Strategy (LSP: Another substitutable implementation)
class StrictGradeStrategy : public IGradeStrategy
{
public:
    char calculateGrade(float percentage) const override
    {
        if (percentage >= 95)
            return 'A';
        if (percentage >= 85)
            return 'B';
        if (percentage >= 75)
            return 'C';
        if (percentage >= 65)
            return 'D';
        return 'F';
    }
};

// CSV Exporter (LSP: Substitutable for IExporter)
class CSVExporter : public IExporter
{
public:
    void exportData(const vector<Student> &students) const override
    {
        ofstream file("students.csv");
        file << "Roll,Name,Class,Age,Gender,Percentage,Grade,Attendance\n";
        for (const auto &s : students)
        {
            file << s.rollNo << "," << s.name << "," << s.studentClass << ","
                 << s.age << "," << s.gender << "," << s.percentage << ","
                 << s.grade << "," << s.attendance << "\n";
        }
        cout << "Data exported to students.csv\n";
    }
};

// Text Report Generator (LSP: Substitutable for IReportGenerator)
class TextReportGenerator : public IReportGenerator
{
public:
    void generateReport(const vector<Student> &students) const override
    {
        string cls;
        cout << "Enter class to view report: ";
        cin >> cls;

        bool found = false;
        for (const auto &s : students)
        {
            if (s.studentClass == cls)
            {
                cout << s.rollNo << "\t" << s.name << "\t" << s.grade << "\t"
                     << s.percentage << "%\n";
                found = true;
            }
        }
        if (!found)
        {
            cout << "No students found in class " << cls << "\n";
        }
    }
};

// ==================== Core Management Classes ====================

class GradeCalculator : public IGradeCalculator
{
    shared_ptr<IGradeStrategy> strategy;

public:
    explicit GradeCalculator(shared_ptr<IGradeStrategy> strat) : strategy(move(strat)) {}

    void calculateGrade(Student &s) const override
    {
        float total = 0;
        for (float mark : s.marks)
            total += mark;
        s.percentage = total / 5;
        s.grade = strategy->calculateGrade(s.percentage);
    }
};

class FileHandler
{
public:
    static void saveToFile(const vector<Student> &students, const string &filename = "students.txt")
    {
        ofstream file(filename);
        for (const auto &s : students)
        {
            file << s.name << " " << s.rollNo << " " << s.studentClass << " "
                 << s.age << " " << s.gender;
            for (float mark : s.marks)
                file << " " << mark;
            file << " " << s.attendance << "\n";
        }
    }

    static vector<Student> loadFromFile(const string &filename = "students.txt")
    {
        vector<Student> students;
        ifstream file(filename);
        Student s;
        while (file >> s.name >> s.rollNo >> s.studentClass >> s.age >> s.gender >> s.marks[0] >> s.marks[1] >> s.marks[2] >> s.marks[3] >> s.marks[4] >> s.attendance)
        {
            students.push_back(s);
        }
        return students;
    }
};

class AuthManager
{
public:
    static bool authenticate()
    {
        string user, pass;
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> pass;
        return user == "admin" && pass == "1234";
    }
};

// ==================== Student Operations ====================

class StudentOperations
{
protected:
    vector<Student> students;
    shared_ptr<IGradeCalculator> gradeCalc; // Changed to interface

public:
    StudentOperations(shared_ptr<IGradeCalculator> strategy) // Modified constructor
        : gradeCalc(move(strategy))
    {
    }

    virtual void addStudent()
    {
        Student s;
        cout << "Enter name: ";
        cin.ignore();
        getline(cin, s.name);
        cout << "Enter roll number: ";
        cin >> s.rollNo;
        cout << "Enter class: ";
        cin.ignore();
        getline(cin, s.studentClass);
        cout << "Enter age: ";
        cin >> s.age;
        cout << "Enter gender: ";
        cin.ignore();
        getline(cin, s.gender);

        gradeCalc->calculateGrade(s); // Use interface
        students.push_back(s);
        cout << "Student added successfully.\n";
    }

    virtual void viewAllStudents() const
    {
        cout << left << setw(10) << "Roll" << setw(20) << "Name" << setw(10) << "Class"
             << setw(6) << "Age" << setw(10) << "Gender" << setw(10) << "Percentage"
             << setw(8) << "Grade" << "Attendance\n";

        for (const auto &s : students)
        {
            cout << setw(10) << s.rollNo << setw(20) << s.name << setw(10) << s.studentClass
                 << setw(6) << s.age << setw(10) << s.gender << setw(10) << fixed
                 << setprecision(2) << s.percentage << setw(8) << s.grade << s.attendance << "\n";
        }
    }

    virtual void searchStudent() const
    {
        int roll;
        cout << "Enter roll number: ";
        cin >> roll;

        auto it = find_if(students.begin(), students.end(),
                          [roll](const Student &s)
                          { return s.rollNo == roll; });

        if (it != students.end())
        {
            const auto &s = *it;
            cout << "\nStudent Details:\n"
                 << "Name: " << s.name << "\n"
                 << "Class: " << s.studentClass << "\n"
                 << "Age: " << s.age << "\n"
                 << "Gender: " << s.gender << "\n"
                 << "Percentage: " << s.percentage << "%\n"
                 << "Grade: " << s.grade << "\n";
        }
        else
        {
            cout << "Student not found.\n";
        }
    }

    virtual void updateStudent()
    {
        int roll;
        cout << "Enter roll number to update: ";
        cin >> roll;

        auto it = find_if(students.begin(), students.end(),
                          [roll](const Student &s)
                          { return s.rollNo == roll; });

        if (it != students.end())
        {
            Student &s = *it;
            cout << "Enter new name: ";
            cin.ignore();
            getline(cin, s.name);
            cout << "Enter new class: ";
            getline(cin, s.studentClass);
            cout << "Enter new age: ";
            cin >> s.age;
            cout << "Enter new gender: ";
            cin.ignore();
            getline(cin, s.gender);

            gradeCalc->calculateGrade(s); // Use interface
            cout << "Student updated successfully.\n";
        }
        else
        {
            cout << "Student not found.\n";
        }
    }

    virtual void deleteStudent()
    {
        int roll;
        cout << "Enter roll number to delete: ";
        cin >> roll;

        auto new_end = remove_if(students.begin(), students.end(),
                                 [roll](const Student &s)
                                 { return s.rollNo == roll; });

        if (new_end != students.end())
        {
            students.erase(new_end, students.end());
            cout << "Student deleted successfully.\n";
        }
        else
        {
            cout << "Student not found.\n";
        }
    }

    virtual void sortStudents()
    {
        sort(students.begin(), students.end(),
             [](const Student &a, const Student &b)
             { return a.rollNo < b.rollNo; });
        cout << "Students sorted by roll number.\n";
    }

    void saveData() const
    {
        FileHandler::saveToFile(students);
        cout << "Data saved successfully.\n";
    }

    void loadData()
    {
        students = FileHandler::loadFromFile();
        for (auto &s : students)
        {
            gradeCalc->calculateGrade(s); // Use interface
        }
    }
};

// ==================== Extended Functionality ====================

class ExtendedStudentOperations : public StudentOperations
{
    shared_ptr<IExporter> exporter;
    shared_ptr<IReportGenerator> reportGenerator;

public:
    ExtendedStudentOperations(shared_ptr<IGradeCalculator> gradeStrategy, // Use IGradeCalculator
                              shared_ptr<IExporter> exp,
                              shared_ptr<IReportGenerator> repGen)
        : StudentOperations(move(gradeStrategy)),
          exporter(move(exp)),
          reportGenerator(move(repGen))
    {
    }

    void markAttendance()
    {
        for (auto &s : students)
        {
            cout << "Mark attendance for " << s.name << " (P/A): ";
            char a;
            cin >> a;
            s.attendance = (toupper(a) == 'P') ? "Present" : "Absent";
        }
    }

    void enterMarks()
    {
        int roll;
        cout << "Enter roll number: ";
        cin >> roll;

        auto it = find_if(students.begin(), students.end(),
                          [roll](const Student &s)
                          { return s.rollNo == roll; });

        if (it != students.end())
        {
            Student &s = *it;
            cout << "Enter marks for 5 subjects (space separated): ";
            for (int i = 0; i < 5; ++i)
            {
                cin >> s.marks[i];
            }
            gradeCalc->calculateGrade(s); // Use interface
            cout << "Marks updated. New grade: " << s.grade << "\n";
        }
        else
        {
            cout << "Student not found.\n";
        }
    }

    void calculateGPA() const
    {
        for (const auto &s : students)
        {
            cout << s.name << ": GPA = " << fixed << setprecision(2)
                 << (s.percentage / 20.0) << "\n";
        }
    }

    void generateClassReport() const
    {
        reportGenerator->generateReport(students);
    }

    void exportData() const
    {
        exporter->exportData(students);
    }
};

// ==================== Menu System ====================

class MenuSystem
{
    unique_ptr<ExtendedStudentOperations> ops;
    map<int, function<void()>> menuActions;

    void initializeMenu()
    {
        menuActions[1] = [this]()
        { ops->addStudent(); };
        menuActions[2] = [this]()
        { ops->viewAllStudents(); };
        menuActions[3] = [this]()
        { ops->searchStudent(); };
        menuActions[4] = [this]()
        { ops->updateStudent(); };
        menuActions[5] = [this]()
        { ops->deleteStudent(); };
        menuActions[6] = [this]()
        { ops->enterMarks(); };
        menuActions[7] = [this]()
        { ops->calculateGPA(); };
        menuActions[8] = [this]()
        { ops->markAttendance(); };
        menuActions[9] = [this]()
        { ops->generateClassReport(); };
        menuActions[10] = [this]()
        { ops->exportData(); };
        menuActions[11] = [this]()
        { ops->sortStudents(); };
    }

public:
    MenuSystem()
    {
        auto gradeStrategy = make_shared<DefaultGradeStrategy>();
        auto exporter = make_shared<CSVExporter>();
        auto reportGen = make_shared<TextReportGenerator>();
        auto gradeCalc = make_shared<GradeCalculator>(gradeStrategy); // Create GradeCalculator

        ops = make_unique<ExtendedStudentOperations>(
            gradeCalc, exporter, reportGen); // Pass IGradeCalculator

        ops->loadData();
        initializeMenu();
    }

    void run()
    {
        if (!AuthManager::authenticate())
        {
            cout << "Authentication failed. Exiting...\n";
            return;
        }

        int choice;
        do
        {
            cout << "\n==== Student Management System ====\n";
            cout << "1. Add Student\n2. View All Students\n3. Search Student\n"
                 << "4. Update Student\n5. Delete Student\n6. Enter Marks\n"
                 << "7. Calculate GPA\n8. Mark Attendance\n9. Class Report\n"
                 << "10. Export Data\n11. Sort Students\n12. Save & Exit\n"
                 << "Enter choice: ";

            cin >> choice;

            if (choice == 12)
            {
                ops->saveData();
                cout << "Exiting system...\n";
                break;
            }

            if (menuActions.count(choice))
            {
                menuActions[choice]();
            }
            else
            {
                cout << "Invalid choice. Try again.\n";
            }

        } while (true);
    }
};

// ==================== Main Function ====================

int main()
{
    MenuSystem system;
    system.run();
    return 0;
}