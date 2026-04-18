#ifndef TIENDA_HPP
#define TIENDA_HPP

#include <ctime>

using namespace std;

class Tienda {
private:
    char nombre[100];
    char rif[20];
    int transaccionesIDs[100];
    int cantidadTransacciones;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;

public:
    Tienda();
    Tienda(const char* nombre, const char* rif);
    Tienda(const Tienda& other);
    ~Tienda();

    const char* getNombre() const;
    const char* getRif() const;
    int getCantidadTransacciones() const;
    time_t getFechaCreacion() const;
    time_t getFechaUltimaModificacion() const;

    bool setNombre(const char* nuevoNombre);
    bool setRif(const char* nuevoRif);
    void setFechaCreacion(time_t fecha);
    void setFechaUltimaModificacion(time_t fecha);

    bool agregarTransaccionID(int id);
    bool eliminarTransaccionID(int id);

    void mostrarInformacionBasica() const;
    void mostrarInformacionCompleta() const;

    static size_t obtenerTamano();
};

#endif
