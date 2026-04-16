#include <iostream>
#include <cstring>
#include "structs.h"
#include "files.h"
#include "ui.h"

using namespace std;

int main() {
    // Inicializar archivos binarios con header
    inicializarArchivos();

    Tienda miTienda;
    strcpy(miTienda.nombre, "Mi TiendaTech");
    strcpy(miTienda.rif, "J-112233445");

    int opcion;

    do {
        printTitle("Sistema de Gestion de Tienda");
        cout << miTienda.nombre << " (" << miTienda.rif << ")" << endl;
        
        cout << "1. Gestion de Productos" << endl;
        cout << "2. Gestion de Proveedores" << endl;
        cout << "3. Gestion de Clientes" << endl;
        cout << "4. Gestion de Transacciones" << endl;

        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch(opcion) {
            case 1: 
                menuProductos(&miTienda); 
                break;
            case 2: 
                menuProveedores(&miTienda); 
                break;
            case 3: 
                menuClientes(&miTienda); 
                break;
            case 4: 
                menuTransacciones(&miTienda); 
                break;
            case 0:
                printInfo("Saliendo del sistema. Hasta luego!");
                break;
            default:
                printError("Opcion no valida. Intente de nuevo.");
        }
    } while (opcion != 0);

    return 0;
}
