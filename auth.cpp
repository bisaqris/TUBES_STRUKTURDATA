#include "kasir.h"
#include <iostream>
#include <cstdlib>
#include <limits>
#include <conio.h>

using namespace std;

string inputPassword()
{
    string password = "";
    char ch;
    while (true)
    {
        ch = _getch();

        if (ch == 13)
        {
            break;
        }
        else if (ch == 8)
        {
            if (!password.empty())
            {
                password.pop_back();
                cout << "\b \b"; 
            }
        }
        else if (ch >= 32 && ch <= 126)
        {
            password += ch;
            cout << '*';
        }
    }
    cout << endl;
    return password;
}

bool login()
{
    string username, password;
    int percobaan = 0;
    const int MAX_PERCOBAAN = 3;

    cout << "\n========================================" << endl;
    cout << "           SISTEM LOGIN KASIR" << endl;
    cout << "=======================================" << endl;

    while (percobaan < MAX_PERCOBAAN)
    {
        cout << "\nUsername: ";
        cin >> username;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        cout << "Password: ";
        password = inputPassword();

        if (cekUserLogin(username, password))
        {
            cout << "\nLogin berhasil! Selamat datang, " << username << "!" << endl;
            pause();
            return true;
        }

        percobaan++;
        cout << "\nUsername atau password salah!" << endl;

        if (percobaan < MAX_PERCOBAAN)
        {
            cout << "Sisa percobaan: " << (MAX_PERCOBAAN - percobaan) << endl;
        }
    }

    cout << "\nTerlalu banyak percobaan gagal. Program akan ditutup." << endl;
    return false;
}

void clearScreen()
{
    system("cls");
}

void pause()
{
    cout << "\nTekan ENTER untuk melanjutkan...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}