#include "operacionesProductos.h"

using namespace std;

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../persistencia/Constantes.h"
#include "../persistencia/GestorArchivos.h"
#include "../proveedores/Proveedor.h"
#include "../utilidades/Formatos.h"
#include "../utilidades/Validaciones.h"
#include "Producto.h"

namespace {
bool codigoDuplicado(const char* codigo, int idIgnorar) {
    vector<Producto> productos;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PRODUCTOS, productos);
    for (size_t i = 0; i < productos.size(); ++i) {
        if (productos[i].getId() != idIgnorar && strcmp(productos[i].getCodigo(), codigo) == 0) {
            return true;
        }
    }
    return false;
}

bool proveedorValido(int idProveedor, Proveedor& proveedor, int& indice) {
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES, idProveedor, proveedor) ||
        proveedor.isEliminado()) {
        return false;
    }
    indice = GestorArchivos::obtenerIndicePorId<Proveedor>(Constantes::RUTA_PROVEEDORES, idProveedor);
    return indice >= 0;
}
}  // namespace

void registrarProducto(Tienda& tienda) {
    (void)tienda;

    if (GestorArchivos::contarRegistrosActivos<Proveedor>(Constantes::RUTA_PROVEEDORES) == 0) {
        cout << "\nDebe registrar al menos un proveedor antes de crear productos.\n";
        return;
    }

    Producto producto;
    char buffer[200];

    cout << "\n--- Registrar Producto ---\n";

    cout << "Codigo: ";
    cin.getline(buffer, 20);
    if (buffer[0] == '\0' || codigoDuplicado(buffer, 0) || !producto.setCodigo(buffer)) {
        cout << "Codigo invalido o duplicado.\n";
        return;
    }

    cout << "Nombre: ";
    cin.getline(buffer, 100);
    if (!producto.setNombre(buffer)) {
        cout << "Nombre invalido.\n";
        return;
    }

    cout << "Descripcion: ";
    cin.getline(buffer, 200);
    producto.setDescripcion(buffer);

    int idProveedor = 0;
    cout << "ID del proveedor: ";
    cin >> idProveedor;
    Formatos::limpiarBuffer();

    Proveedor proveedor;
    int indiceProveedor = -1;
    if (!proveedorValido(idProveedor, proveedor, indiceProveedor) || !producto.setIdProveedor(idProveedor)) {
        cout << "Proveedor invalido.\n";
        return;
    }

    float precio = 0.0f;
    cout << "Precio: ";
    cin >> precio;
    if (!producto.setPrecio(precio)) {
        Formatos::limpiarBuffer();
        cout << "Precio invalido.\n";
        return;
    }

    int stock = 0;
    cout << "Stock inicial: ";
    cin >> stock;
    if (!producto.setStock(stock)) {
        Formatos::limpiarBuffer();
        cout << "Stock invalido.\n";
        return;
    }

    int stockMinimo = 0;
    cout << "Stock minimo: ";
    cin >> stockMinimo;
    Formatos::limpiarBuffer();
    if (!producto.setStockMinimo(stockMinimo)) {
        cout << "Stock minimo invalido.\n";
        return;
    }

    producto.setTotalVendido(0);
    if (!producto.validarDatosBasicos()) {
        cout << "Datos incompletos o invalidos.\n";
        return;
    }

    if (!GestorArchivos::guardarRegistro(Constantes::RUTA_PRODUCTOS, producto)) {
        cout << "No se pudo guardar el producto.\n";
        return;
    }

    proveedor.agregarProductoID(producto.getId());
    GestorArchivos::actualizarRegistro(Constantes::RUTA_PROVEEDORES, indiceProveedor, proveedor);

    cout << "Producto registrado con ID: " << producto.getId() << "\n";
}

