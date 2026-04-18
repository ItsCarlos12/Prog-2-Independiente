#ifndef OPERACIONES_PRODUCTOS_HPP
#define OPERACIONES_PRODUCTOS_HPP

#include "../tienda/Tienda.h"

void registrarProducto(Tienda& tienda);
void buscarProducto(Tienda& tienda);
void actualizarProducto(Tienda& tienda);
void listarProductos(Tienda& tienda);
void eliminarProducto(Tienda& tienda);
void productosStockCritico(Tienda& tienda);

#endif
