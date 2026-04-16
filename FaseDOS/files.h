#ifndef FILES_H
#define FILES_H

#include "structs.h"
#include <fstream>

// Prototipos para la gestión de archivos y headers
void inicializarArchivos();
ArchivoHeader leerHeader(const char* nombreArchivo);
void actualizarHeader(const char* nombreArchivo, ArchivoHeader header);

// Prototipos para búsqueda de índices
int obtenerIndiceProducto(int id);
int obtenerIndiceProveedor(int id);
int obtenerIndiceCliente(int id);

// Funciones para Productos
void guardarProducto(Producto& producto);
Producto obtenerProducto(int id);
void actualizarProducto(const Producto& producto);
void eliminarProducto(int id);
int contarProductos();

// Funciones para Proveedores
void guardarProveedor(Proveedor& proveedor);
Proveedor obtenerProveedor(int id);
void actualizarProveedor(const Proveedor& proveedor);
void eliminarProveedor(int id);
int contarProveedores();

// Funciones para Clientes
void guardarCliente(Cliente& cliente);
Cliente obtenerCliente(int id);
void actualizarCliente(const Cliente& cliente);
void eliminarCliente(int id);
int contarClientes();

// Funciones para Transacciones
void guardarTransaccion(Transaccion& transaccion);
Transaccion obtenerTransaccion(int id);
int contarTransacciones();

// Lógica de negocio
bool realizarVenta(int idCliente, int idProducto, int cantidad);

#endif // FILES_H