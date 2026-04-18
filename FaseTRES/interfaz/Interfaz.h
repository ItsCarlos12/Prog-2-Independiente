#ifndef INTERFAZ_HPP
#define INTERFAZ_HPP

#include "../tienda/Tienda.h"

class Interfaz {
public:
    static void ejecutar();

private:
    static Tienda tienda;

    static void mostrarMenuPrincipal();
    static void menuProductos();
    static void menuProveedores();
    static void menuClientes();
    static void menuTransacciones();
};

#endif
