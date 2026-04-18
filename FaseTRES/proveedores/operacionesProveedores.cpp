#include "operacionesProveedores.h"

using namespace std;

#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../persistencia/Constantes.h"
#include "../persistencia/GestorArchivos.h"
#include "../productos/Producto.h"
#include "../utilidades/Formatos.h"
#include "../utilidades/Validaciones.h"
#include "Proveedor.h"

namespace {
bool rifDuplicado(const char* rif, int idIgnorar) {
    vector<Proveedor> proveedores;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PROVEEDORES, proveedores);
    for (size_t i = 0; i < proveedores.size(); ++i) {
        if (proveedores[i].getId() != idIgnorar && strcmp(proveedores[i].getRif(), rif) == 0) {
            return true;
        }
    }
    return false;
}

bool existeProductoActivoPorProveedor(int idProveedor) {
    vector<Producto> productos;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PRODUCTOS, productos);
    for (size_t i = 0; i < productos.size(); ++i) {
        if (productos[i].getIdProveedor() == idProveedor) {
            return true;
        }
    }
    return false;
}
}  // namespace

void registrarProveedor(Tienda& tienda) {
    (void)tienda;
    Proveedor proveedor;
    char buffer[200];

    cout << "\n--- Registrar Proveedor ---\n";

    cout << "RIF: ";
    cin.getline(buffer, 20);
    if (!Validaciones::validarRIF(buffer)) {
        cout << "RIF invalido.\n";
        return;
    }
    if (rifDuplicado(buffer, 0)) {
        cout << "Ya existe un proveedor con ese RIF.\n";
        return;
    }
    proveedor.setRif(buffer);

    cout << "Nombre: ";
    cin.getline(buffer, 100);
    if (!proveedor.setNombre(buffer)) {
        cout << "Nombre invalido.\n";
        return;
    }

    cout << "Email: ";
    cin.getline(buffer, 100);
    if (!Validaciones::validarEmail(buffer)) {
        cout << "Email invalido.\n";
        return;
    }
    proveedor.setEmail(buffer);

    cout << "Telefono: ";
    cin.getline(buffer, 20);
    if (!Validaciones::validarTelefono(buffer)) {
        cout << "Telefono invalido.\n";
        return;
    }
    proveedor.setTelefono(buffer);

    cout << "Direccion: ";
    cin.getline(buffer, 200);
    proveedor.setDireccion(buffer);

    if (!proveedor.validarDatosBasicos()) {
        cout << "Datos incompletos.\n";
        return;
    }

    if (GestorArchivos::guardarRegistro(Constantes::RUTA_PROVEEDORES, proveedor)) {
        cout << "Proveedor registrado con ID: " << proveedor.getId() << "\n";
    } else {
        cout << "No se pudo guardar el proveedor.\n";
    }
}

void listarProveedores(Tienda& tienda) {
    (void)tienda;
    vector<Proveedor> proveedores;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PROVEEDORES, proveedores);

    if (proveedores.empty()) {
        cout << "\nNo hay proveedores registrados.\n";
        return;
    }

    cout << "\n--- Listado de Proveedores ---\n";
    cout << left << setw(5) << "ID" << setw(15) << "RIF" << setw(24)
              << "Nombre" << setw(28) << "Email" << setw(16) << "Telefono"
              << "\n";
    cout << string(88, '-') << "\n";

    for (size_t i = 0; i < proveedores.size(); ++i) {
        cout << left << setw(5) << proveedores[i].getId() << setw(15)
                  << proveedores[i].getRif() << setw(24) << proveedores[i].getNombre()
                  << setw(28) << proveedores[i].getEmail() << setw(16)
                  << proveedores[i].getTelefono() << "\n";
    }
}

