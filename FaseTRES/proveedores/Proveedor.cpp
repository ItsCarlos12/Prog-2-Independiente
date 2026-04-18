#include "Proveedor.h"

using namespace std;

#include <cstring>
#include <iostream>

Proveedor::Proveedor()
    : id(0), cantidadProductos(0), eliminado(false), fechaCreacion(0), fechaUltimaModificacion(0) {
    telefono[0] = '\0';
    nombre[0] = '\0';
    rif[0] = '\0';
    email[0] = '\0';
    direccion[0] = '\0';
    for (int i = 0; i < 100; ++i) {
        productosIDs[i] = 0;
    }
}

Proveedor::Proveedor(const char* nombreInicial, const char* rifInicial, const char* emailInicial)
    : Proveedor() {
    setNombre(nombreInicial);
    setRif(rifInicial);
    setEmail(emailInicial);
}

Proveedor::Proveedor(const Proveedor& other)
    : id(other.id), cantidadProductos(other.cantidadProductos), eliminado(other.eliminado),
      fechaCreacion(other.fechaCreacion), fechaUltimaModificacion(other.fechaUltimaModificacion) {
    strncpy(telefono, other.telefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    strncpy(nombre, other.nombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    strncpy(rif, other.rif, sizeof(rif) - 1);
    rif[sizeof(rif) - 1] = '\0';
    strncpy(email, other.email, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    strncpy(direccion, other.direccion, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    for (int i = 0; i < 100; ++i) {
        productosIDs[i] = other.productosIDs[i];
    }
}

Proveedor::~Proveedor() = default;

int Proveedor::getId() const { return id; }

const char* Proveedor::getTelefono() const { return telefono; }

const char* Proveedor::getNombre() const { return nombre; }

const char* Proveedor::getRif() const { return rif; }

const char* Proveedor::getEmail() const { return email; }

const char* Proveedor::getDireccion() const { return direccion; }

int Proveedor::getCantidadProductos() const { return cantidadProductos; }

bool Proveedor::isEliminado() const { return eliminado; }

time_t Proveedor::getFechaCreacion() const { return fechaCreacion; }

time_t Proveedor::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Proveedor::setId(int nuevoId) {
    if (nuevoId <= 0) {
        return false;
    }
    id = nuevoId;
    return true;
}

bool Proveedor::setTelefono(const char* nuevoTelefono) {
    if (nuevoTelefono == nullptr || nuevoTelefono[0] == '\0') {
        return false;
    }
    strncpy(telefono, nuevoTelefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
    return true;
}

bool Proveedor::setNombre(const char* nuevoNombre) {
    if (nuevoNombre == nullptr || nuevoNombre[0] == '\0') {
        return false;
    }
    strncpy(nombre, nuevoNombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    return true;
}

bool Proveedor::setRif(const char* nuevoRif) {
    if (nuevoRif == nullptr || nuevoRif[0] == '\0') {
        return false;
    }
    strncpy(rif, nuevoRif, sizeof(rif) - 1);
    rif[sizeof(rif) - 1] = '\0';
    return true;
}

bool Proveedor::setEmail(const char* nuevoEmail) {
    if (nuevoEmail == nullptr || nuevoEmail[0] == '\0') {
        return false;
    }
    strncpy(email, nuevoEmail, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
    return true;
}

bool Proveedor::setDireccion(const char* nuevaDireccion) {
    if (nuevaDireccion == nullptr) {
        return false;
    }
    strncpy(direccion, nuevaDireccion, sizeof(direccion) - 1);
    direccion[sizeof(direccion) - 1] = '\0';
    return true;
}

void Proveedor::setEliminado(bool estado) { eliminado = estado; }

void Proveedor::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }

void Proveedor::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

bool Proveedor::agregarProductoID(int idProducto) {
    if (idProducto <= 0 || cantidadProductos >= 100) {
        return false;
    }
    if (contieneProductoID(idProducto)) {
        return false;
    }
    productosIDs[cantidadProductos++] = idProducto;
    return true;
}

bool Proveedor::eliminarProductoID(int idProducto) {
    for (int i = 0; i < cantidadProductos; ++i) {
        if (productosIDs[i] == idProducto) {
            for (int j = i; j < cantidadProductos - 1; ++j) {
                productosIDs[j] = productosIDs[j + 1];
            }
            productosIDs[cantidadProductos - 1] = 0;
            cantidadProductos--;
            return true;
        }
    }
    return false;
}

bool Proveedor::contieneProductoID(int idProducto) const {
    for (int i = 0; i < cantidadProductos; ++i) {
        if (productosIDs[i] == idProducto) {
            return true;
        }
    }
    return false;
}

bool Proveedor::validarDatosBasicos() const {
    return nombre[0] != '\0' && rif[0] != '\0' && email[0] != '\0';
}

void Proveedor::mostrarInformacionBasica() const {
    cout << "ID: " << id << " | RIF: " << rif << " | Nombre: " << nombre << endl;
}

void Proveedor::mostrarInformacionCompleta() const {
    cout << "ID: " << id << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "RIF: " << rif << endl;
    cout << "Email: " << email << endl;
    cout << "Telefono: " << telefono << endl;
    cout << "Direccion: " << direccion << endl;
    cout << "Productos asociados: " << cantidadProductos << endl;
}

size_t Proveedor::obtenerTamano() { return sizeof(Proveedor); }
