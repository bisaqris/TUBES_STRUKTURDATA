#include "kasir.h"
#include <iostream>
#include <cstdlib>

using namespace std;

void clearScreen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void pause() {
    cout << "\nTekan Enter untuk melanjutkan...";
    cin.ignore();
    cin.get();
}

bool login() {
    clearScreen();
    
    cout << "===============================================" << endl;
    cout << "           SISTEM LOGIN KASIR                  " << endl;
    cout << "===============================================" << endl;
    
    string username, password;
    int percobaan = 0;
    const int maxPercobaan = 3;
    
    while (percobaan < maxPercobaan) {
        cout << "\nUsername: ";
        cin >> username;
        
        cout << "Password: ";
        cin >> password;
        
        if (cekUserLogin(username, password)) {
            cout << "\nLogin berhasil! Selamat datang, " << username << "!" << endl;
            pause();
            return true;
        } else {
            percobaan++;
            cout << "\nUsername atau password salah!" << endl;
            cout << "Sisa percobaan: " << (maxPercobaan - percobaan) << endl;
        }
    }
    
    cout << "\nTerlalu banyak percobaan gagal. Program ditutup." << endl;
    return false;
}