void buscarProveedor(Tienda& tienda) {
    (void)tienda;
    int opcion = -1;
    char criterio[100];

    cout << "\n--- Buscar Proveedor ---\n";
    cout << "1. Por ID\n2. Por Nombre\n3. Por RIF\n0. Cancelar\nOpcion: ";
    cin >> opcion;
    Formatos::limpiarBuffer();

    if (opcion == 0) {
        return;
    }

    cout << "Criterio: ";
    cin.getline(criterio, 100);

    vector<Proveedor> proveedores;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_PROVEEDORES, proveedores);

    bool encontrado = false;
    for (size_t i = 0; i < proveedores.size(); ++i) {
        bool coincide = false;

        if (opcion == 1 && proveedores[i].getId() == atoi(criterio)) {
            coincide = true;
        }
        if (opcion == 2 && strstr(proveedores[i].getNombre(), criterio) != nullptr) {
            coincide = true;
        }
        if (opcion == 3 && strcmp(proveedores[i].getRif(), criterio) == 0) {
            coincide = true;
        }

        if (coincide) {
            proveedores[i].mostrarInformacionCompleta();
            cout << "-----------------------------\n";
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron proveedores con ese criterio.\n";
    }
}

void actualizarProveedor(Tienda& tienda) {
    (void)tienda;
    int id = 0;
    cout << "\nID del proveedor a actualizar: ";
    cin >> id;
    Formatos::limpiarBuffer();

    Proveedor proveedor;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES, id, proveedor) ||
        proveedor.isEliminado()) {
        cout << "Proveedor no encontrado.\n";
        return;
    }

    const int indice = GestorArchivos::obtenerIndicePorId<Proveedor>(Constantes::RUTA_PROVEEDORES, id);
    if (indice < 0) {
        cout << "No se pudo localizar el proveedor en archivo.\n";
        return;
    }

    int opcion = -1;
    char buffer[200];
    do {
        cout << "\n1. Nombre\n2. RIF\n3. Email\n4. Direccion\n5. Telefono\n0. Guardar y salir\nOpcion: ";
        cin >> opcion;
        Formatos::limpiarBuffer();

        switch (opcion) {
            case 1:
                cout << "Nuevo nombre: ";
                cin.getline(buffer, 100);
                if (!proveedor.setNombre(buffer)) {
                    cout << "Nombre invalido.\n";
                }
                break;
            case 2:
                cout << "Nuevo RIF: ";
                cin.getline(buffer, 20);
                if (!Validaciones::validarRIF(buffer) || rifDuplicado(buffer, proveedor.getId())) {
                    cout << "RIF invalido o duplicado.\n";
                } else {
                    proveedor.setRif(buffer);
                }
                break;
            case 3:
                cout << "Nuevo email: ";
                cin.getline(buffer, 100);
                if (!Validaciones::validarEmail(buffer)) {
                    cout << "Email invalido.\n";
                } else {
                    proveedor.setEmail(buffer);
                }
                break;
            case 4:
                cout << "Nueva direccion: ";
                cin.getline(buffer, 200);
                proveedor.setDireccion(buffer);
                break;
            case 5:
                cout << "Nuevo telefono: ";
                cin.getline(buffer, 20);
                if (!Validaciones::validarTelefono(buffer)) {
                    cout << "Telefono invalido.\n";
                } else {
                    proveedor.setTelefono(buffer);
                }
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }
    } while (opcion != 0);

    if (GestorArchivos::actualizarRegistro(Constantes::RUTA_PROVEEDORES, indice, proveedor)) {
        cout << "Proveedor actualizado correctamente.\n";
    } else {
        cout << "No se pudo actualizar el proveedor.\n";
    }
}

void eliminarProveedor(Tienda& tienda) {
    (void)tienda;
    int id = 0;
    cout << "\nID del proveedor a eliminar: ";
    cin >> id;
    Formatos::limpiarBuffer();

    Proveedor proveedor;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES, id, proveedor) ||
        proveedor.isEliminado()) {
        cout << "Proveedor no encontrado.\n";
        return;
    }

    if (existeProductoActivoPorProveedor(id)) {
        cout << "No se puede eliminar: existen productos activos asociados.\n";
        return;
    }

    char confirmar = 'N';
    cout << "Confirmar eliminacion de " << proveedor.getNombre() << " (S/N): ";
    cin >> confirmar;
    Formatos::limpiarBuffer();

    if (confirmar != 'S' && confirmar != 's') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (GestorArchivos::eliminarRegistroLogico<Proveedor>(Constantes::RUTA_PROVEEDORES, id)) {
        cout << "Proveedor eliminado correctamente.\n";
    } else {
        cout << "No se pudo eliminar el proveedor.\n";
    }
}
