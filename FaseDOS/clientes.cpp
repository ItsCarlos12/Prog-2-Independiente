#include<iostream>
#include<cstring>
#include<iomanip>
#include<string>
#include "structs.h"
#include "files.h"
#include "ui.h"

using namespace std;

static int anchosCli[] = {4, 12, 25, 12, 25};
static int colCli = 5;

bool cedulaDuplicada(const char* cedula) {
    ArchivoHeader header = leerHeader("clientes.bin");
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Cliente c = obtenerCliente(i + 1);
        if (c.id != 0 && !c.eliminado && strcmp(c.cedula, cedula) == 0) {
            return true;
        }
    }
    return false;
}

void imprimirFilaCliente(const Cliente& c) {
    cout << setfill(' '); 
    cout << "| " << left << setw(anchosCli[0]) << c.id
         << "| " << setw(anchosCli[1]) << c.cedula
         << "| " << setw(anchosCli[2]) << c.nombre
         << "| " << setw(anchosCli[3]) << c.telefono
         << "| " << setw(anchosCli[4]) << c.email << " |" << endl;
}

void crearCliente(Tienda* tienda){
    (void)tienda; // no se usa, se trabaja directo con archivos

    char buffer[200];
    char opcion;

    mostrarBanner("Registrar Nuevo Cliente");

    Cliente c{};

    cout << "Ingrese la cedula (o '0' para cancelar): ";
    cin.getline(c.cedula, 20);

    if(strcmp(c.cedula, "0") == 0){
        printWarning("Registro cancelado.");
        return;
    }

    if(cedulaDuplicada(c.cedula)){
        printError(string("Error: la cedula ") + c.cedula + " ya existe en el sistema.");
        return;
    }

    cout << "Nombre del cliente: ";
    cin.getline(c.nombre, 100);

    bool correoVer = false;

    do{
        cout << "Email: ";
        cin.getline(buffer, 100);
        if(strchr(buffer, '@')!=nullptr){
            strcpy(c.email, buffer);
            correoVer = true;
        }else{
            printError("Error: el email debe ser valido.");
        }
    }while (!correoVer);

    cout << "Direccion fisica: ";
    cin.getline(c.direccion, 200);
    cout << "Telefono de contacto: ";
    cin.getline(c.telefono, 20);

    cout << "\n Deseas guardar los datos del cliente? (S/N): ";
    cin >> opcion;
    cin.ignore();

    if(opcion == 's' || opcion == 'S'){
        guardarCliente(c);
        printSuccess(string("Cliente registrado exitosamente con ID: ") + to_string(c.id));
    }else{
        printWarning("Registro descartado.");
    }
}

bool hayClientes() {
    return contarClientes() > 0;
}

bool noClientes(Tienda* tienda) {
    (void)tienda;
    if (!hayClientes()) {
        printWarning("No hay clientes registrados.");
        return true;
    }
    return false;
}

