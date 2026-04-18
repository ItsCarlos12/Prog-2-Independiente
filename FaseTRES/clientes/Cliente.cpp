#include "Cliente.h"

using namespace std;

#include <cstring>
#include <iostream>

Cliente::Cliente()
    : id(0), cantidadTransacciones(0), eliminado(false), fechaCreacion(0),
      fechaUltimaModificacion(0) {
    nombre[0] = '\0';
    cedula[0] = '\0';
    telefono[0] = '\0';
    email[0] = '\0';
    direccion[0] = '\0';
    for (int i = 0; i < 100; ++i) {
        transaccionesIDs[i] = 0;
    }
}

Cliente::Cliente(const char* nombreInicial, const char* cedulaInicial, const char* emailInicial)
    : Cliente() {
    setNombre(nombreInicial);
    setCedula(cedulaInicial);
    setEmail(emailInicial);
}

Cliente::Cliente(const Cliente& other)
    : id(other.id), cantidadTransacciones(other.cantidadTransacciones), eliminado(other.eliminado),
      fechaCreacion(other.fechaCreacion), fechaUltimaModificacion(other.fechaUltimaModificacion) {
    strncpy(nombre, other.nombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    strncpy(cedula, other.cedula, sizeof(cedula) - 1);
    cedula[sizeof(cedula) - 1] = '\0';
    strncpy(telefono, other.telefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    strncpy(email, other.email, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    strncpy(direccion, other.direccion, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    for (int i = 0; i < 100; ++i) {
        transaccionesIDs[i] = other.transaccionesIDs[i];
    }
}

Cliente::~Cliente() = default;

int Cliente::getId() const { return id; }

const char* Cliente::getNombre() const { return nombre; }

const char* Cliente::getCedula() const { return cedula; }

const char* Cliente::getTelefono() const { return telefono; }

const char* Cliente::getEmail() const { return email; }

const char* Cliente::getDireccion() const { return direccion; }

int Cliente::getCantidadTransacciones() const { return cantidadTransacciones; }

bool Cliente::isEliminado() const { return eliminado; }

time_t Cliente::getFechaCreacion() const { return fechaCreacion; }

time_t Cliente::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Cliente::setId(int nuevoId) {
    if (nuevoId <= 0) {
        return false;
    }
    id = nuevoId;
    return true;
}

bool Cliente::setNombre(const char* nuevoNombre) {
    if (nuevoNombre == nullptr || nuevoNombre[0] == '\0') {
        return false;
    }
    strncpy(nombre, nuevoNombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    return true;
}

bool Cliente::setCedula(const char* nuevaCedula) {
    if (nuevaCedula == nullptr || nuevaCedula[0] == '\0') {
        return false;
    }
    strncpy(cedula, nuevaCedula, sizeof(cedula) - 1);
    cedula[sizeof(cedula) - 1] = '\0';
    return true;
}

bool Cliente::setTelefono(const char* nuevoTelefono) {
    if (nuevoTelefono == nullptr || nuevoTelefono[0] == '\0') {
        return false;
    }
    strncpy(telefono, nuevoTelefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    return true;
}

bool Cliente::setEmail(const char* nuevoEmail) {
    if (nuevoEmail == nullptr || nuevoEmail[0] == '\0') {
        return false;
    }
    strncpy(email, nuevoEmail, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    return true;
}

bool Cliente::setDireccion(const char* nuevaDireccion) {
    if (nuevaDireccion == nullptr) {
        return false;
    }
    strncpy(direccion, nuevaDireccion, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    return true;
}

void Cliente::setEliminado(bool estado) { eliminado = estado; }

void Cliente::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }

void Cliente::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

bool Cliente::agregarTransaccionID(int idTransaccion) {
    if (idTransaccion <= 0 || cantidadTransacciones >= 100) {
        return false;
    }
    for (int i = 0; i < cantidadTransacciones; ++i) {
        if (transaccionesIDs[i] == idTransaccion) {
            return false;
        }
    }
    transaccionesIDs[cantidadTransacciones++] = idTransaccion;
    return true;
}

bool Cliente::eliminarTransaccionID(int idTransaccion) {
    for (int i = 0; i < cantidadTransacciones; ++i) {
        if (transaccionesIDs[i] == idTransaccion) {
            for (int j = i; j < cantidadTransacciones - 1; ++j) {
                transaccionesIDs[j] = transaccionesIDs[j + 1];
            }
            transaccionesIDs[cantidadTransacciones - 1] = 0;
            cantidadTransacciones--;
            return true;
        }
    }
    return false;
}

bool Cliente::validarDatosBasicos() const {
    return nombre[0] != '\0' && cedula[0] != '\0' && email[0] != '\0';
}

void Cliente::mostrarInformacionBasica() const {
    cout << "ID: " << id << " | Cedula: " << cedula << " | Nombre: " << nombre
              << endl;
}

void Cliente::mostrarInformacionCompleta() const {
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Cedula: " << cedula << endl;
    cout << "Email: " << email << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Direccion: " << direccion << endl;
}

size_t Cliente::obtenerTamano() { return sizeof(Cliente); }
