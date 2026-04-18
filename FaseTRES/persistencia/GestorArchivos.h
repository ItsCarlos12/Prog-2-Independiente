#ifndef GESTOR_ARCHIVOS_HPP
#define GESTOR_ARCHIVOS_HPP

#include <ctime>
#include <fstream>
#include <vector>

using namespace std;

#include "Constantes.h"

class GestorArchivos {
public:
    static bool inicializarSistemaArchivos();
    static bool reiniciarSistemaArchivos();
    static ArchivoHeader leerHeader(const char* rutaArchivo);
    static bool escribirHeader(const char* rutaArchivo, const ArchivoHeader& header);

    template <typename T>
    static bool guardarRegistro(const char* rutaArchivo, T& registro);

    template <typename T>
    static bool leerRegistroPorIndice(const char* rutaArchivo, int indice, T& registro);

    template <typename T>
    static bool actualizarRegistro(const char* rutaArchivo, int indice, T& registro);

    template <typename T>
    static int obtenerIndicePorId(const char* rutaArchivo, int id);

    template <typename T>
    static bool obtenerRegistroPorId(const char* rutaArchivo, int id, T& registro);

    template <typename T>
    static bool listarRegistrosActivos(const char* rutaArchivo, vector<T>& salida);

    template <typename T>
    static bool eliminarRegistroLogico(const char* rutaArchivo, int id);

    template <typename T>
    static int contarRegistrosActivos(const char* rutaArchivo);

private:
    static bool inicializarArchivo(const char* rutaArchivo);
};

template <typename T>
bool GestorArchivos::guardarRegistro(const char* rutaArchivo, T& registro) {
    ArchivoHeader header = leerHeader(rutaArchivo);

    fstream archivo(rutaArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }

    registro.setId(header.proximoID);
    registro.setEliminado(false);
    const time_t ahora = time(nullptr);
    registro.setFechaCreacion(ahora);
    registro.setFechaUltimaModificacion(ahora);

    const streamoff offset = sizeof(ArchivoHeader) +
                                  static_cast<streamoff>(header.cantidadRegistros) *
                                      static_cast<streamoff>(sizeof(T));
    archivo.seekp(offset);
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));

    if (!archivo.good()) {
        archivo.close();
        return false;
    }
    archivo.close();

    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    return escribirHeader(rutaArchivo, header);
}

template <typename T>
bool GestorArchivos::leerRegistroPorIndice(const char* rutaArchivo, int indice, T& registro) {
    if (indice < 0) {
        return false;
    }

    ifstream archivo(rutaArchivo, ios::binary);
    if (!archivo.is_open()) {
        return false;
    }

    ArchivoHeader header = leerHeader(rutaArchivo);
    if (indice >= header.cantidadRegistros) {
        archivo.close();
        return false;
    }

    const streamoff offset = sizeof(ArchivoHeader) +
                                  static_cast<streamoff>(indice) *
                                      static_cast<streamoff>(sizeof(T));
    archivo.seekg(offset);
    archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));
    const bool exito = archivo.good();
    archivo.close();
    return exito;
}

template <typename T>
bool GestorArchivos::actualizarRegistro(const char* rutaArchivo, int indice, T& registro) {
    if (indice < 0) {
        return false;
    }

    ArchivoHeader header = leerHeader(rutaArchivo);
    if (indice >= header.cantidadRegistros) {
        return false;
    }

    fstream archivo(rutaArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        return false;
    }

    registro.setFechaUltimaModificacion(time(nullptr));
    const streamoff offset = sizeof(ArchivoHeader) +
                                  static_cast<streamoff>(indice) *
                                      static_cast<streamoff>(sizeof(T));
    archivo.seekp(offset);
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
    const bool exito = archivo.good();
    archivo.close();
    return exito;
}

template <typename T>
int GestorArchivos::obtenerIndicePorId(const char* rutaArchivo, int id) {
    if (id <= 0) {
        return -1;
    }

    ArchivoHeader header = leerHeader(rutaArchivo);
    T registro;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        if (leerRegistroPorIndice(rutaArchivo, i, registro) && registro.getId() == id) {
            return i;
        }
    }
    return -1;
}

template <typename T>
bool GestorArchivos::obtenerRegistroPorId(const char* rutaArchivo, int id, T& registro) {
    const int indice = obtenerIndicePorId<T>(rutaArchivo, id);
    if (indice < 0) {
        return false;
    }
    return leerRegistroPorIndice<T>(rutaArchivo, indice, registro);
}

template <typename T>
bool GestorArchivos::listarRegistrosActivos(const char* rutaArchivo, vector<T>& salida) {
    salida.clear();
    ArchivoHeader header = leerHeader(rutaArchivo);
    T registro;

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        if (leerRegistroPorIndice(rutaArchivo, i, registro) && !registro.isEliminado()) {
            salida.push_back(registro);
        }
    }
    return true;
}

template <typename T>
bool GestorArchivos::eliminarRegistroLogico(const char* rutaArchivo, int id) {
    T registro;
    const int indice = obtenerIndicePorId<T>(rutaArchivo, id);
    if (indice < 0 || !leerRegistroPorIndice<T>(rutaArchivo, indice, registro) || registro.isEliminado()) {
        return false;
    }

    registro.setEliminado(true);
    if (!actualizarRegistro<T>(rutaArchivo, indice, registro)) {
        return false;
    }

    ArchivoHeader header = leerHeader(rutaArchivo);
    if (header.registrosActivos > 0) {
        header.registrosActivos--;
    }
    return escribirHeader(rutaArchivo, header);
}

template <typename T>
int GestorArchivos::contarRegistrosActivos(const char* rutaArchivo) {
    return leerHeader(rutaArchivo).registrosActivos;
}

#endif