void listarProductos(Tienda& tienda) {
    (void)tienda;
    vector<Producto> productos;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PRODUCTOS, productos);

    if (productos.empty()) {
        cout << "\nNo hay productos registrados.\n";
        return;
    }

    cout << "\n--- Inventario de Productos ---\n";
    cout << left << setw(5) << "ID" << setw(12) << "Codigo" << setw(25)
              << "Nombre" << setw(22) << "Proveedor" << setw(10) << "Precio"
              << setw(8) << "Stock" << "\n";
    cout << string(82, '-') << "\n";

    for (size_t i = 0; i < productos.size(); ++i) {
        Proveedor proveedor;
        const bool existeProv = GestorArchivos::obtenerRegistroPorId(
            Constantes::RUTA_PROVEEDORES, productos[i].getIdProveedor(), proveedor);
        const char* nombreProveedor = (existeProv && !proveedor.isEliminado()) ? proveedor.getNombre()
                                                                               : "N/A";

        cout << left << setw(5) << productos[i].getId() << setw(12)
                  << productos[i].getCodigo() << setw(25) << productos[i].getNombre()
                  << setw(22) << nombreProveedor << setw(10) << fixed
                  << setprecision(2) << productos[i].getPrecio() << setw(8)
                  << productos[i].getStock() << "\n";
    }
}

void buscarProducto(Tienda& tienda) {
    (void)tienda;
    int opcion = -1;
    char criterio[120];

    cout << "\n--- Buscar Producto ---\n";
    cout << "1. Por ID\n2. Por Codigo\n3. Por Nombre\n0. Cancelar\nOpcion: ";
    cin >> opcion;
    Formatos::limpiarBuffer();

    if (opcion == 0) {
        return;
    }

    cout << "Criterio: ";
    cin.getline(criterio, 120);

    bool encontrado = false;
    if (opcion == 1) {
        Producto producto;
        const int id = atoi(criterio);
        if (GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PRODUCTOS, id, producto) &&
            !producto.isEliminado()) {
            producto.mostrarInformacionCompleta();
            encontrado = true;
        }
    } else {
        vector<Producto> productos;
        GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PRODUCTOS, productos);
        for (size_t i = 0; i < productos.size(); ++i) {
            bool coincide = false;
            if (opcion == 2 && strcmp(productos[i].getCodigo(), criterio) == 0) {
                coincide = true;
            }
            if (opcion == 3 && strstr(productos[i].getNombre(), criterio) != nullptr) {
                coincide = true;
            }

            if (coincide) {
                productos[i].mostrarInformacionCompleta();
                cout << "-----------------------------\n";
                encontrado = true;
            }
        }
    }

    if (!encontrado) {
        cout << "No se encontraron coincidencias.\n";
    }
}

