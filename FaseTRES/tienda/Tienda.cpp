#include "Tienda.h"

using namespace std;

#include <cstring>
#include <iostream>

Tienda::Tienda()
    : cantidadTransacciones(0), fechaCreacion(time(nullptr)),
      fechaUltimaModificacion(time(nullptr)) {
    strncpy(nombre, "Mi TiendaTech", sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';

    strncpy(rif, "J-112233445", sizeof(rif) - 1);
    rif[sizeof(rif) - 1] = '\0';

    for (int i = 0; i < 100; ++i) {
        transaccionesIDs[i] = 0;
    }
}

Tienda::Tienda(const char* nombreInicial, const char* rifInicial) : Tienda() {
    setNombre(nombreInicial);
    setRif(rifInicial);
}

Tienda::Tienda(const Tienda& other)
    : cantidadTransacciones(other.cantidadTransacciones), fechaCreacion(other.fechaCreacion),
      fechaUltimaModificacion(other.fechaUltimaModificacion) {
    strncpy(nombre, other.nombre, sizeof(nombre));
    strncpy(rif, other.rif, sizeof(rif));
    for (int i = 0; i < 100; ++i) {
        transaccionesIDs[i] = other.transaccionesIDs[i];
    }
}

Tienda::~Tienda() = default;

const char* Tienda::getNombre() const { return nombre; }

const char* Tienda::getRif() const { return rif; }

int Tienda::getCantidadTransacciones() const { return cantidadTransacciones; }

time_t Tienda::getFechaCreacion() const { return fechaCreacion; }

time_t Tienda::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Tienda::setNombre(const char* nuevoNombre) {
    if (nuevoNombre == nullptr || nuevoNombre[0] == '\0') {
        return false;
    }
    strncpy(nombre, nuevoNombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    fechaUltimaModificacion = time(nullptr);
    return true;
}

bool Tienda::setRif(const char* nuevoRif) {
    if (nuevoRif == nullptr || nuevoRif[0] == '\0') {
        return false;
    }
    strncpy(rif, nuevoRif, sizeof(rif) - 1);
    rif[sizeof(rif) - 1] = '\0';
    fechaUltimaModificacion = time(nullptr);
    return true;
}

void Tienda::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }

void Tienda::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

bool Tienda::agregarTransaccionID(int id) {
    if (id <= 0 || cantidadTransacciones >= 100) {
        return false;
    }
    for (int i = 0; i < cantidadTransacciones; ++i) {
        if (transaccionesIDs[i] == id) {
            return false;
        }
    }
    transaccionesIDs[cantidadTransacciones++] = id;
    fechaUltimaModificacion = time(nullptr);
    return true;
}

bool Tienda::eliminarTransaccionID(int id) {
    if (id <= 0) {
        return false;
    }
    for (int i = 0; i < cantidadTransacciones; ++i) {
        if (transaccionesIDs[i] == id) {
            for (int j = i; j < cantidadTransacciones - 1; ++j) {
                transaccionesIDs[j] = transaccionesIDs[j + 1];
            }
            transaccionesIDs[cantidadTransacciones - 1] = 0;
            cantidadTransacciones--;
            fechaUltimaModificacion = time(nullptr);
            return true;
        }
    }
    return false;
}

void Tienda::mostrarInformacionBasica() const {
    cout << "Tienda: " << nombre << " (" << rif << ")" << endl;
}

void Tienda::mostrarInformacionCompleta() const {
    cout << "Nombre: " << nombre << endl;
    cout << "RIF: " << rif << endl;
    cout << "Transacciones asociadas: " << cantidadTransacciones << endl;
}

size_t Tienda::obtenerTamano() { return sizeof(Tienda); }
