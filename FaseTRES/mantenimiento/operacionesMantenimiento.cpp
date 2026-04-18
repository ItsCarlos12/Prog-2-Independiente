#include "operacionesMantenimiento.h"

using namespace std;

#include <cstring>
#include <filesystem>
#include <iostream>
#include <vector>

#include "../clientes/Cliente.h"
#include "../persistencia/Constantes.h"
#include "../persistencia/GestorArchivos.h"
#include "../productos/Producto.h"
#include "../proveedores/Proveedor.h"
#include "../transacciones/Transaccion.h"
#include "../utilidades/Formatos.h"

void verificarIntegridadReferencial(Tienda& tienda) {
    (void)tienda;

    int errores = 0;
    vector<Producto> productos;
    vector<Transaccion> transacciones;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PRODUCTOS, productos);
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_TRANSACCIONES, transacciones);

    for (size_t i = 0; i < productos.size(); ++i) {
        Proveedor proveedor;
        if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES,
                                                  productos[i].getIdProveedor(), proveedor) ||
            proveedor.isEliminado()) {
            cout << "Referencia rota: Producto ID " << productos[i].getId()
                      << " apunta a proveedor inexistente/eliminado (ID "
                      << productos[i].getIdProveedor() << ").\n";
            errores++;
        }
    }

    for (size_t i = 0; i < transacciones.size(); ++i) {
        for (int j = 0; j < transacciones[i].getCantidadDetalles(); ++j) {
            DetalleTransaccion detalle{};
            if (!transacciones[i].getDetalle(j, detalle)) {
                continue;
            }

            Producto producto;
            if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PRODUCTOS, detalle.idProducto,
                                                      producto) ||
                producto.isEliminado()) {
                cout << "Referencia rota: Transaccion ID " << transacciones[i].getId()
                          << " apunta a producto inexistente/eliminado (ID " << detalle.idProducto
                          << ").\n";
                errores++;
            }
        }

        if (strcmp(transacciones[i].getTipo(), "Venta") == 0) {
            Cliente cliente;
            if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_CLIENTES,
                                                      transacciones[i].getIdRelacionado(), cliente) ||
                cliente.isEliminado()) {
                cout << "Referencia rota: Transaccion de venta ID " << transacciones[i].getId()
                          << " apunta a cliente inexistente/eliminado (ID "
                          << transacciones[i].getIdRelacionado() << ").\n";
                errores++;
            }
        } else if (strcmp(transacciones[i].getTipo(), "Compra") == 0) {
            Proveedor proveedor;
            if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES,
                                                      transacciones[i].getIdRelacionado(), proveedor) ||
                proveedor.isEliminado()) {
                cout << "Referencia rota: Transaccion de compra ID " << transacciones[i].getId()
                          << " apunta a proveedor inexistente/eliminado (ID "
                          << transacciones[i].getIdRelacionado() << ").\n";
                errores++;
            }
        }
    }

    if (errores == 0) {
        cout << "\nIntegridad OK: no se encontraron referencias rotas.\n";
    } else {
        cout << "\nIntegridad con errores: " << errores << " inconsistencias encontradas.\n";
    }
}

void crearBackup(Tienda& tienda) {
    (void)tienda;

    const time_t ahora = time(nullptr);
    const string sello = Formatos::formatearFecha(ahora);
    string nombreCarpeta = "datos/backup_" + sello;

    for (size_t i = 0; i < nombreCarpeta.size(); ++i) {
        if (nombreCarpeta[i] == '/') {
            nombreCarpeta[i] = '-';
        }
    }

    filesystem::create_directories(nombreCarpeta);

    const char* archivos[] = {Constantes::RUTA_TIENDA, Constantes::RUTA_PRODUCTOS,
                              Constantes::RUTA_PROVEEDORES, Constantes::RUTA_CLIENTES,
                              Constantes::RUTA_TRANSACCIONES};

    for (const char* ruta : archivos) {
        filesystem::path origen(ruta);
        if (filesystem::exists(origen)) {
            filesystem::path destino = filesystem::path(nombreCarpeta) / origen.filename();
            filesystem::copy_file(origen, destino,
                                       filesystem::copy_options::overwrite_existing);
        }
    }

    cout << "Backup creado en: " << nombreCarpeta << "\n";
}

void reporteStockCritico(Tienda& tienda) {
    (void)tienda;

    vector<Producto> productos;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PRODUCTOS, productos);
    bool hay = false;

    cout << "\n--- Reporte de Stock Critico ---\n";
    for (size_t i = 0; i < productos.size(); ++i) {
        if (productos[i].getStock() <= productos[i].getStockMinimo()) {
            cout << "ID: " << productos[i].getId() << " | " << productos[i].getNombre()
                      << " | Stock: " << productos[i].getStock()
                      << " | Minimo: " << productos[i].getStockMinimo() << "\n";
            hay = true;
        }
    }

    if (!hay) {
        cout << "No hay productos en estado critico.\n";
    }
}

void reporteHistorialCliente(Tienda& tienda) {
    (void)tienda;

    int idCliente = 0;
    cout << "\nID cliente para historial: ";
    cin >> idCliente;
    Formatos::limpiarBuffer();

    Cliente cliente;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_CLIENTES, idCliente, cliente) ||
        cliente.isEliminado()) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    vector<Transaccion> transacciones;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_TRANSACCIONES, transacciones);

    bool hay = false;
    cout << "\nHistorial de " << cliente.getNombre() << " (ID " << cliente.getId() << ")\n";
    cout << string(56, '-') << "\n";
    for (size_t i = 0; i < transacciones.size(); ++i) {
        if (strcmp(transacciones[i].getTipo(), "Venta") == 0 &&
            transacciones[i].getIdRelacionado() == idCliente) {
            transacciones[i].mostrarInformacionBasica();
            hay = true;
        }
    }

    if (!hay) {
        cout << "No hay ventas asociadas a este cliente.\n";
    }
}

void menuMantenimiento(Tienda& tienda) {
    int opcion = -1;

    do {
        cout << "\n------------------------------------------------------------\n";
        cout << " MENU DE MANTENIMIENTO Y REPORTES\n";
        cout << "------------------------------------------------------------\n";
        cout << " [1] Verificar integridad referencial\n";
        cout << " [2] Crear backup\n";
        cout << " [3] Reporte de stock critico\n";
        cout << " [4] Historial por cliente\n";
        cout << " [0] Volver al menu principal\n";
        cout << "------------------------------------------------------------\n";
        cout << " Seleccione una opcion: ";
        cin >> opcion;
        Formatos::limpiarBuffer();

        switch (opcion) {
            case 1:
                verificarIntegridadReferencial(tienda);
                break;
            case 2:
                crearBackup(tienda);
                break;
            case 3:
                reporteStockCritico(tienda);
                break;
            case 4:
                reporteHistorialCliente(tienda);
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }

        if (opcion != 0) {
            Formatos::pausar();
        }
    } while (opcion != 0);
}
