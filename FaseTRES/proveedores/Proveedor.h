#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include <cstddef>
#include <ctime>

using namespace std;

class Proveedor {
private:
    int id;
    char telefono[20];
    char nombre[100];
    char rif[20];
    char email[100];
    char direccion[200];
    int productosIDs[100];
    int cantidadProductos;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Proveedor();
    Proveedor(const char* nombre, const char* rif, const char* email);
    Proveedor(const Proveedor& other);
    ~Proveedor();

    int getId() const;
    const char* getTelefono() const;
    const char* getNombre() const;
    const char* getRif() const;
    const char* getEmail() const;
    const char* getDireccion() const;
    int getCantidadProductos() const;
    bool isEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    bool setId(int nuevoId);
    bool setTelefono(const char* nuevoTelefono);
    bool setNombre(const char* nuevoNombre);
    bool setRif(const char* nuevoRif);
    bool setEmail(const char* nuevoEmail);
    bool setDireccion(const char* nuevaDireccion);
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    bool agregarProductoID(int idProducto);
    bool eliminarProductoID(int idProducto);
    bool contieneProductoID(int idProducto) const;

    bool validarDatosBasicos() const;
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static size_t obtenerTamano();
};

#endif
