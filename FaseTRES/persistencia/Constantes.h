#ifndef CONSTANTES_HPP
#define CONSTANTES_HPP

struct ArchivoHeader {
    int cantidadRegistros;
    int proximoID;
    int registrosActivos;
    int version;
};

namespace Constantes {
constexpr int VERSION_ARCHIVO = 1;

constexpr const char* RUTA_TIENDA = "datos/tienda.bin";
constexpr const char* RUTA_PRODUCTOS = "datos/productos.bin";
constexpr const char* RUTA_PROVEEDORES = "datos/proveedores.bin";
constexpr const char* RUTA_CLIENTES = "datos/clientes.bin";
constexpr const char* RUTA_TRANSACCIONES = "datos/transacciones.bin";

constexpr int MAX_RELACIONES = 100;
constexpr int MAX_DETALLES_TRANSACCION = 50;
}  // namespace Constantes

#endif
