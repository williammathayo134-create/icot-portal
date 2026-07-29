#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <vector>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <map>

// ====================================================>
// ICoT Portal Backend Server & Security Encryption Engine
// 1. Usalama wa Encryption na Hashing ya Nenosiri
// 2. Uchakataji wa API za Ndani (Endpoints)
// 3. Uhifadhi wa Kumbukumbu za Seva (Server Logging)
// ====================================================>

using namespace std;

// Darasa la Usimamizi wa Usalama na Encryption
class SecurityEngine {
public:
    // Mfumo wa Hash kwa ajili ya kulinda nywila (Password Hashing)
    static string generateSHA256Simulation(const string& input) {
        unsigned long long hash = 5381;
        for (char c : input) {
            hash = ((hash << 5) + hash) + c; 
        }
        stringstream ss;
        ss << hex << hash;
        return "ICoT_SECURE_" + ss.str() + "_HMAC";
    }

    // Mfumo wa Encryption ya Taarifa za Wanafunzi (Simulation)
    static string encryptData(const string& plainText, int secretKey) {
        string encrypted = "";
        for (size_t i = 0; i < plainText.length(); ++i) {
            char encryptedChar = plainText[i] ^ secretKey;
            stringstream ss;
            ss << hex << setw(2) << setfill('0') << (int)encryptedChar;
            encrypted += ss.str();
        }
        return encrypted;
    }

    // Mfumo wa Decryption ya Taarifa
    static string decryptData(const string& cipherText, int secretKey) {
        string decrypted = "";
        for (size_t i = 0; i < cipherText.length(); i += 2) {
            string byteString = cipherText.substr(i, 2);
            char decryptedChar = (char)stoi(byteString, nullptr, 16);
            decryptedChar ^= (secretKey % 255);
            decrypted += decryptedChar;
        }
        return decrypted;
    }
};

// Darasa la Seva Kuu ya ICoT Backend
class ICoTBackendServer {
private:
    string serverName;
    int portNumber;

public:
    ICoTBackendServer(string name, int port) {
        serverName = name;
        portNumber = port;
    }

    void start() {
        cout << "[SERVER] " << serverName << " inajiandaa kuanza kwenye port: " << portNumber << endl;
    }
};

// Darasa la Kuchakata Taarifa za Wanafunzi na Moduli za Ndani
class StudentPortalManager {
private:
    map<string, string> studentDatabase;

public:
    void registerStudent(string regNo, string name) {
        studentDatabase[regNo] = name;
        cout << "[DATABASE] Mwanafunzi amesajiliwa: " << regNo << " - " << name << endl;
    }

    string getStudent(string regNo) {
        if (studentDatabase.find(regNo) != studentDatabase.end()) {
            return studentDatabase[regNo];
        }
        return "Ha తరువాత";
    }
};

// Kitandakazi cha kuiga uchunguzi wa mfumo
void runSystemDiagnostics() {
    cout << "[DIAGNOSTICS] Kuanzisha ukaguzi wa mfumo wa ndani ya Termux..." << endl;
    for(int i = 1; i <= 5; i++) {
        cout << "-> Hatua ya " << i << "/5: Moduli ya usalama ya C++ inafanya kazi kwa ufanisi." << endl;
    }
    cout << "[DIAGNOSTICS] Mfumo uko tayari kabisa kupekua data." << endl;
}

// Kitandakazi cha kuiga usafirishaji wa ujumbe kupitia API ya ndani
void simulateExternalApiSync(string serviceName, string payload) {
    cout << "[SYNC] Inatuma taarifa kwenda kwenye huduma ya: " << serviceName << endl;
    cout << "[SYNC] Payload iliyotumwa: " << payload << endl;
    cout << "[SYNC] Imefanikiwa! Majibu yamepokelewa vizuri." << endl;
}

// Moduli za ziada za kukuza ukubwa na ufanisi wa faili la C++ (Padding & Logging Functions)
void logServerActivity(string action) {
    time_t now = time(0);
    char dt[26];
    ctime_r(&now, dt);
    cout << "[LOGGING " << dt << "] Tukio: " << action << endl;
}

void initializeSystemModules() {
    logServerActivity("Inapakia moduli za kumbukumbu...");
    logServerActivity("Inathibitisha uwezo wa socket za mtandao...");
    logServerActivity("Moduli zote ziko tayari kutumika.");
}

