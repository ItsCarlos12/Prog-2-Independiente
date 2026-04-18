#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include <cstddef>
#include <ctime>

using namespace std;

class Producto {
private:
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    float precio;
    int stock;
    int idProveedor;
    int stockMinimo;
    int totalVendido;
    int transaccionesIDs[100];
    int cantidadTransacciones;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Producto();
    Producto(const char* codigo, const char* nombre, float precio, int stock, int idProveedor);
    Producto(const Producto& other);
    ~Producto();

    int getId() const;
    const char* getCodigo() const;
    const char* getNombre() const;
    const char* getDescripcion() const;
    float getPrecio() const;
    int getStock() const;
    int getIdProveedor() const;
    int getStockMinimo() const;
    int getTotalVendido() const;
    bool isEliminado() const;
    int getCantidadTransacciones() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    bool setId(int nuevoId);
    bool setCodigo(const char* nuevoCodigo);
    bool setNombre(const char* nuevoNombre);
    bool setDescripcion(const char* nuevaDescripcion);
    bool setPrecio(float nuevoPrecio);
    bool setStock(int nuevoStock);
    bool setIdProveedor(int nuevoIdProveedor);
    bool setStockMinimo(int nuevoStockMinimo);
    bool setTotalVendido(int nuevoTotalVendido);
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    bool agregarTransaccionID(int idTransaccion);
    bool eliminarTransaccionID(int idTransaccion);
    bool aumentarStock(int cantidad);
    bool reducirStock(int cantidad);

    bool validarDatosBasicos() const;
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static size_t obtenerTamano();
};

#endif
