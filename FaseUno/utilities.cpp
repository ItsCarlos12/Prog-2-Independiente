#include <iostream>
#include <cstring>
#include <cctype>
#include<iomanip>
#include "structs.h"

using namespace std;

void inicializarTienda(
    Tienda* tienda,
    const char* nombre,
    const char* rif
){
    strcpy(tienda -> nombre, nombre);
    strcpy(tienda -> rif, rif);

    //utilizando new para los arreglos con memoria dinamica.

    //productos
    tienda -> productos = new Producto[5];
    tienda -> capacidadProductos = 5;
    tienda -> numProductos = 0; //inicializamos en 0.

    //proveedores
    tienda -> proveedores = new Proveedor[5];
    tienda -> capacidadProveedores = 5;
    tienda -> numProveedores = 0;

    //clientes
    tienda -> clientes = new Cliente[5];
    tienda -> capacidadClientes = 5;
    tienda -> numClientes = 0;

    //transacciones
    tienda -> transacciones = new Transaccion[5];
    tienda -> capacidadTransacciones = 5;
    tienda -> numTransacciones = 0;

    //Siempre inicializar los IDs en 1.
    tienda -> siguienteIdProducto = 1;
    tienda -> siguienteIdProveedor = 1;
    tienda -> siguienteIdCliente = 1;
    tienda -> siguienteIdTransaccion = 1;
}

void liberarTienda(Tienda* tienda){
    delete[] tienda -> productos;
    delete[] tienda -> proveedores;
    delete[] tienda -> clientes;
    delete[] tienda -> transacciones;

    tienda -> productos = nullptr;
    tienda -> proveedores = nullptr;
    tienda -> clientes = nullptr;
    tienda -> transacciones = nullptr;

    cout<<"La memoria se libero correctamente."<<endl;
}


//Transformacion temporal a minusculas.
bool compararLetras(const char* original, const char* busqueda){
    if(!original || !busqueda) return false;

    char tempOriginal[100];
    char tempBusqueda[100];
    int i, j;

    for(i = 0; original[i] && i < 99; i++){
        tempOriginal[i] = tolower((unsigned char)original[i]);
    }
    tempOriginal[i] = '\0';

    for(j = 0; busqueda[j] && j < 99; j++){
        tempBusqueda[j] = tolower((unsigned char)busqueda[j]);
    }
    tempBusqueda[j] = '\0';

    return strstr(tempOriginal, tempBusqueda) != nullptr;
}

//Creacion de tabla.
void dibujarTabla(int anchos[], int columnas){
    cout << "+";
    for(int i = 0; i < columnas; i++){
        for(int j = 0; j < anchos[i] + 2; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
}

bool inventarioVacio(Tienda* tienda){
    if(tienda -> numProductos == 0){
        cout << "Operacion cancelada: el inventario esta vacio" << endl;
        return true;
    }

    return false;
}

//dibujar en cada menu y no tener que estarlo haciendo a mano.
void mostrarBanner(const char* titulo){
    int ancho = 60;
    int textlength = strlen(titulo);
    int espacios = (ancho - textlength - 4)/2;

    //para el borde superior
    cout << "\n " << setfill('=') << setw(ancho) << "" << endl;

    cout << "||" << setfill(' ') << setw(espacios) << ""
         << titulo
         << setw(ancho - textlength - espacios - 4) << "" << "||" << endl;
         
    cout << " " << setfill('=') << setw(ancho) << "" <<endl;

    cout << setfill(' ');
}