#include "interfaz/Interfaz.h"
#include "persistencia/GestorArchivos.h"

#include <iostream>

int main() {
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        std::cerr << "Error al inicializar archivos" << std::endl;
        return 1;
    }

    Interfaz::ejecutar();
    return 0;
}
