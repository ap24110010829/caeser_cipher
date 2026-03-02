#include <iostream>
#include <string>
#include <fstream>
#include <limits>
#include <thread>
#include <chrono>

using namespace std;

class CaesarCipher {
public:
    string encrypt(const string &text, int key) {
        string result;
        key = ((key % 26) + 26) % 26;

        for (char c : text) {
            if (isalpha(static_cast<unsigned char>(c))) {
                char base = isupper(static_cast<unsigned char>(c)) ? 'A' : 'a';
                result += char((c - base + key) % 26 + base);
            } else {
                result += c;
            }
        }
        return result;
    }

    string decrypt(const string &text, int key) {
        key = ((key % 26) + 26) % 26;
        return encrypt(text, 26 - key);
    }
};

const string RESET  = "\033[0m";
const string RED    = "\033[31m";
const string GREEN  = "\033[32m";
const string YELLOW = "\033[33m";
const string CYAN   = "\033[36m";


void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void pauseConsole() {
    cout << "\nPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

int getKeyFromUser() {
    int key;
    while (true) {
        cout << "Enter key (1-25): ";
        if (!(cin >> key)) {
            cout << RED << "Invalid input. Enter a number.\n" << RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (key >= 1 && key <= 25) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return key;
        }

        cout << RED << "Key must be between 1 and 25.\n" << RESET;
    }
}

string loadFromFile(const string &filename) {
    ifstream fin(filename);
    if (!fin) return "__FILE_ERROR__";

    string content, line;
    while (getline(fin, line)) {
        content += line + '\n';
    }
    fin.close();
    return content;
}

bool saveToFile(const string &filename, const string &text) {
    ofstream fout(filename);
    if (!fout) return false;

    fout << text;
    fout.close();
    return true;
}

int main() {
    CaesarCipher cipher;

    int choice;
    string text;
    string lastResult;
    bool running = true;

    while (running) {
        clearScreen();

        cout << CYAN
             << "================= CAESAR CIPHER - SECRET TOOL =================\n"
             << RESET;

        cout << "1. Encrypt Text\n";
        cout << "2. Decrypt Text\n";
        cout << "3. Brute Force Attack\n";
        cout << "4. Load Text From File\n";
        cout << "5. Save Last Result To File\n";
        cout << "6. Exit\n";

        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << RED << "Invalid input.\n" << RESET;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            pauseConsole();
            continue;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {

        case 1: {
            cout << "Enter text to encrypt: ";
            getline(cin, text);

            if (text.empty()) {
                cout << YELLOW << "No text entered.\n" << RESET;
                pauseConsole();
                break;
            }

            int key = getKeyFromUser();
            lastResult = cipher.encrypt(text, key);

            cout << GREEN << "Encrypted Text: " << RESET << lastResult << "\n";
            pauseConsole();
            break;
        }

        case 2: {
            cout << "Enter text to decrypt: ";
            getline(cin, text);

            if (text.empty()) {
                cout << YELLOW << "No text entered.\n" << RESET;
                pauseConsole();
                break;
            }

            int key = getKeyFromUser();
            lastResult = cipher.decrypt(text, key);

            cout << GREEN << "Decrypted Text: " << RESET << lastResult << "\n";
            pauseConsole();
            break;
        }

        case 3: {
            cout << "Enter ciphertext: ";
            getline(cin, text);

            if (text.empty()) {
                cout << YELLOW << "No text entered.\n" << RESET;
                pauseConsole();
                break;
            }

            for (int key = 1; key < 26; ++key) {
                cout << "Key " << key << ": "
                     << cipher.decrypt(text, key) << "\n";
                #ifdef _WIN32
                    
                #else
                    usleep(60000);
                #endif
            }

            pauseConsole();
            break;
        }

        case 4: {
            cout << "Enter filename: ";
            string filename;
            getline(cin, filename);

            string loaded = loadFromFile(filename);

            if (loaded == "__FILE_ERROR__") {
                cout << RED << "File not found.\n" << RESET;
            }
            else if (loaded.empty()) {
                cout << YELLOW << "File is empty.\n" << RESET;
            }
            else {
                cout << GREEN << "File Loaded:\n" << RESET << loaded;
                lastResult = loaded;
            }

            pauseConsole();
            break;
        }

        case 5: {
            if (lastResult.empty()) {
                cout << YELLOW << "Nothing to save.\n" << RESET;
                pauseConsole();
                break;
            }

            cout << "Enter filename to save: ";
            string filename;
            getline(cin, filename);

            if (saveToFile(filename, lastResult))
                cout << GREEN << "Saved successfully.\n" << RESET;
            else
                cout << RED << "Save failed.\n" << RESET;

            pauseConsole();
            break;
        }

        case 6:
            running = false;
            break;

        default:
            cout << RED << "Invalid choice.\n" << RESET;
            pauseConsole();
        }
    }

    return 0;
}