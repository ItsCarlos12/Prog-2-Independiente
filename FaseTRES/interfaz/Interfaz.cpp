#include "Interfaz.h"

using namespace std;

#include <iostream>

#include "../clientes/operacionesClientes.h"
#include "../mantenimiento/operacionesMantenimiento.h"
#include "../productos/operacionesProductos.h"
#include "../proveedores/operacionesProveedores.h"
#include "../transacciones/operacionesTransacciones.h"
#include "../utilidades/Formatos.h"

Tienda Interfaz::tienda("Mi TiendaTech", "J-112233445");

namespace {
void imprimirSeparadorPrincipal() {
    cout << "============================================================\n";
}

void imprimirSeparadorSecundario() {
    cout << "------------------------------------------------------------\n";
}

int leerOpcionEnRango(int minimo, int maximo) {
    int opcion = -1;
    while (true) {
        if (cin >> opcion && opcion >= minimo && opcion <= maximo) {
            Formatos::limpiarBuffer();
            return opcion;
        }

        Formatos::limpiarBuffer();
        cout << "Entrada invalida. Seleccione una opcion entre " << minimo << " y " << maximo
                  << ": ";
    }
}
}  // namespace

void Interfaz::mostrarMenuPrincipal() {
    cout << "\n";
    imprimirSeparadorPrincipal();
    cout << " SISTEMA DE GESTION DE INVENTARIO - PROYECTO 3\n";
    cout << " POO Y MODULARIZACION\n";
    imprimirSeparadorPrincipal();
    cout << " Tienda: " << tienda.getNombre() << " (" << tienda.getRif() << ")\n";
    imprimirSeparadorSecundario();
    cout << " [1] Gestion de Productos\n";
    cout << " [2] Gestion de Proveedores\n";
    cout << " [3] Gestion de Clientes\n";
    cout << " [4] Gestion de Transacciones\n";
    cout << " [5] Mantenimiento y Reportes\n";
    cout << " [0] Salir\n";
    imprimirSeparadorSecundario();
    cout << " Seleccione una opcion (0-5): ";
}

void Interfaz::menuProductos() {
    int opcion = -1;
    do {
        cout << "\n";
        imprimirSeparadorSecundario();
        cout << " MENU DE PRODUCTOS\n";
        imprimirSeparadorSecundario();
        cout << " [1] Registrar producto\n";
        cout << " [2] Buscar producto\n";
        cout << " [3] Actualizar producto\n";
        cout << " [4] Listar productos\n";
        cout << " [5] Eliminar producto\n";
        cout << " [6] Reporte de stock critico\n";
        cout << " [0] Volver al menu principal\n";
        imprimirSeparadorSecundario();
        cout << " Seleccione una opcion (0-6): ";
        opcion = leerOpcionEnRango(0, 6);

        switch (opcion) {
            case 1:
                registrarProducto(tienda);
                break;
            case 2:
                buscarProducto(tienda);
                break;
            case 3:
                actualizarProducto(tienda);
                break;
            case 4:
                listarProductos(tienda);
                break;
            case 5:
                eliminarProducto(tienda);
                break;
            case 6:
                productosStockCritico(tienda);
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }

        if (opcion != 0) {
            Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuProveedores() {
    int opcion = -1;
    do {
        cout << "\n";
        imprimirSeparadorSecundario();
        cout << " MENU DE PROVEEDORES\n";
        imprimirSeparadorSecundario();
        cout << " [1] Registrar proveedor\n";
        cout << " [2] Buscar proveedor\n";
        cout << " [3] Actualizar proveedor\n";
        cout << " [4] Listar proveedores\n";
        cout << " [5] Eliminar proveedor\n";
        cout << " [0] Volver al menu principal\n";
        imprimirSeparadorSecundario();
        cout << " Seleccione una opcion (0-5): ";
        opcion = leerOpcionEnRango(0, 5);

        switch (opcion) {
            case 1:
                registrarProveedor(tienda);
                break;
            case 2:
                buscarProveedor(tienda);
                break;
            case 3:
                actualizarProveedor(tienda);
                break;
            case 4:
                listarProveedores(tienda);
                break;
            case 5:
                eliminarProveedor(tienda);
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }

        if (opcion != 0) {
            Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuClientes() {
    int opcion = -1;
    do {
        cout << "\n";
        imprimirSeparadorSecundario();
        cout << " MENU DE CLIENTES\n";
        imprimirSeparadorSecundario();
        cout << " [1] Registrar cliente\n";
        cout << " [2] Buscar cliente\n";
        cout << " [3] Actualizar cliente\n";
        cout << " [4] Listar clientes\n";
        cout << " [5] Eliminar cliente\n";
        cout << " [0] Volver al menu principal\n";
        imprimirSeparadorSecundario();
        cout << " Seleccione una opcion (0-5): ";
        opcion = leerOpcionEnRango(0, 5);

        switch (opcion) {
            case 1:
                registrarCliente(tienda);
                break;
            case 2:
                buscarCliente(tienda);
                break;
            case 3:
                actualizarCliente(tienda);
                break;
            case 4:
                listarClientes(tienda);
                break;
            case 5:
                eliminarCliente(tienda);
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }

        if (opcion != 0) {
            Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::menuTransacciones() {
    int opcion = -1;
    do {
        cout << "\n";
        imprimirSeparadorSecundario();
        cout << " MENU DE TRANSACCIONES\n";
        imprimirSeparadorSecundario();
        cout << " [1] Registrar compra\n";
        cout << " [2] Registrar venta\n";
        cout << " [3] Listar transacciones\n";
        cout << " [4] Buscar transaccion\n";
        cout << " [5] Cancelar transaccion\n";
        cout << " [0] Volver al menu principal\n";
        imprimirSeparadorSecundario();
        cout << " Seleccione una opcion (0-5): ";
        opcion = leerOpcionEnRango(0, 5);

        switch (opcion) {
            case 1:
                registrarCompra(tienda);
                break;
            case 2:
                registrarVenta(tienda);
                break;
            case 3:
                listarTransacciones(tienda);
                break;
            case 4:
                buscarTransaccion(tienda);
                break;
            case 5:
                cancelarTransaccion(tienda);
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }

        if (opcion != 0) {
            Formatos::pausar();
        }
    } while (opcion != 0);
}

void Interfaz::ejecutar() {
    int opcion = -1;
    do {
        mostrarMenuPrincipal();
        opcion = leerOpcionEnRango(0, 5);

        switch (opcion) {
            case 1:
                menuProductos();
                break;
            case 2:
                menuProveedores();
                break;
            case 3:
                menuClientes();
                break;
            case 4:
                menuTransacciones();
                break;
            case 5:
                menuMantenimiento(tienda);
                break;
            case 0:
                cout << "Hasta luego.\n";
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }
    } while (opcion != 0);
}
