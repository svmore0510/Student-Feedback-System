#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <ctime>
#include <cstdlib>
using namespace std;


string trim(string s) {
    while (!s.empty() && (s.back() == '\n' || s.back() == '\r' || s.back() == ' '))
        s.pop_back();
    while (!s.empty() && s.front() == ' ')
        s.erase(s.begin());
    return s;
}

string toLower(string s) {
    for (char &c : s)
        c = tolower(c);
    return s;
}


bool userExists(string user) {
    ifstream file("output/users.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string u;
        getline(ss, u, ',');

        if (trim(u) == user)
            return true;
    }
    return false;
}

bool login(string user, string pass, string role) {
    ifstream file("output/users.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string u, p, r;

        getline(ss, u, ',');
        getline(ss, p, ',');
        getline(ss, r, ',');

        if (trim(u) == user && trim(p) == pass && trim(r) == role)
            return true;
    }
    return false;
}

string getTeacherSubject(string user) {
    ifstream file("output/users.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string u, p, r, subject;

        getline(ss, u, ',');
        getline(ss, p, ',');
        getline(ss, r, ',');
        getline(ss, subject);

        if (trim(u) == user && trim(r) == "teacher")
            return trim(subject);
    }
    return "";
}


void registerStudent() {
    string user, pass;

    cout << "\n--- Student Registration ---\n";
    cout << "Enter UserID: ";
    cin >> user;

    if (userExists(user)) {
        cout << "User already exists!\n";
        return;
    }

    cout << "Enter Password: ";
    cin >> pass;

    ofstream file("output/users.csv", ios::app);
    file << user << "," << pass << ",student\n";

    cout << "Student Registered!\n";
}

void registerTeacher() {
    string user, pass, subject;

    cout << "\n--- Teacher Registration ---\n";
    cout << "Enter UserID: ";
    cin >> user;

    if (userExists(user)) {
        cout << "User already exists!\n";
        return;
    }

    cout << "Enter Password: ";
    cin >> pass;

    cin.ignore();
    cout << "Enter Subject: ";
    getline(cin, subject);

    ofstream file("output/users.csv", ios::app);
    file << user << "," << pass << ",teacher," << subject << "\n";

    cout << "Teacher Registered!\n";
}


string generateID() {
    string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string id = "";

    for (int i = 0; i < 10; i++) {
        id += chars[rand() % chars.length()];
    }

    return id;
}


bool isDuplicate(int roll, string subject) {
    ifstream file("output/feedback.csv");
    string line;

    while (getline(file, line)) {
        stringstream ss(line);
        string r, id, sub;

        getline(ss, r, ',');
        getline(ss, id, ',');
        getline(ss, sub, ',');

        if (stoi(r) == roll && toLower(trim(sub)) == toLower(trim(subject)))
            return true;
    }
    return false;
}


void studentModule() {
    int roll, rating;
    string subject, comment;

    cout << "\n--- Feedback Form ---\n";
    cout << "Enter Roll Number: ";
    cin >> roll;

    cin.ignore();
    cout << "Enter Subject: ";
    getline(cin, subject);

    if (isDuplicate(roll, subject)) {
        cout << "Feedback already submitted for this subject!\n";
        return;
    }

    cout << "Enter Rating (1-5): ";
    cin >> rating;

    cin.ignore();
    cout << "Enter Comment: ";
    getline(cin, comment);

    string id = generateID();

    ofstream file("output/feedback.csv", ios::app);
    file << roll << "," << id << "," << subject << "," << rating << "," << comment << "\n";

    cout << "Feedback Submitted!\n";
}


void teacherView(string teacherSubject) {
    ifstream file("output/feedback.csv");
    string line;

    cout << "\n--- Teacher View ---\n";

    while (getline(file, line)) {
        stringstream ss(line);
        string roll, id, subject, rating, comment;

        getline(ss, roll, ',');
        getline(ss, id, ',');
        getline(ss, subject, ',');
        getline(ss, rating, ',');
        getline(ss, comment);

        if (toLower(trim(subject)) == toLower(trim(teacherSubject))) {
            cout << "ID: " << id
                 << " | " << subject
                 << " | Rating: " << rating
                 << " | Comment: " << comment << endl;
        }
    }
}


void principalView() {
    ifstream file("output/feedback.csv");
    string line;

    cout << "\n--- Principal View ---\n";

    while (getline(file, line)) {
        stringstream ss(line);
        string roll, id, subject, rating, comment;

        getline(ss, roll, ',');
        getline(ss, id, ',');
        getline(ss, subject, ',');
        getline(ss, rating, ',');
        getline(ss, comment);

        cout << "Roll: " << roll
             << " | ID: " << id
             << " | " << subject
             << " | Rating: " << rating
             << " | Comment: " << comment << endl;
    }
}


int main() {
    srand(time(0)); 

    int choice;
    string user, pass;

    do {
        cout << "\n===== DYP AnonVoice =====\n";
        cout << "1. Student Register\n";
        cout << "2. Teacher Register\n";
        cout << "3. Login (Student/Teacher)\n";
        cout << "4. Principal Login\n";
        cout << "5. Exit\n";
        cout << "Enter choice: ";
        cin >> choice;

        switch (choice) {

        case 1:
            registerStudent();
            break;

        case 2:
            registerTeacher();
            break;

        case 3:
            cout << "UserID: ";
            cin >> user;
            cout << "Password: ";
            cin >> pass;

            if (login(user, pass, "student")) {
                cout << "Student Login Success\n";
                studentModule();
            }
            else if (login(user, pass, "teacher")) {
                cout << "Teacher Login Success\n";
                string subject = getTeacherSubject(user);
                teacherView(subject);
            }
            else {
                cout << "Invalid Login\n";
            }
            break;

        case 4:
            cout << "Enter Principal UserID: ";
            cin >> user;
            cout << "Enter Password: ";
            cin >> pass;

            if (login(user, pass, "principal")) {
                cout << "Principal Login Success\n";
                principalView();
            } else {
                cout << "Invalid Principal Login\n";
            }
            break;

        case 5:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice\n";
        }

    } while (choice != 5);

    return 0;
}