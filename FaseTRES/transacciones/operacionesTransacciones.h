#ifndef OPERACIONES_TRANSACCIONES_HPP
#define OPERACIONES_TRANSACCIONES_HPP

#include "../tienda/Tienda.h"

void registrarCompra(Tienda& tienda);
void registrarVenta(Tienda& tienda);
void listarTransacciones(Tienda& tienda);
void buscarTransaccion(Tienda& tienda);
void cancelarTransaccion(Tienda& tienda);

#endif
