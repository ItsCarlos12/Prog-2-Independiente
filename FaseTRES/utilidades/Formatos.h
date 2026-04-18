#ifndef FORMATOS_HPP
#define FORMATOS_HPP

#include <ctime>
#include <string>

class Formatos {
public:
    static std::string formatearFecha(std::time_t fecha);
    static std::string formatearMoneda(float monto);
    static void convertirAMayusculas(char* texto);
    static void limpiarBuffer();
    static void pausar();
};

#endif
