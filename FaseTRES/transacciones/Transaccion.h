#ifndef TRANSACCION_HPP
#define TRANSACCION_HPP

#include <cstddef>
#include <ctime>

using namespace std;

#include "../persistencia/Constantes.h"

struct DetalleTransaccion {
    int idProducto;
    int cantidad;
    float precioUnitario;
};

class Transaccion {
private:
    int id;
    char tipo[10];
    int idRelacionado;
    DetalleTransaccion detalles[Constantes::MAX_DETALLES_TRANSACCION];
    int cantidadDetalles;
    float total;
    char descripcion[200];
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Transaccion();
    Transaccion(const char* tipo, int idRelacionado);
    Transaccion(const Transaccion& other);
    ~Transaccion();

    int getId() const;
    const char* getTipo() const;
    int getIdRelacionado() const;
    int getCantidadDetalles() const;
    float getTotal() const;
    const char* getDescripcion() const;
    bool isEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    const DetalleTransaccion* getDetalles() const;
    bool getDetalle(int indice, DetalleTransaccion& detalle) const;

    bool setId(int nuevoId);
    bool setTipo(const char* nuevoTipo);
    bool setIdRelacionado(int nuevoIdRelacionado);
    bool setDescripcion(const char* nuevaDescripcion);
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    bool agregarDetalle(int idProducto, int cantidad, float precioUnitario);
    bool limpiarDetalles();
    void recalcularTotal();

    bool validarDatosBasicos() const;
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static size_t obtenerTamano();
};

#endif
