#include <iostream>
#include <ctime>
#include <cstring> 
#include <iomanip>
#include <string>
#include "structs.h"
#include "files.h"
#include "ui.h"


using namespace std;

int anchosProv[] = {4, 15, 20, 25, 12};
int colProv = 5;

bool rifDuplicado(const char* rif){
    ArchivoHeader header = leerHeader("proveedores.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Proveedor p = obtenerProveedor(i + 1);
        if (p.id != 0 && !p.eliminado && strcmp(p.rif, rif) == 0) {
            return true;
        }
    }
    return false;
}

bool emailValido(const char* email){
    return strchr(email, '@') != nullptr;
}

void imprimirFilaProveedor(const Proveedor& prov){

    cout << "| " << left << setw(anchosProv[0]) << prov.id
         << "| " << setw(anchosProv[1]) << prov.rif
         << "| " << setw(anchosProv[2]) << prov.nombre
         << "| " << setw(anchosProv[3]) << prov.email
         << "| " << setw(anchosProv[4]) << prov.telefono << " |" <<endl;
}

void mostrarProveedores(){
    dibujarTabla(anchosProv, colProv);
    cout << "| " << left << setw(anchosProv[0]) << "ID"
         << "| " << setw(anchosProv[1]) << "RIF"
         << "| " << setw(anchosProv[2]) << "Nombre"
         << "| " << setw(anchosProv[3]) << "Email"
         << "| " << setw(anchosProv[4]) << "Telefono" << " |" <<endl;
    
    dibujarTabla(anchosProv, colProv);
}

bool hayProveedores() {
    return contarProveedores() > 0;
}

bool noProveedores(Tienda* tienda){
    (void)tienda;
    if(!hayProveedores()){
        printWarning("No hay proveedores registrados.");
        return true;
    }
    return false;
}