void listarClientes(Tienda* tienda) {
    (void)tienda;

    if(!hayClientes()) {
        printWarning("No hay clientes registrados.");
        return;
    }

    mostrarBanner("LISTADO DE CLIENTES");
    dibujarTabla(anchosCli, colCli);
    cout << "| " << left << setw(anchosCli[0]) << "ID"
         << "| " << setw(anchosCli[1]) << "Cedula"
         << "| " << setw(anchosCli[2]) << "Nombre"
         << "| " << setw(anchosCli[3]) << "Tlf"
         << "| " << setw(anchosCli[4]) << "Email" << " |" << endl;
    dibujarTabla(anchosCli, colCli);

    ArchivoHeader header = leerHeader("clientes.bin");
    for(int i = 0; i < header.cantidadRegistros; i++) {
        Cliente c = obtenerCliente(i + 1);
        if (c.id != 0 && !c.eliminado) {
            imprimirFilaCliente(c);
        }
    }
    dibujarTabla(anchosCli, colCli);

    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

void buscarCliente(Tienda* tienda) {
    (void)tienda;

    if (!hayClientes()) {
        printWarning("No hay clientes registrados.");
        return;
    }

    int opcion;
    char criterio[100];
    mostrarBanner("Busqueda de Clientes");
    cout << "1. Por ID\n2. Por Cedula\n3. Por Nombre (Parcial)\n0. Cancelar\nSeleccione: ";
    cin >> opcion;
    cin.ignore();

    if (opcion == 0) {
        cout << "Busqueda cancelada." << endl;
        return;
    }

    cout << "Ingrese el valor a buscar: ";
    cin.getline(criterio, 100);

    bool encontrado = false;
    bool tablaMostrada = false;

    ArchivoHeader header = leerHeader("clientes.bin");
    for (int i = 0; i < header.cantidadRegistros; i++) {
        Cliente c = obtenerCliente(i + 1);
        if (c.id == 0 || c.eliminado) continue;

        bool coincide = false;

        switch (opcion) {
            case 1:
                if (c.id == atoi(criterio)) {
                    coincide = true;
                }
                break;
            case 2:
                if (strcmp(c.cedula, criterio) == 0) {
                    coincide = true;
                }
                break;
            case 3:
                if (compararLetras(c.nombre, criterio)) {
                    coincide = true;
                }
                break;
            default:
                printError("Opcion no valida.");
                return;
        }

        if (coincide) {
            if (!tablaMostrada) {
                dibujarTabla(anchosCli, colCli);
                cout << "| " << left << setw(anchosCli[0]) << "ID"
                     << "| " << setw(anchosCli[1]) << "Cedula"
                     << "| " << setw(anchosCli[2]) << "Nombre"
                     << "| " << setw(anchosCli[3]) << "Tlf"
                     << "| " << setw(anchosCli[4]) << "Email" << " |" << endl;
                dibujarTabla(anchosCli, colCli);
                tablaMostrada = true;
            }
            imprimirFilaCliente(c);
            encontrado = true;
        }
    }

    if (encontrado) {
        dibujarTabla(anchosCli, colCli);
    } else {
        cout << "\nNo se encontraron coincidencias." << endl;
    }
}

void actualizarCliente(Tienda* tienda) {
    (void)tienda;

    if (!hayClientes()) {
        printWarning("No hay clientes registrados.");
        return;
    }

    int idBuscar;
    mostrarBanner("Actualizar Cliente");
    cout << "Ingrese el ID del cliente a modificar: ";
    cin >> idBuscar;
    cin.ignore();

    Cliente c = obtenerCliente(idBuscar);
    if (c.id == 0 || c.eliminado) {
        printError("Error: cliente no encontrado.");
        return;
    }

    int opcion;
    char buffer[200];

    do {
        cout << "\nModificando a: " << c.nombre << " (ID: " << c.id << ")" << endl;
        cout << "1. Nombre\n2. Cedula\n3. Email\n4. Direccion\n5. Telefono\n0. Guardar y Salir\nSeleccione: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                cout << "Nuevo nombre: ";
                cin.getline(c.nombre, 100);
                break;
            case 2:
                cout << "Nueva cedula: ";
                cin.getline(buffer, 20);
                if (!cedulaDuplicada(buffer)) {
                    strcpy(c.cedula, buffer);
                } else {
                    printError("Error: la cedula ya esta registrada.");
                }
                break;
            case 3:
                cout << "Nuevo email: ";
                cin.getline(buffer, 100);
                if (strchr(buffer, '@')) {
                    strcpy(c.email, buffer);
                } else {
                    printError("Error: formato de email invalido.");
                }
                break;
            case 4:
                cout << "Nueva direccion: ";
                cin.getline(c.direccion, 200);
                break;
            case 5:
                cout << "Nuevo telefono: ";
                cin.getline(c.telefono, 20);
                break;
            case 0:
                printSuccess("Cambios guardados.");
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);

    actualizarCliente(c);
}

void eliminarCliente(Tienda* tienda) {
    (void)tienda;

    if (!hayClientes()) {
        printWarning("No hay clientes registrados.");
        return;
    }

    int idBuscar;
    mostrarBanner("Eliminar Cliente");
    cout << "Ingrese el ID del cliente a eliminar: ";
    cin >> idBuscar;
    cin.ignore();

    Cliente c = obtenerCliente(idBuscar);
    if (c.id == 0 || c.eliminado) {
        printError("Error: cliente no encontrado.");
        return;
    }

    char confirmar;
    cout << "Deseas eliminar a '" << c.nombre << "'? (S/N): ";
    cin >> confirmar;
    cin.ignore();

    if (confirmar == 's' || confirmar == 'S') {
        eliminarCliente(idBuscar);
        printSuccess("Cliente eliminado exitosamente.");
    } else {
        printWarning("Operacion cancelada.");
    }
}

void menuClientes(Tienda* tienda) {
    int opcion;
    do {
        mostrarBanner("Gestion de Clientes");
        cout << "1. Registrar Cliente" << endl;
        cout << "2. Listar Clientes" << endl;
        cout << "3. Buscar Cliente" << endl;
        cout << "4. Actualizar Cliente" << endl;
        cout << "5. Eliminar Cliente" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "\nSeleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1: crearCliente(tienda); break;
            case 2: listarClientes(tienda); break;
            case 3: buscarCliente(tienda); break;
            case 4: actualizarCliente(tienda); break;
            case 5: eliminarCliente(tienda); break;
            case 0: break;
            default: printError("Opcion no valida.");
        }
    } while (opcion != 0);
}

// Las funciones redimensionarClientes y existeCliente(Tienda*, ...) ya no son necesarias
// porque los clientes se gestionan directamente en archivos binarios mediante files.cpp.