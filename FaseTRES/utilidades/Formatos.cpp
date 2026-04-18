#include "Formatos.h"

using namespace std;

#include <cctype>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>

string Formatos::formatearFecha(time_t fecha) {
    if (fecha == 0) {
        return "N/A";
    }

    tm* tmInfo = localtime(&fecha);
    if (tmInfo == nullptr) {
        return "N/A";
    }

    ostringstream salida;
    salida << setfill('0') << setw(2) << tmInfo->tm_mday << '/'
           << setw(2) << (tmInfo->tm_mon + 1) << '/'
           << (tmInfo->tm_year + 1900);
    return salida.str();
}

string Formatos::formatearMoneda(float monto) {
    ostringstream salida;
    salida << fixed << setprecision(2) << monto;
    return salida.str();
}

void Formatos::convertirAMayusculas(char* texto) {
    if (texto == nullptr) {
        return;
    }

    for (int i = 0; texto[i] != '\0'; ++i) {
        texto[i] = static_cast<char>(toupper(static_cast<unsigned char>(texto[i])));
    }
}

void Formatos::limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void Formatos::pausar() {
    cout << "\nPresione ENTER para continuar...";
    cin.get();
}
