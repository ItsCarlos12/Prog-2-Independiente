#include "operacionesTransacciones.h"

using namespace std;

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../clientes/Cliente.h"
#include "../persistencia/Constantes.h"
#include "../persistencia/GestorArchivos.h"
#include "../productos/Producto.h"
#include "../proveedores/Proveedor.h"
#include "../utilidades/Formatos.h"
#include "Transaccion.h"

namespace {
bool obtenerProductoActivo(int idProducto, Producto& producto, int& indiceProducto) {
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PRODUCTOS, idProducto, producto) ||
        producto.isEliminado()) {
        return false;
    }
    indiceProducto = GestorArchivos::obtenerIndicePorId<Producto>(Constantes::RUTA_PRODUCTOS, idProducto);
    return indiceProducto >= 0;
}

bool obtenerProveedorActivo(int idProveedor, Proveedor& proveedor, int& indiceProveedor) {
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES, idProveedor, proveedor) ||
        proveedor.isEliminado()) {
        return false;
    }
    indiceProveedor =
        GestorArchivos::obtenerIndicePorId<Proveedor>(Constantes::RUTA_PROVEEDORES, idProveedor);
    return indiceProveedor >= 0;
}

bool obtenerClienteActivo(int idCliente, Cliente& cliente, int& indiceCliente) {
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_CLIENTES, idCliente, cliente) ||
        cliente.isEliminado()) {
        return false;
    }
    indiceCliente = GestorArchivos::obtenerIndicePorId<Cliente>(Constantes::RUTA_CLIENTES, idCliente);
    return indiceCliente >= 0;
}

void imprimirFilaTransaccion(const Transaccion& t) {
    int idProducto = 0;
    int cantidad = 0;
    if (t.getCantidadDetalles() > 0) {
        DetalleTransaccion detalle{};
        t.getDetalle(0, detalle);
        idProducto = detalle.idProducto;
        cantidad = detalle.cantidad;
    }

    cout << left << setw(5) << t.getId() << setw(10) << t.getTipo()
              << setw(12) << idProducto << setw(12) << t.getIdRelacionado() << setw(10)
              << cantidad << setw(12) << fixed << setprecision(2) << t.getTotal()
              << setw(14) << Formatos::formatearFecha(t.getFechaCreacion()) << "\n";
}
}  // namespace

void registrarCompra(Tienda& tienda) {
    if (GestorArchivos::contarRegistrosActivos<Producto>(Constantes::RUTA_PRODUCTOS) == 0 ||
        GestorArchivos::contarRegistrosActivos<Proveedor>(Constantes::RUTA_PROVEEDORES) == 0) {
        cout << "\nDebe existir al menos un producto y un proveedor activos.\n";
        return;
    }

    int idProducto = 0;
    int idProveedor = 0;
    int cantidad = 0;
    float precioUnitario = 0.0f;

    cout << "\n--- Registrar Compra ---\n";
    cout << "ID producto: ";
    cin >> idProducto;
    cout << "ID proveedor: ";
    cin >> idProveedor;
    cout << "Cantidad: ";
    cin >> cantidad;
    cout << "Precio unitario de compra: ";
    cin >> precioUnitario;
    Formatos::limpiarBuffer();

    if (cantidad <= 0 || precioUnitario < 0.0f) {
        cout << "Cantidad o precio invalidos.\n";
        return;
    }

    Producto producto;
    int indiceProducto = -1;
    if (!obtenerProductoActivo(idProducto, producto, indiceProducto)) {
        cout << "Producto no encontrado.\n";
        return;
    }

    Proveedor proveedor;
    int indiceProveedor = -1;
    if (!obtenerProveedorActivo(idProveedor, proveedor, indiceProveedor)) {
        cout << "Proveedor no encontrado.\n";
        return;
    }

    Transaccion transaccion;
    transaccion.setTipo("Compra");
    transaccion.setIdRelacionado(idProveedor);
    transaccion.setDescripcion("Compra registrada a proveedor.");
    if (!transaccion.agregarDetalle(idProducto, cantidad, precioUnitario)) {
        cout << "No se pudo construir la transaccion.\n";
        return;
    }

    if (!GestorArchivos::guardarRegistro(Constantes::RUTA_TRANSACCIONES, transaccion)) {
        cout << "No se pudo guardar la transaccion.\n";
        return;
    }

    producto.aumentarStock(cantidad);
    producto.agregarTransaccionID(transaccion.getId());
    GestorArchivos::actualizarRegistro(Constantes::RUTA_PRODUCTOS, indiceProducto, producto);

    proveedor.agregarProductoID(idProducto);
    GestorArchivos::actualizarRegistro(Constantes::RUTA_PROVEEDORES, indiceProveedor, proveedor);

    tienda.agregarTransaccionID(transaccion.getId());
    cout << "Compra registrada con ID transaccion: " << transaccion.getId() << "\n";
}

