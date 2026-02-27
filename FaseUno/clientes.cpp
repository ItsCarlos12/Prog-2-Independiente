#include<iostream>
#include<cstring>
#include<iomanip>
#include "structs.h"

using namespace std;

static int anchosCli[] = {4, 12, 25, 12, 25};
static int colCli = 5;

bool cedulaDuplicada(Tienda* tienda, const char* cedula) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (strcmp(tienda->clientes[i].cedula, cedula) == 0) {
            return true;
        }
    }
    return false;
}

void crearCliente(Tienda* tienda){
    char buffer[200];
    char opcion;

    mostrarBanner("Registrar Nuevo Cliente");

    if(tienda -> numClientes == tienda -> capacidadClientes){
        redimensionarClientes(tienda);
    }

    Cliente& c = tienda -> clientes[tienda -> numClientes];

    cout << "Ingrese la cedula (o '0' para cancelar): ";
    cin.getline(buffer, 20);

    if(strcmp(buffer, "0") == 0){
        std::cout << "Registro cancelado" << std::endl;
        return;
    }

    if(cedulaDuplicada(tienda, buffer)){
        cout << "Error: la cedula " << buffer << " ya existe en el sistema" << endl;
        return;
    }

    strcpy(c.cedula, buffer);

    //Datos basicos

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
            cout << "Error: el email debe ser valido" << endl;
        }
    }while (!correoVer);

    cout << "Direccion fisica: ";
    cin.getline(c.direccion, 200);
    cout << "Telefono de contacto: ";
    cin.getline(c.telefono, 20);

    c.id = tienda -> siguienteIdCliente;

    cout << "\n Deseas guardar los datos del cliente? (S/N): ";
    cin >> opcion;
    cin.ignore();

    if(opcion == 's' || opcion == 'S'){
        tienda -> numClientes++;
        tienda -> siguienteIdCliente++;
        cout << "Cliente registrado exitosamente con ID: " << c.id << endl;
    }else{
        cout << "Registro descartado" << endl;
    }
}

void imprimirFilaCliente(const Cliente& c) {
    cout << setfill(' '); 
    cout << "| " << left << setw(anchosCli[0]) << c.id
         << "| " << setw(anchosCli[1]) << c.cedula
         << "| " << setw(anchosCli[2]) << c.nombre
         << "| " << setw(anchosCli[3]) << c.telefono
         << "| " << setw(anchosCli[4]) << c.email << " |" << endl;
}

void listarClientes(Tienda* tienda) {
    if(tienda->numClientes == 0) {
        cout << "No hay clientes registrados." << endl;
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

    for(int i = 0; i < tienda->numClientes; i++) {
        imprimirFilaCliente(tienda->clientes[i]);
    }
    dibujarTabla(anchosCli, colCli);

    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

int obtenerIndiceCli(Tienda* tienda, int idBuscar) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (tienda->clientes[i].id == idBuscar) {
            return i;
        }
    }
    return -1;
}

bool noClientes(Tienda* tienda) {
    if (tienda->numClientes == 0) {
        cout << "No hay clientes registrados." << endl;
        return true;
    }
    return false;
}

void buscarCliente(Tienda* tienda) {
    if (noClientes(tienda)) return;

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

    for (int i = 0; i < tienda->numClientes; i++) {
        Cliente& c = tienda->clientes[i];
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
                cout << "Opcion no valida." << endl;
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
    if (noClientes(tienda)) return;

    int idBuscar;
    mostrarBanner("Actualizar Cliente");
    cout << "Ingrese el ID del cliente a modificar: ";
    cin >> idBuscar;
    cin.ignore();

    int indice = obtenerIndiceCli(tienda, idBuscar);

    if (indice == -1) {
        cout << "Error: cliente no encontrado." << endl;
        return;
    }

    Cliente& c = tienda->clientes[indice];
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
                if (!cedulaDuplicada(tienda, buffer)) {
                    strcpy(c.cedula, buffer);
                } else {
                    cout << "Error: La cedula ya esta registrada." << endl;
                }
                break;
            case 3:
                cout << "Nuevo email: ";
                cin.getline(buffer, 100);
                if (strchr(buffer, '@')) {
                    strcpy(c.email, buffer);
                } else {
                    cout << "Error: formato de email invalido." << endl;
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
                cout << "Cambios guardados." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}

void eliminarCliente(Tienda* tienda) {
    if (noClientes(tienda)) return;

    int idBuscar;
    mostrarBanner("Eliminar Cliente");
    cout << "Ingrese el ID del cliente a eliminar: ";
    cin >> idBuscar;
    cin.ignore();

    int indice = obtenerIndiceCli(tienda, idBuscar);

    if (indice == -1) {
        cout << "Error: cliente no encontrado." << endl;
        return;
    }

    char confirmar;
    cout << "Deseas eliminar a '" << tienda->clientes[indice].nombre << "'? (S/N): ";
    cin >> confirmar;
    cin.ignore();

    if (confirmar == 's' || confirmar == 'S') {
        for (int i = indice; i < tienda->numClientes - 1; i++) {
            tienda->clientes[i] = tienda->clientes[i + 1];
        }
        tienda->numClientes--;
        cout << "Cliente eliminado exitosamente." << endl;
    } else {
        cout << "Operacion cancelada." << endl;
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
            default: cout << "Opcion no valida." << endl;
        }
    } while (opcion != 0);
}

void redimensionarClientes(Tienda* tienda){
int nuevaCapacidad = tienda -> capacidadClientes * 2;

Cliente* newArray = new Cliente[nuevaCapacidad];

//copiar todos los clientes.
for(int i=0; i<tienda -> numClientes; i++){
    newArray[i] = tienda -> clientes[i];
}

delete [] tienda -> clientes;
tienda -> clientes = nullptr;

tienda -> clientes = newArray;
tienda -> capacidadClientes = nuevaCapacidad;

cout<<"Redimensionado"<<endl;

}

bool existeCliente(Tienda* tienda, int idCliente) {
    for (int i = 0; i < tienda->numClientes; i++) {
        if (tienda->clientes[i].id == idCliente) {
            return true;
        }
    }
    return false;
}