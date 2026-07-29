#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>
#include <algorithm>
#include <cstdlib>

using namespace std;

// Muundo wa Taarifa za Mwanafunzi (Level 1 - 8, Password = Majina Matatu)
struct Student {
    string fullName;
    string phone;
    string email;
    string regDate;
    string level; // Level 1 mpaka Level 8
    string password; // Majina matatu
    bool hasResults = false;
    double totalMarks = 0.0;
    string grade = "-";
    int position = 0;
};

// Muundo wa Taarifa za Mwalimu
struct Teacher {
    string fullName;
    string email;
    string specialCode;
};

// Database ya mfumo kwa ajili ya kumbukumbu za muda
vector<Student> globalStudents;
vector<Teacher> globalTeachers;
bool teacherSubmittedResults = false; // Hali ya kuonyesha kama mwalimu kajaza au bado

// Kazi ya kuzalisha Special Code ya Mwalimu
string generateSpecialCode() {
    srand(time(0));
    int codeNum = 1000 + rand() % 9000;
    return "ICoT-TCH-" + to_string(codeNum);
}

// C++ Engine ya Kuhesabu Jumla, Daraja, na Position Otomatiki
void calculateStudentStandings() {
    for (size_t i = 0; i < globalStudents.size(); ++i) {
        for (size_t j = i + 1; j < globalStudents.size(); ++j) {
            if (globalStudents[i].totalMarks < globalStudents[j].totalMarks) {
                Student temp = globalStudents[i];
                globalStudents[i] = globalStudents[j];
                globalStudents[j] = temp;
            }
        }
    }

    for (size_t i = 0; i < globalStudents.size(); ++i) {
        globalStudents[i].position = i + 1;
        
        if (globalStudents[i].totalMarks >= 160) globalStudents[i].grade = "Grade A (Excellent)";
        else if (globalStudents[i].totalMarks >= 130) globalStudents[i].grade = "Grade B (Very Good)";
        else if (globalStudents[i].totalMarks >= 100) globalStudents[i].grade = "Grade C (Good)";
        else if (globalStudents[i].totalMarks >= 80) globalStudents[i].grade = "Grade D (Pass)";
        else globalStudents[i].grade = "Grade F (Fail)";
    }
}


int main() {
    const char* env_port = getenv("PORT");
    int server_port = env_port ? stoi(env_port) : 10000;

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1) return 1;

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(server_port);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) return 1;
    if (listen(server_fd, 50) < 0) return 1;

    cout << "[ICoT Portal] Backend server imeanza kufanya kazi kwenye port: " << server_port << endl;

    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd < 0) continue;

        char buffer[8192] = {0};
        read(client_fd, buffer, sizeof(buffer) - 1);
        string request(buffer);

        string path = "/index.html";
        size_t first_space = request.find(" ");
        if (first_space != string::npos) {
            size_t second_space = request.find(" ", first_space + 1);
            if (second_space != string::npos) {
                path = request.substr(first_space + 1, second_space - first_space - 1);
            }
        }

        string responseBody = "";
        string mimeType = "text/html; charset=UTF-8";

        // Upangaji wa Kurasa (Routing)
        if (path == "/" || path == "/index.html") {
            // Soma faili la index.html au toa ukurasa wa mwanzo
            ifstream file("index.html");
            if (file.is_open()) {
                stringstream buf; buf << file.rdbuf();
                responseBody = buf.str();
            } else {
                responseBody = "<!DOCTYPE html><html><head><title>ICoT Portal</title></head>"
                               "<body style='background:#0f172a;color:#fff;font-family:Arial;padding:40px;text-align:center;'>"
                               "<h1>Karibu ICoT Portal</h1>"
                               "<p>Williams Auto Electric Engineer System</p>"
                               "<p><a href='/student-register.html' style='color:#38bdf8;'>Usajili wa Mwanafunzi</a> | "
                               "<a href='/teacher-login.html' style='color:#38bdf8;'>Mlango wa Mwalimu</a></p></body></html>";
            }
        } 
        else if (path.find("/student-register") != string::npos) {
            // Mfano wa kurudisha fomu ya usajili au ujumbe wa mafanikio ya usajili wa mwanafunzi
            responseBody = "<!DOCTYPE html><html lang='sw'><head><meta charset='UTF-8'><title>Usajili wa Mwanafunzi</title></head>"
                           "<body style='background:#0f172a;color:#fff;font-family:Arial;padding:40px;'>"
                           "<h2>Usajili wa Mwanafunzi (Level 1 - 8)</h2>"
                           "<form method='GET' action='/save-student'>"
                           "Majina Matatu: <input type='text' name='name' required><br><br>"
                           "Namba ya Simu: <input type='text' name='phone' required><br><br>"
                           "Email: <input type='email' name='email' required><br><br>"
                           "Tarehe ya Kujiandikisha: <input type='date' name='date' required><br><br>"
                           "Level: <select name='level'><option>Level 1</option><option>Level 2</option><option>Level 3</option><option>Level 4</option><option>Level 5</option><option>Level 6</option><option>Level 7</option><option>Level 8</option></select><br><br>"
                           "<button type='submit'>Jisajili (Password itakuwa majina yako matatu)</button>"
                           "</form></body></html>";
        }
        else if (path.find("/save-student") != string::npos) {
            // Hapa C++ inasajili mwanafunzi na kuweka password kuwa majina yake matatu
            Student s;
            s.fullName = "Mwanafunzi Mpya ICoT"; 
            s.password = s.fullName; // Majina matatu yanakuwa password
            s.hasResults = teacherSubmittedResults;
            globalStudents.push_back(s);
            
            responseBody = "<h1>Usajili Umekamilika!</h1><p>Nenosiri (Password) yako ni majina yako matatu.</p><a href='/'>Rudi Nyumbani</a>";
        }
            else {
        // Angalia kama path inaomba ukurasa wa matokeo na mwalimu hajasubiri bado
        if (path.find("result") != string::npos || path.find("matokeo") != string::npos) {
            if (!teacherSubmittedResults) {
                responseBody = "<div style='background:#0f172a;color:#fff;text-align:center;padding:50px;font-family:Arial;'>"
                               "<h1 style='color:#ef4444;'>Not Available</h1>"
                               "<p style='color:#94a3b8;'>Mwalimu bado hajasubiri matokeo kwa muhula huu.</p>"
                               "<p><a href='/' style='color:#38bdf8;'>Rudi Nyumbani</a></p>"
                               "</div>";
            } else {
                responseBody = "<h1>Matokeo Yamewasilishwa na Mwalimu</h1>";
            }
        } else {
            responseBody = "<h1>404 - Kurasa Haionekani</h1>";
        }
    }


        string httpResponse = "HTTP/1.1 200 OK\r\n"
                              "Content-Type: " + mimeType + "\r\n"
                              "Content-Length: " + to_string(responseBody.length()) + "\r\n"
                              "Connection: close\r\n\r\n" + responseBody;

        send(client_fd, httpResponse.c_str(), httpResponse.length(), 0);
        close(client_fd);
    }

    close(server_fd);
    return 0;
}

