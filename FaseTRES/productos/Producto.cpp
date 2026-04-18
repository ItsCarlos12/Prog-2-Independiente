#include "Producto.h"

using namespace std;

#include <cstring>
#include <iostream>

Producto::Producto()
    : id(0), precio(0.0f), stock(0), idProveedor(0), stockMinimo(0), totalVendido(0),
      cantidadTransacciones(0), eliminado(false), fechaCreacion(0), fechaUltimaModificacion(0) {
    codigo[0] = '\0';
    nombre[0] = '\0';
    descripcion[0] = '\0';
    for (int i = 0; i < 100; ++i) {
        transaccionesIDs[i] = 0;
    }
}

Producto::Producto(const char* codigoInicial, const char* nombreInicial, float precioInicial,
                   int stockInicial, int idProveedorInicial)
    : Producto() {
    setCodigo(codigoInicial);
    setNombre(nombreInicial);
    setPrecio(precioInicial);
    setStock(stockInicial);
    setIdProveedor(idProveedorInicial);
}

Producto::Producto(const Producto& other)
    : id(other.id), precio(other.precio), stock(other.stock), idProveedor(other.idProveedor),
      stockMinimo(other.stockMinimo), totalVendido(other.totalVendido),
      cantidadTransacciones(other.cantidadTransacciones), eliminado(other.eliminado),
      fechaCreacion(other.fechaCreacion), fechaUltimaModificacion(other.fechaUltimaModificacion) {
    strncpy(codigo, other.codigo, sizeof(codigo) - 1);
    codigo[sizeof(codigo) - 1] = '\0';
    strncpy(nombre, other.nombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    strncpy(descripcion, other.descripcion, sizeof(descripcion) - 1);
    descripcion[sizeof(descripcion) - 1] = '\0';

    for (int i = 0; i < 100; ++i) {
        transaccionesIDs[i] = other.transaccionesIDs[i];
    }
}

Producto::~Producto() = default;

int Producto::getId() const { return id; }

const char* Producto::getCodigo() const { return codigo; }

const char* Producto::getNombre() const { return nombre; }

const char* Producto::getDescripcion() const { return descripcion; }

float Producto::getPrecio() const { return precio; }

int Producto::getStock() const { return stock; }

int Producto::getIdProveedor() const { return idProveedor; }

int Producto::getStockMinimo() const { return stockMinimo; }

int Producto::getTotalVendido() const { return totalVendido; }

bool Producto::isEliminado() const { return eliminado; }

int Producto::getCantidadTransacciones() const { return cantidadTransacciones; }

time_t Producto::getFechaCreacion() const { return fechaCreacion; }

time_t Producto::getFechaUltimaModificacion() const { return fechaUltimaModificacion; }

bool Producto::setId(int nuevoId) {
    if (nuevoId <= 0) {
        return false;
    }
    id = nuevoId;
    return true;
}

bool Producto::setCodigo(const char* nuevoCodigo) {
    if (nuevoCodigo == nullptr || nuevoCodigo[0] == '\0') {
        return false;
    }
    strncpy(codigo, nuevoCodigo, sizeof(codigo) - 1);
    codigo[sizeof(codigo) - 1] = '\0';
    return true;
}

bool Producto::setNombre(const char* nuevoNombre) {
    if (nuevoNombre == nullptr || nuevoNombre[0] == '\0') {
        return false;
    }
    strncpy(nombre, nuevoNombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
    return true;
}

bool Producto::setDescripcion(const char* nuevaDescripcion) {
    if (nuevaDescripcion == nullptr) {
        return false;
    }
    strncpy(descripcion, nuevaDescripcion, sizeof(descripcion) - 1);
    descripcion[sizeof(descripcion) - 1] = '\0';
    return true;
}

bool Producto::setPrecio(float nuevoPrecio) {
    if (nuevoPrecio < 0.0f) {
        return false;
    }
    precio = nuevoPrecio;
    return true;
}

bool Producto::setStock(int nuevoStock) {
    if (nuevoStock < 0) {
        return false;
    }
    stock = nuevoStock;
    return true;
}

bool Producto::setIdProveedor(int nuevoIdProveedor) {
    if (nuevoIdProveedor <= 0) {
        return false;
    }
    idProveedor = nuevoIdProveedor;
    return true;
}

bool Producto::setStockMinimo(int nuevoStockMinimo) {
    if (nuevoStockMinimo < 0) {
        return false;
    }
    stockMinimo = nuevoStockMinimo;
    return true;
}

bool Producto::setTotalVendido(int nuevoTotalVendido) {
    if (nuevoTotalVendido < 0) {
        return false;
    }
    totalVendido = nuevoTotalVendido;
    return true;
}

void Producto::setEliminado(bool estado) { eliminado = estado; }

void Producto::setFechaCreacion(time_t fecha) { fechaCreacion = fecha; }

void Producto::setFechaUltimaModificacion(time_t fecha) { fechaUltimaModificacion = fecha; }

bool Producto::agregarTransaccionID(int idTransaccion) {
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

bool Producto::eliminarTransaccionID(int idTransaccion) {
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

bool Producto::aumentarStock(int cantidad) {
    if (cantidad <= 0) {
        return false;
    }
    stock += cantidad;
    return true;
}

bool Producto::reducirStock(int cantidad) {
    if (cantidad <= 0 || cantidad > stock) {
        return false;
    }
    stock -= cantidad;
    totalVendido += cantidad;
    return true;
}

bool Producto::validarDatosBasicos() const {
    return codigo[0] != '\0' && nombre[0] != '\0' && precio >= 0.0f && stock >= 0 &&
           idProveedor > 0;
}

void Producto::mostrarInformacionBasica() const {
    cout << "ID: " << id << " | Codigo: " << codigo << " | Nombre: " << nombre
              << " | Precio: " << precio << " | Stock: " << stock << endl;
}

void Producto::mostrarInformacionCompleta() const {
    cout << "ID: " << id << endl;
    cout << "Codigo: " << codigo << endl;
    cout << "Nombre: " << nombre << endl;
    cout << "Descripcion: " << descripcion << endl;
    cout << "Precio: " << precio << endl;
    cout << "Stock: " << stock << endl;
    cout << "Proveedor ID: " << idProveedor << endl;
    cout << "Stock minimo: " << stockMinimo << endl;
    cout << "Total vendido: " << totalVendido << endl;
}

size_t Producto::obtenerTamano() { return sizeof(Producto); }