void crearProveedor(Tienda* tienda){
    (void)tienda;

    char buffer[200];
    char opcion;

    mostrarBanner("Registrar Nuevo Proveedor");

    Proveedor prov{};

    cout << "Ingrese el RIF (o '0' para cancelar): ";
    cin.getline(buffer, 20);

    if(cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if(strcmp(buffer, "0") == 0){
        printWarning("Registro cancelado.");
        return;
    }

    if(rifDuplicado(buffer)){
        printError(string("Error: el RIF ") + buffer + " ya existe en el sistema.");
        return;
    }

    strcpy(prov.rif, buffer);

    cout << "Nombre del proveedor: ";
    cin.getline(prov.nombre, 100);

    bool correoVer = false;

    do{
        cout << "Email: ";
        cin.getline(buffer, 100);
        if(strchr(buffer, '@')!=nullptr){
            strcpy(prov.email, buffer);
            correoVer = true;
        }else{
            printError("Error: el email debe ser valido.");
        }
    }while (!correoVer);

    cout << "Direccion fisica: ";
    cin.getline(prov.direccion, 150);
    cout << "Telefono de contacto: ";
    cin.getline(prov.telefono, 20);

    cout << "\n Deseas guardar los datos del proveedor? (S/N): ";
    cin >> opcion;
    cin.ignore();

    if(opcion == 's' || opcion == 'S'){
        guardarProveedor(prov);
        printSuccess(string("Proveedor registrado exitosamente con ID: ") + to_string(prov.id));
    }else{
        printWarning("Registro descartado.");
    }
}

void listarProveedores(Tienda* tienda){
    (void)tienda;

    if(!hayProveedores()){
        printWarning("No hay proveedores registrados.");
        return;
    }

    mostrarBanner("Listado de Proveedores");
    mostrarProveedores();

    ArchivoHeader header = leerHeader("proveedores.bin");
    for(int i = 0; i < header.cantidadRegistros; i++){
        Proveedor p = obtenerProveedor(i + 1);
        if (p.id != 0 && !p.eliminado) {
            imprimirFilaProveedor(p);
        }
    }

    dibujarTabla(anchosProv, colProv);
    cout << "Total de proveedores activos: " << contarProveedores() << endl;

    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

void buscarProveedor(Tienda* tienda){
    (void)tienda;

    if(!hayProveedores()){
        printWarning("No hay proveedores registrados.");
        return;
    }

    int opcion;
    char criterio[100];

    mostrarBanner("Busqueda de Proveedores");
    cout << "1. Por ID | 2. Por Nombre | 3. Por RIF | 0. Cancelar | Seleccione: ";
    cin >> opcion;
    cin.ignore();

    if(opcion == 0) return;

    cout << "Ingrese valor a buscar: ";
    cin.getline(criterio, 100);

    bool encontrado = false;
    bool tablaIniciada = false;

    ArchivoHeader header = leerHeader("proveedores.bin");
    for(int i = 0; i < header.cantidadRegistros; i++){
        Proveedor prov = obtenerProveedor(i + 1);
        if (prov.id == 0 || prov.eliminado) continue;

        bool coincide = false;

        if(opcion == 1 && prov.id == atoi(criterio)) coincide = true;
        if(opcion == 2 && compararLetras(prov.nombre, criterio)) coincide = true;
        if(opcion == 3 && strcmp(prov.rif, criterio) == 0) coincide = true;

        if(coincide){
            if(!tablaIniciada){
                mostrarProveedores();
                tablaIniciada = true;
            }
            imprimirFilaProveedor(prov);
            encontrado = true;
        }
    }

    if(encontrado) dibujarTabla (anchosProv, colProv);
    else printWarning("No se encontraron coincidencias."); 
}

void actualizarProveedor(Tienda* tienda){
    (void)tienda;

    if(!hayProveedores()){
        printWarning("No hay proveedores registrados.");
        return;
    }

    int idBuscar;
    mostrarBanner("Actualizar Proveedor");
    cout << "Ingrese el ID del proveedor a modificar: ";
    cin >> idBuscar; 
    cin.ignore();

    Proveedor prov = obtenerProveedor(idBuscar);
    if (prov.id == 0 || prov.eliminado) {
        printError("Error: proveedor no encontrado.");
        return;
    }

    int opcion;
    char buffer[200];

    do{
        cout << "\nModificando: " << prov.nombre << " (ID: " << prov.id << ")" << endl;

        cout << "1. Nombre | 2. RIF | 3. Email | 4. Direccion | 5. Telefono | 0. Salir: ";
        cin >> opcion; cin.ignore();

        switch (opcion){
            case 1: cout << "Nuevo nombre: "; cin.getline(prov.nombre, 100); break;

            case 2: 
                cout << "Nuevo RIF: "; cin.getline(buffer, 20);
                if(!rifDuplicado(buffer)) strcpy(prov.rif, buffer);
                else printError("Error: RIF ya registrado."); break;
            
            case 3: 
                cout << "Nuevo email: "; cin.getline(buffer, 100);
                if(strchr(buffer, '@')) strcpy(prov.email, buffer);
                else printError("Error: formato invalido."); break;

            case 4: cout << "Nueva direccion: "; cin.getline(prov.direccion, 150); break;

            case 5: cout << "Nuevo telefono: "; cin.getline(prov.telefono, 20); break;
        }
    }while(opcion != 0);

    actualizarProveedor(prov);
}

void eliminarProveedor(Tienda* tienda){
    (void)tienda;

    if(!hayProveedores()){
        printWarning("No hay proveedores registrados.");
        return;
    }

    int idBuscar;
    mostrarBanner("Eliminar Proveedor");
    cout << "ID del proveedor a eliminar: ";
    cin >> idBuscar; cin.ignore();

    Proveedor prov = obtenerProveedor(idBuscar);
    if (prov.id == 0 || prov.eliminado) {
        printError("Error: proveedor no encontrado.");
        return;
    }

    char confirmar;
    cout << "Deseas eliminar a '" << prov.nombre << "'? (S/N): ";
    cin >> confirmar; cin.ignore();

    if(confirmar == 's' || confirmar == 'S'){
        eliminarProveedor(idBuscar);
        printSuccess("Proveedor eliminado exitosamente.");
    } else {
        printWarning("Operacion cancelada.");
    }
}

void menuProveedores(Tienda* tienda){
    int opcion;
    
    do{
        mostrarBanner("Gestion de Proveedores");
        cout << "1. Registrar proveedor" << endl;
        cout << "2. Listar Proveedores" << endl;
        cout << "3. Buscar Proveedor" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "\n Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch(opcion){
            case 1: crearProveedor(tienda); break;
            case 2: listarProveedores(tienda); break;
            case 3: buscarProveedor(tienda); break;
            case 0: break;
            default: printError("Opcion no valida.");
        }
    } while(opcion != 0);
}

// redimensionarProveedores y obtenerIndiceProv con Tienda* ya no son necesarios,
// la gestion de proveedores ahora es totalmente binaria via files.cpp.