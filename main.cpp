#include "kasir.h"
#include <iostream>
#include <iomanip>

using namespace std;

int main() {
    if (!connectDatabase("kasir.db")) {
        cerr << "Gagal terhubung ke database. Program dihentikan." << endl;
        return 1;
    }
     if (!login()) {
        disconnectDatabase();
        return 1;
    }
    disconnectDatabase();
    return 0;
}