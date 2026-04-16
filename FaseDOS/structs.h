#ifndef STRUCTS_H
#define STRUCTS_H

#include <ctime>

struct ArchivoHeader {
    int cantidadRegistros;      // Total histórico de registros
    int proximoID;              // Siguiente ID a asignar (Autoincremental)
    int registrosActivos;       // Registros que no están marcados como eliminados
    int version;                // Control de versión del archivo
};

struct Producto {
    // 1. Datos básicos
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    float precio;
    int stock;
    
    // 2. Llaves Foráneas
    int idProveedor;                 
    
    // 4. Estadísticas del registro
    int stockMinimo;                 
    int totalVendido;                
    
    // 5. Metadata de Control Obligatoria
    bool eliminado;                  // Para BORRADO LÓGICO
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Proveedor {
    int id;
    char telefono[20];
    char nombre[100];
    char rif[20];
    char email[100];
    char direccion[200];

    // Relaciones
    int productosIDs[100];
    int cantidadProductos;

    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct Cliente {
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];

    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaUltimaModificacion;
};

struct DetalleTransaccion {
    int idProducto;
    int cantidad;
    float precioUnitario;
};

struct Transaccion {
    int id;
    char tipo[10]; // "Venta" o "Compra"
    int idRelacionado; // idCliente o idProveedor
    
    DetalleTransaccion detalles[50]; // Array para múltiples productos
    int cantidadDetalles;

    float total;
    char descripcion[200];

    // Metadata
    bool eliminado;
    time_t fechaCreacion;
};

struct Tienda {
    char nombre[100];
    char rif[20];
    // Aquí se pueden agregar estadísticas globales, ej: totalVentas, totalCompras
};

//Prototipos
bool inventarioVacio(Tienda* tienda);
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif);
void liberarTienda(Tienda* tienda);

//Prototipos - Productos
void menuProductos(Tienda* tienda);
void mostrarTabla();
void imprimirFilaProducto(const Producto& p);
int obtenerIndice(Tienda* tienda, int idBuscar);

//Prototipos - Proveedores
void menuProveedores(Tienda* tienda);
bool noProveedores(Tienda* tienda);
void imprimirFilaProveedor(const Proveedor& prov);
void mostrarProveedores();
int obtenerIndiceProv(Tienda* tienda, int idBuscar);
bool existeProveedor(Tienda* tienda, int idProveedor);

//Prototipos - Utilidades
void dibujarTabla(int anchos[], int columnas);
bool compararLetras(const char* original, const char* busqueda);
void mostrarBanner(const char* titulo);
void obtenerFechaActual(char* fecha);

//Prototipos - Clientes
bool cedulaDuplicada(Tienda* tienda, const char* cedula);
void listarClientes(Tienda* tienda);
void imprimirFilaCliente(const Cliente& c);
void redimensionarClientes(Tienda* tienda);
bool noClientes(Tienda* tienda);
int obtenerIndiceCli(Tienda* tienda, int idBuscar);
void menuClientes(Tienda* tienda);
bool existeCliente(Tienda* tienda, int idCliente);

//Prototipos - Transacciones
void menuTransacciones(Tienda* tienda);

#endif