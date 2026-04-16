#include <iostream>
#include <ctime>
#include <cstring> 
#include <iomanip>
#include "structs.h"
#include "files.h"
#include "ui.h"

using namespace std;

static int col = 7;
static int anchosProd [] = {4, 12, 20, 20, 10, 8, 12};

void imprimirFilaProductoMejorada(const Producto& p) {
    Proveedor prov = obtenerProveedor(p.idProveedor);
    string nombreProveedor = (prov.id != 0 && !prov.eliminado) ? prov.nombre : "N/A";

    cout << "| " << left << setw(anchosProd[0]) << p.id
         << "| " << left << setw(anchosProd[1]) << p.codigo
         << "| " << left << setw(anchosProd[2]) << p.nombre
         << "| " << left << setw(anchosProd[3]) << nombreProveedor
         << "| " << left << setw(anchosProd[4]) << fixed << setprecision(2) <<  p.precio
         << "| " << left << setw(anchosProd[5]) << p.stock
         << "| " << left << setw(anchosProd[6]) << p.fechaCreacion << " |" <<endl;
}

void mostrarTablaProductosMejorada() {
    int anchoTotal = 0;
    for(int i = 0; i < col; i++){
        anchoTotal += anchosProd[i];
    }
    anchoTotal += col * 3 + 1;

    cout << "\n" << setfill('=') << setw(anchoTotal) << "" << endl;
    cout << setfill(' ');
    int padding = (anchoTotal - strlen("Listado de Productos")) / 2;
    cout << setw(padding) << "" << "Listado de Productos" << endl;
    cout << setw(anchoTotal) << setfill('=') << "" << endl;
    cout << setfill(' ');

    cout << "| " << left << setw(anchosProd[0]) << "ID"
         << "| " << setw(anchosProd[1]) << "Codigo"
         << "| " << setw(anchosProd[2]) << "Nombre"
         << "| " << setw(anchosProd[3]) << "Proveedor"
         << "| " << setw(anchosProd[4]) << "Precio"
         << "| " << setw(anchosProd[5]) << "Stock"
         << "| " << setw(anchosProd[6]) << "Fecha" << " |" <<endl;
         
    cout << setw(anchoTotal) << setfill('-') << "" << endl;
    cout << setfill(' ');
}


void crearProducto(Tienda* tienda){
    printTitle("Registrar Nuevo Producto");
    
    Producto p;
    char buffer[200];

    cout << "Ingrese el codigo del producto: ";
    cin.getline(buffer, 20);
    // Aquí iría la validación de código duplicado
    strcpy(p.codigo, buffer);

    cout << "Ingrese el nombre del producto: ";
    cin.getline(p.nombre, 100);

    cout << "Ingrese la descripcion del producto: ";
    cin.getline(p.descripcion, 200);

    cout << "Ingrese el ID del proveedor: ";
    cin >> p.idProveedor;
    
    Proveedor prov = obtenerProveedor(p.idProveedor);
    if (prov.id == 0 || prov.eliminado) {
        printError("Error: El proveedor no existe o fue eliminado.");
        return;
    }

    cout << "Ingrese el precio del producto: ";
    cin >> p.precio;

    cout <<"Ingrese el stock inicial: ";
    cin >> p.stock;
    
    cout << "Ingrese el stock minimo: ";
    cin >> p.stockMinimo;
    cin.ignore();

    p.totalVendido = 0;

    guardarProducto(p);
    printSuccess("Producto guardado exitosamente.");
}

void listarProductos(Tienda* tienda){
    printTitle("Inventario Completo");
    
    ArchivoHeader header = leerHeader("productos.bin");
    if (header.registrosActivos == 0) {
        printWarning("No hay productos registrados.");
        return;
    }

    mostrarTablaProductosMejorada();

    for(int i = 0; i < header.cantidadRegistros; i++){
        Producto p = obtenerProducto(i + 1);
        if (p.id != 0 && !p.eliminado) {
            imprimirFilaProductoMejorada(p);
        }
    }
    cout << setfill('-') << setw(85) << "" << endl;
}

void menuProductos(Tienda* tienda){
    int opcion;
    
    do{
        printTitle("Gestion de Productos");
        cout << "1. Registrar producto" << endl;
        cout << "2. Listar inventario completo" << endl;
        // Aquí se pueden añadir más opciones como buscar, actualizar, eliminar
        cout << "0. Volver al menu principal" << endl;
        cout << "\nSeleccione una opcion: ";

        cin >> opcion;
        cin.ignore();

        switch (opcion){
            case 1: crearProducto(tienda); break;
            case 2: listarProductos(tienda); break;
            case 0: break;
            default: printError("Opcion no valida");
        }
    }while(opcion != 0);
}