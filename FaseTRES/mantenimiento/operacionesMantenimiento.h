#ifndef OPERACIONES_MANTENIMIENTO_HPP
#define OPERACIONES_MANTENIMIENTO_HPP

#include "../tienda/Tienda.h"

void verificarIntegridadReferencial(Tienda& tienda);
void crearBackup(Tienda& tienda);
void reporteStockCritico(Tienda& tienda);
void reporteHistorialCliente(Tienda& tienda);
void menuMantenimiento(Tienda& tienda);

#endif
