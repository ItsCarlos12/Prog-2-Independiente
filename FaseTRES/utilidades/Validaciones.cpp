#include "Validaciones.h"

using namespace std;

#include <cctype>
#include <cstring>

bool Validaciones::validarEmail(const char* email) {
    if (email == nullptr || strlen(email) < 5) {
        return false;
    }

    const char* arroba = strchr(email, '@');
    if (arroba == nullptr) {
        return false;
    }

    const char* puntoLuegoArroba = strchr(arroba + 1, '.');
    return puntoLuegoArroba != nullptr;
}

bool Validaciones::validarRIF(const char* rif) {
    if (rif == nullptr || strlen(rif) < 4) {
        return false;
    }

    const char tipo = static_cast<char>(toupper(static_cast<unsigned char>(rif[0])));
    if (tipo != 'J' && tipo != 'V' && tipo != 'E' && tipo != 'G' && tipo != 'P') {
        return false;
    }
    if (rif[1] != '-') {
        return false;
    }

    for (size_t i = 2; i < strlen(rif); ++i) {
        if (!isdigit(static_cast<unsigned char>(rif[i]))) {
            return false;
        }
    }

    return true;
}

bool Validaciones::validarTelefono(const char* telefono) {
    if (telefono == nullptr || strlen(telefono) < 7) {
        return false;
    }

    int digitos = 0;
    for (size_t i = 0; i < strlen(telefono); ++i) {
        const unsigned char c = static_cast<unsigned char>(telefono[i]);
        if (isdigit(c)) {
            digitos++;
            continue;
        }
        if (telefono[i] != '-' && telefono[i] != ' ' && telefono[i] != '(' && telefono[i] != ')') {
            return false;
        }
    }
    return digitos >= 7;
}

bool Validaciones::validarRango(int valor, int minimo, int maximo) {
    return valor >= minimo && valor <= maximo;
}
