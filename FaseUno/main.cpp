#include <iostream>
#include "structs.h"

using namespace std;

int main(){
    Tienda miTienda;

    inicializarTienda(&miTienda, "Mi TiendaTech", "J-112233445");

    int opcion;

    do{
        cout << "\n===== Sistema de Gestion de " << miTienda.nombre << " =====" << endl;
        
        cout << "1. Gestion de productos" << endl;
        cout << "2. Gestion de Proveedores" << endl;
        cout << "3. Lista de productos" << endl;

        cout << "0. Salir" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore(); // Limpiar el buffer de entrada.

        switch(opcion){
            case 1: 
                menuProductos(&miTienda); break;
    
            case 2: 
                menuProveedores(&miTienda); break;
            case 3: 
                

            case 0:
                cout << "Saliendo del sistema. Hasta luego!" << endl;
                break;

            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
        }
    } while (opcion != 0);

    liberarTienda(&miTienda);

    return 0;
}
