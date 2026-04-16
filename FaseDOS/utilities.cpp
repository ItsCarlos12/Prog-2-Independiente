#include <iostream>
#include <fstream>
#include <cstring>
#include <cctype>
#include <iomanip>
#include <ctime>
#include "structs.h"
#include "utilities.h"

using namespace std;

// Implementación de strcasecmp para Windows/MSVC
#ifdef _MSC_VER
int strcasecmp(const char *s1, const char *s2) {
    while (*s1 && *s2) {
        if (tolower((unsigned char)*s1) != tolower((unsigned char)*s2)) {
            break;
        }
        s1++;
        s2++;
    }
    return tolower((unsigned char)*s1) - tolower((unsigned char)*s2);
}
#endif

void inicializarArchivos() {
    const char* archivos[] = {"productos.dat", "proveedores.dat", "clientes.dat", "transacciones.dat"};
    for (const char* archivo : archivos) {
        ifstream f(archivo, ios::binary);
        if (!f) {
            cout << "Creando archivo " << archivo << "..." << endl;
            ofstream newFile(archivo, ios::binary);
            newFile.close();
        } else {
            f.close();
        }
    }
}

int contarRegistros(const char* archivo, int sizeOfStruct) {
    ifstream f(archivo, ios::binary | ios::ate);
    if (!f) return 0;
    long long size = f.tellg();
    f.close();
    return size / sizeOfStruct;
}

int obtenerSiguienteId(const char* archivo) {
    if (strcmp(archivo, "productos.dat") == 0) {
        return contarRegistros(archivo, sizeof(Producto)) + 1;
    }
    if (strcmp(archivo, "proveedores.dat") == 0) {
        return contarRegistros(archivo, sizeof(Proveedor)) + 1;
    }
    if (strcmp(archivo, "clientes.dat") == 0) {
        return contarRegistros(archivo, sizeof(Cliente)) + 1;
    }
    if (strcmp(archivo, "transacciones.dat") == 0) {
        return contarRegistros(archivo, sizeof(Transaccion)) + 1;
    }
    return 0;
}

void guardarRegistro(const char* archivo, const void* registro, int pos) {
    fstream f(archivo, ios::in | ios::out | ios::binary);
    if (!f) {
        cerr << "Error al abrir el archivo " << archivo << " para escritura." << endl;
        return;
    }
    
    int sizeOfStruct = 0;
    if (strcmp(archivo, "productos.dat") == 0) sizeOfStruct = sizeof(Producto);
    else if (strcmp(archivo, "proveedores.dat") == 0) sizeOfStruct = sizeof(Proveedor);
    else if (strcmp(archivo, "clientes.dat") == 0) sizeOfStruct = sizeof(Cliente);
    else if (strcmp(archivo, "transacciones.dat") == 0) sizeOfStruct = sizeof(Transaccion);

    f.seekp(pos * sizeOfStruct, ios::beg);
    f.write(static_cast<const char*>(registro), sizeOfStruct);
    f.close();
}

bool leerRegistro(const char* archivo, void* registro, int pos) {
    ifstream f(archivo, ios::binary);
    if (!f) {
        cerr << "Error al abrir el archivo " << archivo << " para lectura." << endl;
        return false;
    }

    int sizeOfStruct = 0;
    if (strcmp(archivo, "productos.dat") == 0) sizeOfStruct = sizeof(Producto);
    else if (strcmp(archivo, "proveedores.dat") == 0) sizeOfStruct = sizeof(Proveedor);
    else if (strcmp(archivo, "clientes.dat") == 0) sizeOfStruct = sizeof(Cliente);
    else if (strcmp(archivo, "transacciones.dat") == 0) sizeOfStruct = sizeof(Transaccion);

    f.seekg(pos * sizeOfStruct, ios::beg);
    f.read(static_cast<char*>(registro), sizeOfStruct);
    
    bool success = f.good();
    f.close();
    return success;
}

void obtenerFechaActual(char* buffer) {
    time_t ahora = time(0);
    tm* ltm = localtime(&ahora);
    sprintf(buffer, "%02d/%02d/%d", ltm->tm_mday, 1 + ltm->tm_mon, 1900 + ltm->tm_year);
}

void dibujarTabla(int anchos[], int columnas) {
    cout << "+";
    for (int i = 0; i < columnas; i++) {
        for (int j = 0; j < anchos[i] + 2; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
}

void mostrarBanner(const char* titulo) {
    int ancho = 60;
    int textlength = strlen(titulo);
    int espacios = (ancho - textlength - 4) / 2;

    cout << "\n " << setfill('=') << setw(ancho) << "" << endl;
    cout << "||" << setfill(' ') << setw(espacios) << ""
         << titulo
         << setw(ancho - textlength - espacios - 4) << "" << "||" << endl;
    cout << " " << setfill('=') << setw(ancho) << "" << endl;
    cout << setfill(' ');
}

bool compararLetras(const char* original, const char* busqueda) {
    if (!original || !busqueda) return false;
    string o(original);
    string b(busqueda);
    for (char &c : o) c = tolower(c);
    for (char &c : b) c = tolower(c);
    return o.find(b) != string::npos;
}

// Funciones específicas que necesitan buscar en archivos
int obtenerIndiceProducto(int id) {
    int count = contarRegistros("productos.dat", sizeof(Producto));
    Producto p;
    for (int i = 0; i < count; i++) {
        if (leerRegistro("productos.dat", &p, i) && p.id == id) {
            return i;
        }
    }
    return -1;
}

bool existeProveedor(int id) {
    int count = contarRegistros("proveedores.dat", sizeof(Proveedor));
    Proveedor p;
    for (int i = 0; i < count; i++) {
        if (leerRegistro("proveedores.dat", &p, i) && p.id == id) {
            return true;
        }
    }
    return false;
}

bool existeCliente(int id) {
    int count = contarRegistros("clientes.dat", sizeof(Cliente));
    Cliente c;
    for (int i = 0; i < count; i++) {
        if (leerRegistro("clientes.dat", &c, i) && c.id == id) {
            return true;
        }
    }
    return false;
}