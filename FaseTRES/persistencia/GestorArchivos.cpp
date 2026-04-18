#include "GestorArchivos.h"

using namespace std;

#include <filesystem>

bool GestorArchivos::inicializarArchivo(const char* rutaArchivo) {
    fstream archivo(rutaArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        ofstream nuevo(rutaArchivo, ios::binary);
        if (!nuevo.is_open()) {
            return false;
        }
        nuevo.close();
        archivo.open(rutaArchivo, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            return false;
        }
    }

    archivo.seekg(0, ios::end);
    if (archivo.tellg() < static_cast<streamoff>(sizeof(ArchivoHeader))) {
        ArchivoHeader header{0, 1, 0, Constantes::VERSION_ARCHIVO};
        archivo.seekp(0);
        archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    }

    const bool exito = archivo.good();
    archivo.close();
    return exito;
}

bool GestorArchivos::inicializarSistemaArchivos() {
    filesystem::create_directories("datos");

    return inicializarArchivo(Constantes::RUTA_TIENDA) &&
           inicializarArchivo(Constantes::RUTA_PRODUCTOS) &&
           inicializarArchivo(Constantes::RUTA_PROVEEDORES) &&
           inicializarArchivo(Constantes::RUTA_CLIENTES) &&
           inicializarArchivo(Constantes::RUTA_TRANSACCIONES);
}

bool GestorArchivos::reiniciarSistemaArchivos() {
    filesystem::create_directories("datos");
    const char* rutas[] = {Constantes::RUTA_TIENDA, Constantes::RUTA_PRODUCTOS,
                           Constantes::RUTA_PROVEEDORES, Constantes::RUTA_CLIENTES,
                           Constantes::RUTA_TRANSACCIONES};

    for (const char* ruta : rutas) {
        ofstream truncar(ruta, ios::binary | ios::trunc);
        if (!truncar.is_open()) {
            return false;
        }
        ArchivoHeader header{0, 1, 0, Constantes::VERSION_ARCHIVO};
        truncar.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        truncar.close();
    }

    return true;
}

ArchivoHeader GestorArchivos::leerHeader(const char* rutaArchivo) {
    ifstream archivo(rutaArchivo, ios::binary);
    ArchivoHeader header{0, 1, 0, Constantes::VERSION_ARCHIVO};
    if (!archivo.is_open()) {
        return header;
    }

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return header;
}

bool GestorArchivos::escribirHeader(const char* rutaArchivo, const ArchivoHeader& header) {
    fstream archivo(rutaArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }

    archivo.seekp(0);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    const bool exito = archivo.good();
    archivo.close();
    return exito;
}