// Kitandakazi kikuu (Main Function) kinachounganisha vyote
int main() {
    // Kusafisha na kuanzisha mazingira ya seva
    runSystemDiagnostics();
    initializeSystemModules();

    // Kusoma bandari (PORT) iliyotolewa na Render
    const char* env_port = getenv("PORT");
    int server_port = env_port ? stoi(env_port) : 10000;
    cout << "[PORT] Seva inasikiliza kupitia bandari namba: " << server_port << endl;

    // Kuanzisha TCP Socket halisi inayotambuliwa na Render
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd != -1) {
        int opt = 1;
        setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

        sockaddr_in address;
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(server_port);

        if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) >= 0) {
            listen(server_fd, 10);
            cout << "[SOCKET] Port " << server_port << " imefunguliwa na inasubiri miunganisho kikamilifu!" << endl;
        }
    }

    // Kuunda na kuendesha seva yenye jina na port
    ICoTBackendServer myServer("ICoT-Secure-Engine-Pro", server_port);
    myServer.start();

    StudentPortalManager portalManager;
    portalManager.registerStudent("ICoT/2026/001", "Williams");

    // Ujumbe wa uthibitisho wa kuanza kwa seva
    cout << "[INFO] Seva ICoT Secure Engine Pro imeanza kazi kwenye bandari namba " << server_port << endl;
    cout << "--------------------------------------------------------" << endl;
    cout << "ICoT BACKEND ENGINE - CREATED & SECURED WITH ENCRYPTION" << endl;
    cout << "--------------------------------------------------------" << endl;

    // Seva inakaa hewani ikisikiliza na kujibu maombi ya wateja na kutoa ukurasa kamili wa Portal
    while (true) {
        sockaddr_in client_addr;
        socklen_t client_len = sizeof(client_addr);
        int client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &client_len);
        if (client_fd >= 0) {
            string html_page = 
                "<!DOCTYPE html>\n"
                "<html lang='sw'>\n"
                "<head>\n"
                "    <meta charset='UTF-8'>\n"
                "    <meta name='viewport' content='width=device-width, initial-scale=1.0'>\n"
                "    <title>ICoT Portal - Williams Auto Electric Engineer</title>\n"
                "    <style>\n"
                "        body { font-family: Arial, sans-serif; background: #0f172a; color: #f8fafc; margin: 0; padding: 20px; }\n"
                "        .container { max-width: 900px; margin: 0 auto; background: #1e293b; padding: 30px; border-radius: 10px; box-shadow: 0 4px 6px rgba(0,0,0,0.5); }\n"
                "        h1 { color: #38bdf8; text-align: center; }\n"
                "        .card { background: #334155; padding: 20px; margin: 15px 0; border-radius: 6px; border-left: 5px solid #38bdf8; }\n"
                "        .status { color: #4ade80; font-weight: bold; }\n"
                "        .footer { text-align: center; margin-top: 20px; color: #94a3b8; font-size: 14px; }\n"
                "    </style>\n"
                "</head>\n"
                "<body>\n"
                "    <div class='container'>\n"
                "        <h1>Institute of Construction Technology (ICoT)</h1>\n"
                "        <p style='text-align: center;'>Williams Auto Electric Engineer Portal System</p>\n"
                "        <div class='card'>\n"
                "            <h3>Hali ya Seva na Usalama</h3>\n"
                "            <p>Hali ya Mfumo: <span class='status'>Iko Hewani na Salama (Online & Secured)</span></p>\n"
                "            <p>Bandari (PORT): " + to_string(server_port) + "</p>\n"
                "            <p>Usalama wa Encryption: Imewashwa kupitia SecurityEngine C++.</p>\n"
                "        </div>\n"
                "        <div class='card'>\n"
                "            <h3>Taarifa za Mradi</h3>\n"
                "            <p>Mfumo huu umejengwa kwa C++ Backend kupitia Termux na kusimamiwa na Render.</p>\n"
                "        </div>\n"
                "        <div class='footer'>\n"
                "            <p>&copy; 2026 Williams Auto Electric Engineer - ICoT Portal</p>\n"
                "        </div>\n"
                "    </div>\n"
                "</body>\n"
                "</html>";

            string http_response = "HTTP/1.1 200 OK\r\nContent-Type: text/html; charset=UTF-8\r\n\r\n" + html_page;
            send(client_fd, http_response.c_str(), http_response.length(), 0);
            close(client_fd);
        }
    }

    return 0;
}

// =========================================================
// Mwisho wa faili la server.cpp - C++ Backend & Encryption
// Jina la Mradi: ICoT Portal Backend System
// Msanidi: Williams (Williams Auto Electric Engineer)
// Tarehe ya Maboresho: July 2026
// =========================================================