void actualizarProducto(Tienda& tienda) {
    (void)tienda;
    int id = 0;
    cout << "\nID del producto a actualizar: ";
    cin >> id;
    Formatos::limpiarBuffer();

    Producto producto;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PRODUCTOS, id, producto) ||
        producto.isEliminado()) {
        cout << "Producto no encontrado.\n";
        return;
    }

    const int indice = GestorArchivos::obtenerIndicePorId<Producto>(Constantes::RUTA_PRODUCTOS, id);
    if (indice < 0) {
        cout << "No se pudo localizar el producto en archivo.\n";
        return;
    }

    int opcion = -1;
    char buffer[200];

    do {
        cout << "\n1. Codigo\n2. Nombre\n3. Descripcion\n4. Precio\n5. Stock\n6. Stock minimo\n7. Proveedor\n0. Guardar y salir\nOpcion: ";
        cin >> opcion;
        Formatos::limpiarBuffer();

        switch (opcion) {
            case 1:
                cout << "Nuevo codigo: ";
                cin.getline(buffer, 20);
                if (buffer[0] == '\0' || codigoDuplicado(buffer, producto.getId()) ||
                    !producto.setCodigo(buffer)) {
                    cout << "Codigo invalido o duplicado.\n";
                }
                break;
            case 2:
                cout << "Nuevo nombre: ";
                cin.getline(buffer, 100);
                if (!producto.setNombre(buffer)) {
                    cout << "Nombre invalido.\n";
                }
                break;
            case 3:
                cout << "Nueva descripcion: ";
                cin.getline(buffer, 200);
                producto.setDescripcion(buffer);
                break;
            case 4: {
                float precio = 0.0f;
                cout << "Nuevo precio: ";
                cin >> precio;
                Formatos::limpiarBuffer();
                if (!producto.setPrecio(precio)) {
                    cout << "Precio invalido.\n";
                }
                break;
            }
            case 5: {
                int stock = 0;
                cout << "Nuevo stock: ";
                cin >> stock;
                Formatos::limpiarBuffer();
                if (!producto.setStock(stock)) {
                    cout << "Stock invalido.\n";
                }
                break;
            }
            case 6: {
                int stockMinimo = 0;
                cout << "Nuevo stock minimo: ";
                cin >> stockMinimo;
                Formatos::limpiarBuffer();
                if (!producto.setStockMinimo(stockMinimo)) {
                    cout << "Stock minimo invalido.\n";
                }
                break;
            }
            case 7: {
                int idProveedor = 0;
                cout << "Nuevo ID proveedor: ";
                cin >> idProveedor;
                Formatos::limpiarBuffer();
                Proveedor proveedor;
                int indiceProveedor = -1;
                if (!proveedorValido(idProveedor, proveedor, indiceProveedor) ||
                    !producto.setIdProveedor(idProveedor)) {
                    cout << "Proveedor invalido.\n";
                }
                break;
            }
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }
    } while (opcion != 0);

    if (GestorArchivos::actualizarRegistro(Constantes::RUTA_PRODUCTOS, indice, producto)) {
        cout << "Producto actualizado correctamente.\n";
    } else {
        cout << "No se pudo actualizar el producto.\n";
    }
}

void eliminarProducto(Tienda& tienda) {
    (void)tienda;
    int id = 0;
    cout << "\nID del producto a eliminar: ";
    cin >> id;
    Formatos::limpiarBuffer();

    Producto producto;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PRODUCTOS, id, producto) ||
        producto.isEliminado()) {
        cout << "Producto no encontrado.\n";
        return;
    }

    char confirmar = 'N';
    cout << "Confirmar eliminacion de " << producto.getNombre() << " (S/N): ";
    cin >> confirmar;
    Formatos::limpiarBuffer();

    if (confirmar != 'S' && confirmar != 's') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (!GestorArchivos::eliminarRegistroLogico<Producto>(Constantes::RUTA_PRODUCTOS, id)) {
        cout << "No se pudo eliminar el producto.\n";
        return;
    }

    Proveedor proveedor;
    const int indiceProveedor = GestorArchivos::obtenerIndicePorId<Proveedor>(
        Constantes::RUTA_PROVEEDORES, producto.getIdProveedor());
    if (indiceProveedor >= 0 &&
        GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES, producto.getIdProveedor(),
                                             proveedor) &&
        !proveedor.isEliminado()) {
        proveedor.eliminarProductoID(id);
        GestorArchivos::actualizarRegistro(Constantes::RUTA_PROVEEDORES, indiceProveedor, proveedor);
    }

    cout << "Producto eliminado correctamente.\n";
}

void productosStockCritico(Tienda& tienda) {
    (void)tienda;
    vector<Producto> productos;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PRODUCTOS, productos);

    bool encontrado = false;
    cout << "\n--- Productos con Stock Critico ---\n";
    for (size_t i = 0; i < productos.size(); ++i) {
        if (productos[i].getStock() <= productos[i].getStockMinimo()) {
            cout << "ID: " << productos[i].getId() << " | " << productos[i].getNombre()
                      << " | Stock: " << productos[i].getStock()
                      << " | Minimo: " << productos[i].getStockMinimo() << "\n";
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No hay productos con stock critico.\n";
    }
}
