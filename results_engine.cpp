#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

// =========================================================================
// ICoT Student Results Engine & Grade Calculator (C++)
// Moduli maalum ya kuchakata na kutuma taarifa za matokeo ya mitihani
// =========================================================================

using namespace std;

class StudentResultProcessor {
private:
    string studentName;
    string regNumber;
    vector<double> marks;

public:
    StudentResultProcessor(string name, string reg, vector<double> studentMarks) {
        studentName = name;
        regNumber = reg;
        marks = studentMarks;
    }

    string calculateGrade(double average) {
        if (average >= 80) return "A (Vizuri Sana)";
        else if (average >= 70) return "B+ (Vizuri)";
        else if (average >= 60) return "B (Wastani Mzuri)";
        else if (average >= 50) return "C (Wastani)";
        else return "F (Feli - Rudia Somo)";
    }

    void generateTranscript() {
        cout << "====================================================" << endl;
        cout << "       RIPOTI RASMI YA MATOKEO - ICoT PORTAL        " << endl;
        cout << "====================================================" << endl;
        cout << "Jina la Mwanafunzi: " << studentName << endl;
        cout << "Namba ya Usajili:   " << regNumber << endl;
        cout << "----------------------------------------------------" << endl;

        double total = 0;
        for (size_t i = 0; i < marks.size(); ++i) {
            cout << "Somo la " << (i + 1) << " Alama: " << marks[i] << "%" << endl;
            total += marks[i];
        }

        double average = marks.empty() ? 0 : total / marks.size();
        cout << "----------------------------------------------------" << endl;
        cout << "Wastani wa Jumla (Average): " << fixed << setprecision(2) << average << "%" << endl;
        cout << "Daraja la Jumla (Overall Grade): " << calculateGrade(average) << endl;
        cout << "====================================================" << endl;
    }
};