void registrarVenta(Tienda& tienda) {
    if (GestorArchivos::contarRegistrosActivos<Producto>(Constantes::RUTA_PRODUCTOS) == 0 ||
        GestorArchivos::contarRegistrosActivos<Cliente>(Constantes::RUTA_CLIENTES) == 0) {
        cout << "\nDebe existir al menos un producto y un cliente activos.\n";
        return;
    }

    int idProducto = 0;
    int idCliente = 0;
    int cantidad = 0;

    cout << "\n--- Registrar Venta ---\n";
    cout << "ID producto: ";
    cin >> idProducto;
    cout << "ID cliente: ";
    cin >> idCliente;
    cout << "Cantidad: ";
    cin >> cantidad;
    Formatos::limpiarBuffer();

    if (cantidad <= 0) {
        cout << "Cantidad invalida.\n";
        return;
    }

    Producto producto;
    int indiceProducto = -1;
    if (!obtenerProductoActivo(idProducto, producto, indiceProducto)) {
        cout << "Producto no encontrado.\n";
        return;
    }

    Cliente cliente;
    int indiceCliente = -1;
    if (!obtenerClienteActivo(idCliente, cliente, indiceCliente)) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    if (producto.getStock() < cantidad) {
        cout << "Stock insuficiente. Disponible: " << producto.getStock() << "\n";
        return;
    }

    Transaccion transaccion;
    transaccion.setTipo("Venta");
    transaccion.setIdRelacionado(idCliente);
    transaccion.setDescripcion("Venta registrada a cliente.");
    if (!transaccion.agregarDetalle(idProducto, cantidad, producto.getPrecio())) {
        cout << "No se pudo construir la transaccion.\n";
        return;
    }

    if (!GestorArchivos::guardarRegistro(Constantes::RUTA_TRANSACCIONES, transaccion)) {
        cout << "No se pudo guardar la transaccion.\n";
        return;
    }

    producto.reducirStock(cantidad);
    producto.agregarTransaccionID(transaccion.getId());
    GestorArchivos::actualizarRegistro(Constantes::RUTA_PRODUCTOS, indiceProducto, producto);

    cliente.agregarTransaccionID(transaccion.getId());
    GestorArchivos::actualizarRegistro(Constantes::RUTA_CLIENTES, indiceCliente, cliente);

    tienda.agregarTransaccionID(transaccion.getId());
    cout << "Venta registrada con ID transaccion: " << transaccion.getId() << "\n";
}

void listarTransacciones(Tienda& tienda) {
    (void)tienda;

    vector<Transaccion> transacciones;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_TRANSACCIONES, transacciones);

    if (transacciones.empty()) {
        cout << "\nNo hay transacciones registradas.\n";
        return;
    }

    cout << "\n--- Historial de Transacciones ---\n";
    cout << left << setw(5) << "ID" << setw(10) << "Tipo" << setw(12)
              << "IDProd" << setw(12) << "IDRel" << setw(10) << "Cantidad"
              << setw(12) << "Total" << setw(14) << "Fecha" << "\n";
    cout << string(75, '-') << "\n";

    for (size_t i = 0; i < transacciones.size(); ++i) {
        imprimirFilaTransaccion(transacciones[i]);
    }
}

void buscarTransaccion(Tienda& tienda) {
    (void)tienda;

    int opcion = -1;
    char criterio[100];
    cout << "\n--- Buscar Transaccion ---\n";
    cout << "1. Por ID\n2. Por Tipo\n3. Por ID Relacionado\n4. Por Fecha (DD/MM/AAAA)\n0. Cancelar\nOpcion: ";
    cin >> opcion;
    Formatos::limpiarBuffer();

    if (opcion == 0) {
        return;
    }

    cout << "Criterio: ";
    cin.getline(criterio, 100);

    vector<Transaccion> transacciones;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_TRANSACCIONES, transacciones);

    bool encontrado = false;
    for (size_t i = 0; i < transacciones.size(); ++i) {
        bool coincide = false;
        if (opcion == 1 && transacciones[i].getId() == atoi(criterio)) {
            coincide = true;
        }
        if (opcion == 2 && strcmp(transacciones[i].getTipo(), criterio) == 0) {
            coincide = true;
        }
        if (opcion == 3 && transacciones[i].getIdRelacionado() == atoi(criterio)) {
            coincide = true;
        }
        if (opcion == 4 && Formatos::formatearFecha(transacciones[i].getFechaCreacion()) == criterio) {
            coincide = true;
        }

        if (coincide) {
            transacciones[i].mostrarInformacionCompleta();
            cout << "-----------------------------\n";
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron transacciones con ese criterio.\n";
    }
}

void cancelarTransaccion(Tienda& tienda) {
    int idTransaccion = 0;
    cout << "\nID de transaccion a cancelar: ";
    cin >> idTransaccion;
    Formatos::limpiarBuffer();

    Transaccion transaccion;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_TRANSACCIONES, idTransaccion, transaccion) ||
        transaccion.isEliminado()) {
        cout << "Transaccion no encontrada.\n";
        return;
    }

    char confirmar = 'N';
    cout << "Se revertira el stock asociado. Confirmar (S/N): ";
    cin >> confirmar;
    Formatos::limpiarBuffer();

    if (confirmar != 'S' && confirmar != 's') {
        cout << "Operacion cancelada.\n";
        return;
    }

    for (int i = 0; i < transaccion.getCantidadDetalles(); ++i) {
        DetalleTransaccion detalle{};
        if (!transaccion.getDetalle(i, detalle)) {
            continue;
        }

        Producto producto;
        int indiceProducto = -1;
        if (!obtenerProductoActivo(detalle.idProducto, producto, indiceProducto)) {
            continue;
        }

        if (strcmp(transaccion.getTipo(), "Compra") == 0) {
            if (producto.getStock() < detalle.cantidad) {
                cout << "No se puede anular: stock actual insuficiente para revertir compra en producto ID "
                          << detalle.idProducto << "\n";
                return;
            }
            producto.setStock(producto.getStock() - detalle.cantidad);
        } else {
            producto.aumentarStock(detalle.cantidad);
        }
        GestorArchivos::actualizarRegistro(Constantes::RUTA_PRODUCTOS, indiceProducto, producto);
    }

    if (GestorArchivos::eliminarRegistroLogico<Transaccion>(Constantes::RUTA_TRANSACCIONES,
                                                            idTransaccion)) {
        tienda.eliminarTransaccionID(idTransaccion);
        cout << "Transaccion cancelada correctamente.\n";
    } else {
        cout << "No se pudo cancelar la transaccion.\n";
    }
}
