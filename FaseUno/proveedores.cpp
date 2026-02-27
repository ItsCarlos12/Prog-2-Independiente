#include <iostream>
#include <ctime>
#include <cstring> 
#include<iomanip>
#include "structs.h"


using namespace std;

int anchosProv[] = {4, 15, 20, 25, 12};
int colProv = 5;

bool rifDuplicado(Tienda* tienda, const char* rif){
    for(int i = 0; i < tienda -> numProveedores; i++){
        if(strcmp(tienda -> proveedores[i].rif, rif) == 0){
            return true;
        }
    }
    return false;
}

bool emailValido(const char* email){
    return strchr(email, '@') != nullptr;
}

void redimensionarProveedores(Tienda* tienda){
    int nuevaCapacidad = tienda -> capacidadProveedores * 2;

    //crear nuevo arreglo con la nueva capacidad.
    Proveedor* newArray = new Proveedor[nuevaCapacidad];

    //copiar todos los proveedores.
    for(int i=0; i<tienda -> numProveedores; i++){
        newArray[i] = tienda -> proveedores[i];
    }

    delete [] tienda -> proveedores;
    tienda -> proveedores = nullptr;

    tienda -> proveedores = newArray;
    tienda -> capacidadProveedores = nuevaCapacidad;

    cout<<"Redimensionado"<<endl;
}

void crearProveedor(Tienda* tienda){
    char buffer[200];
    char opcion;

    mostrarBanner("Registrar Nuevo Proveedor");

    if(tienda -> numProveedores == tienda -> capacidadProveedores){
        redimensionarProveedores(tienda);
    }

    Proveedor& prov = tienda -> proveedores[tienda -> numProveedores];

    cout << "Ingrese el RIF (o '0' para cancelar): ";
    cin.getline(buffer, 20);

    if(cin.fail()){
        cin.clear();
        cin.ignore(1000, '\n');
    }

    if(strcmp(buffer, "0") == 0){
        std::cout << "Registro cancelado" << endl;
        return;
    }

    if(rifDuplicado(tienda, buffer)){
        cout << "Error: el rif " << buffer << " ya existe en el sistema" << endl;
        return;
    }

    strcpy(prov.rif, buffer);

    //Datos basicos

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
            cout << "Error: el email debe ser valido" << endl;
        }
    }while (!correoVer);

    cout << "Direccion fisica: ";
    cin.getline(prov.direccion, 150);
    cout << "Telefono de contacto: ";
    cin.getline(prov.telefono, 20);

    prov.id = tienda -> siguienteIdProveedor;

    cout << "\n Deseas guardar los datos del proveedor? (S/N): ";
    cin >> opcion;
    cin.ignore();

    if(opcion == 's' || opcion == 'S'){
        tienda -> numProveedores++;
        tienda -> siguienteIdProveedor++;
        cout << "Proveedor registrado exitosamente con ID: " << prov.id << endl;
    }else{
        cout << "Registro descartado" << endl;
    }


}

void buscarProveedor(Tienda* tienda){
    if(noProveedores(tienda)) return;

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

    for(int i = 0; i < tienda -> numProveedores; i++){
        Proveedor& prov = tienda -> proveedores[i];
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
    else cout << "\n No se encontraron coincidencias" << endl; 
}

void listarProveedores(Tienda* tienda){
    if(noProveedores(tienda)) return;

    mostrarBanner("Listado de Proveedores");
    mostrarProveedores();

    for(int i = 0; i < tienda -> numProveedores; i++){
        imprimirFilaProveedor(tienda -> proveedores[i]);
    }

    dibujarTabla(anchosProv, colProv);
    cout << "Total de proveedores registrados: " << tienda -> numProveedores << endl;
}

void actualizarProveedor(Tienda* tienda){
    if(noProveedores(tienda)) return;

    int idBuscar;
    mostrarBanner("Actualizar Proveedor");
    cout << "Ingrese el ID del proveedor a modificar: ";
    cin >> idBuscar; 
    cin.ignore();

    int indice = obtenerIndiceProv(tienda, idBuscar);

    if(indice == -1){
        cout << "Error: proveedor no encontrado" << endl;
        return;
    }

    Proveedor& prov = tienda -> proveedores[indice];
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
                if(!rifDuplicado(tienda, buffer)) strcpy(prov.rif, buffer);
                else cout << "Error: RIF ya registrado" << endl; break;
            
            case 3: 
                cout << "Nuevo email: "; cin.getline(buffer, 100);
                if(strchr(buffer, '@')) strcpy(prov.email, buffer);
                else cout << "Error: formato invalido" << endl; break;

            case 4: cout << "Nueva direccion: "; cin.getline(prov.direccion, 150); break;

            case 5: cout << "Nuevo telefono: "; cin.getline(prov.telefono, 20); break;
        }
    }while(opcion != 0);
}

void eliminarProveedor(Tienda* tienda){
    if(noProveedores(tienda)) return;

    int idBuscar;
    mostrarBanner("Eliminar Proveedor");
    cout << "ID del proveedor a eliminar: ";
    cin >> idBuscar; cin.ignore();

    int indice = obtenerIndiceProv(tienda, idBuscar);

    if(indice == -1){
        cout << "Error: proveedor no encontrado" << endl;
        return;
    }

    char confirmar;
    cout << "Deseas eliminar a '" << tienda -> proveedores[indice].nombre << "'? (S/N): ";
    cin >> confirmar; cin.ignore();

    if(confirmar == 's' || confirmar == 'S'){
        for(int i = indice; i < tienda -> numProveedores - 1; i++){
            tienda -> proveedores[i] = tienda -> proveedores [i + 1];                 
        }
        tienda -> numProveedores--;
        cout << "Proveedor eliminado exitosamente" << endl;
    }
}

bool noProveedores(Tienda* tienda){

    if(tienda -> numProveedores == 0){
        cout << "No hay proveedores registrados para buscar" << endl;
        return true;
    }
    return false;
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
        }
    } while(opcion != 0);
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

int obtenerIndiceProv(Tienda * tienda, int idBuscar){
    for(int i = 0; i < tienda -> numProveedores; i++){
        if(tienda -> proveedores[i].id == idBuscar){
            return i; 
        }
    }
    return -1;
}