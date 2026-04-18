#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <cstddef>
#include <ctime>

using namespace std;

class Cliente {
private:
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    int transaccionesIDs[100];
    int cantidadTransacciones;
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Cliente();
    Cliente(const char* nombre, const char* cedula, const char* email);
    Cliente(const Cliente& other);
    ~Cliente();

    int getId() const;
    const char* getNombre() const;
    const char* getCedula() const;
    const char* getTelefono() const;
    const char* getEmail() const;
    const char* getDireccion() const;
    int getCantidadTransacciones() const;
    bool isEliminado() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    bool setId(int nuevoId);
    bool setNombre(const char* nuevoNombre);
    bool setCedula(const char* nuevaCedula);
    bool setTelefono(const char* nuevoTelefono);
    bool setEmail(const char* nuevoEmail);
    bool setDireccion(const char* nuevaDireccion);
    void setEliminado(bool estado);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    bool agregarTransaccionID(int idTransaccion);
    bool eliminarTransaccionID(int idTransaccion);

    bool validarDatosBasicos() const;
    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static size_t obtenerTamano();
};

#endif
