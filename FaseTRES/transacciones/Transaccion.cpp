#include "Transaccion.h"

using namespace std;

#include <cstring>
#include <iostream>

Transaccion::Transaccion()
    : id(0), idRelacionado(0), cantidadDetalles(0), total(0.0f), eliminado(false),
      fechaCreacion(0), fechaUltimaModificacion(0) {
    tipo[0] = '\0';
    descripcion[0] = '\0';
    for (int i = 0; i < Constantes::MAX_DETALLES_TRANSACCION; ++i) {
        detalles[i] = {0, 0, 0.0f};
    }
}

Transaccion::Transaccion(const char* tipoInicial, int idRelacionadoInicial) : Transaccion() {
    setTipo(tipoInicial);
    setIdRelacionado(idRelacionadoInicial);
}

Transaccion::Transaccion(const Transaccion& other)
    : id(other.id), idRelacionado(other.idRelacionado), cantidadDetalles(other.cantidadDetalles),
      total(other.total), eliminado(other.eliminado), fechaCreacion(other.fechaCreacion),
      fechaUltimaModificacion(other.fechaUltimaModificacion) {
    strncpy(tipo, other.tipo, sizeof(tipo) - 1);
    tipo[sizeof(tipo) - 1] = '\0';
    strncpy(descripcion, other.descripcion, sizeof(descripcion) - 1);
    descripcion[sizeof(descripcion) - 1] = '\0';
    for (int i = 0; i < Constantes::MAX_DETALLES_TRANSACCION; ++i) {
        detalles[i] = other.detalles[i];
    }
}

Transaccion::~Transaccion() = default;

int Transaccion::getId() const { return id; }

const char* Transaccion::getTipo() const { return tipo; }

int Transaccion::getIdRelacionado() const { return idRelacionado; }

int Transaccion::getCantidadDetalles() const { return cantidadDetalles; }

float Transaccion::getTotal() const { return total; }

const char* Transaccion::getDescripcion() const { return descripcion; }

bool Transaccion::isEliminado() const { return eliminado; }

time_t Transaccion::getFechaCreacion() const { return fechaCreacion; }

time_t Transaccion::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

const DetalleTransaccion* Transaccion::getDetalles() const { return detalles; }

bool Transaccion::getDetalle(int indice, DetalleTransaccion& detalle) const {
    if (indice < 0 || indice >= cantidadDetalles) {
        return false;
    }
    detalle = detalles[indice];
    return true;
}

bool Transaccion::setId(int nuevoId) {
    if (nuevoId <= 0) {
        return false;
    }
    id = nuevoId;
    return true;
}

bool Transaccion::setTipo(const char* nuevoTipo) {
    if (nuevoTipo == nullptr) {
        return false;
    }
    if (strcmp(nuevoTipo, "Venta") != 0 && strcmp(nuevoTipo, "Compra") != 0) {
        return false;
    }
    strncpy(tipo, nuevoTipo, sizeof(tipo) - 1);
    tipo[sizeof(tipo) - 1] = '\0';
    return true;
}

bool Transaccion::setIdRelacionado(int nuevoIdRelacionado) {
    if (nuevoIdRelacionado <= 0) {
        return false;
    }
    idRelacionado = nuevoIdRelacionado;
    return true;
}

bool Transaccion::setDescripcion(const char* nuevaDescripcion) {
    if (nuevaDescripcion == nullptr) {
        return false;
    }
    strncpy(descripcion, nuevaDescripcion, sizeof(descripcion) - 1);
    descripcion[sizeof(descripcion) - 1] = '\0';
    return true;
}

void Transaccion::setEliminado(bool estado) { eliminado = estado; }

void Transaccion::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }

void Transaccion::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

bool Transaccion::agregarDetalle(int idProducto, int cantidad, float precioUnitario) {
    if (idProducto <= 0 || cantidad <= 0 || precioUnitario < 0.0f ||
        cantidadDetalles >= Constantes::MAX_DETALLES_TRANSACCION) {
        return false;
    }
    detalles[cantidadDetalles++] = {idProducto, cantidad, precioUnitario};
    recalcularTotal();
    return true;
}

bool Transaccion::limpiarDetalles() {
    cantidadDetalles = 0;
    for (int i = 0; i < Constantes::MAX_DETALLES_TRANSACCION; ++i) {
        detalles[i] = {0, 0, 0.0f};
    }
    total = 0.0f;
    return true;
}

void Transaccion::recalcularTotal() {
    total = 0.0f;
    for (int i = 0; i < cantidadDetalles; ++i) {
        total += static_cast<float>(detalles[i].cantidad) * detalles[i].precioUnitario;
    }
}

bool Transaccion::validarDatosBasicos() const {
    return tipo[0] != '\0' && idRelacionado > 0 && cantidadDetalles > 0;
}

void Transaccion::mostrarInformacionBasica() const {
    cout << "ID: " << id << " | Tipo: " << tipo << " | Relacionado ID: " << idRelacionado
              << " | Total: " << total << endl;
}

void Transaccion::mostrarInformacionCompleta() const {
    cout << "ID: " << id << endl;
    cout << "Tipo: " << tipo << endl;
    cout << "ID Relacionado: " << idRelacionado << endl;
    cout << "Descripcion: " << descripcion << endl;
    cout << "Total: " << total << endl;
    cout << "Cantidad detalles: " << cantidadDetalles << endl;
    for (int i = 0; i < cantidadDetalles; ++i) {
        cout << "  - Producto ID: " << detalles[i].idProducto
                  << ", Cantidad: " << detalles[i].cantidad
                  << ", P.Unit: " << detalles[i].precioUnitario << endl;
    }
}

size_t Transaccion::obtenerTamano() { return sizeof(Transaccion); }
