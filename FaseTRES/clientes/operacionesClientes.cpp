#include "operacionesClientes.h"

using namespace std;

#include <cstdlib>
#include <cstring>
#include <iomanip>
#include <iostream>
#include <vector>

#include "../persistencia/Constantes.h"
#include "../persistencia/GestorArchivos.h"
#include "../utilidades/Formatos.h"
#include "../utilidades/Validaciones.h"
#include "Cliente.h"

namespace {
bool cedulaDuplicada(const char* cedula, int idIgnorar) {
    vector<Cliente> clientes;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_CLIENTES, clientes);
    for (size_t i = 0; i < clientes.size(); ++i) {
        if (clientes[i].getId() != idIgnorar && strcmp(clientes[i].getCedula(), cedula) == 0) {
            return true;
        }
    }
    return false;
}
}  // namespace

void registrarCliente(Tienda& tienda) {
    (void)tienda;
    Cliente cliente;
    char buffer[200];

    cout << "\n--- Registrar Cliente ---\n";

    cout << "Cedula: ";
    cin.getline(buffer, 20);
    if (buffer[0] == '\0') {
        cout << "Cedula invalida.\n";
        return;
    }
    if (cedulaDuplicada(buffer, 0)) {
        cout << "La cedula ya existe.\n";
        return;
    }
    cliente.setCedula(buffer);

    cout << "Nombre: ";
    cin.getline(buffer, 100);
    if (!cliente.setNombre(buffer)) {
        cout << "Nombre invalido.\n";
        return;
    }

    cout << "Email: ";
    cin.getline(buffer, 100);
    if (!Validaciones::validarEmail(buffer)) {
        cout << "Email invalido.\n";
        return;
    }
    cliente.setEmail(buffer);

    cout << "Telefono: ";
    cin.getline(buffer, 20);
    if (!Validaciones::validarTelefono(buffer)) {
        cout << "Telefono invalido.\n";
        return;
    }
    cliente.setTelefono(buffer);

    cout << "Direccion: ";
    cin.getline(buffer, 200);
    cliente.setDireccion(buffer);

    if (!cliente.validarDatosBasicos()) {
        cout << "Datos incompletos.\n";
        return;
    }

    if (GestorArchivos::guardarRegistro(Constantes::RUTA_CLIENTES, cliente)) {
        cout << "Cliente registrado con ID: " << cliente.getId() << "\n";
    } else {
        cout << "No se pudo guardar el cliente.\n";
    }
}

void listarClientes(Tienda& tienda) {
    (void)tienda;
    vector<Cliente> clientes;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_CLIENTES, clientes);

    if (clientes.empty()) {
        cout << "\nNo hay clientes registrados.\n";
        return;
    }

    cout << "\n--- Listado de Clientes ---\n";
    cout << left << setw(5) << "ID" << setw(16) << "Cedula" << setw(26)
              << "Nombre" << setw(28) << "Email" << setw(16) << "Telefono" << "\n";
    cout << string(91, '-') << "\n";

    for (size_t i = 0; i < clientes.size(); ++i) {
        cout << left << setw(5) << clientes[i].getId() << setw(16)
                  << clientes[i].getCedula() << setw(26) << clientes[i].getNombre()
                  << setw(28) << clientes[i].getEmail() << setw(16)
                  << clientes[i].getTelefono() << "\n";
    }
}

void buscarCliente(Tienda& tienda) {
    (void)tienda;
    int opcion = -1;
    char criterio[100];

    cout << "\n--- Buscar Cliente ---\n";
    cout << "1. Por ID\n2. Por Cedula\n3. Por Nombre\n0. Cancelar\nOpcion: ";
    cin >> opcion;
    Formatos::limpiarBuffer();

    if (opcion == 0) {
        return;
    }

    cout << "Criterio: ";
    cin.getline(criterio, 100);

    vector<Cliente> clientes;
    GestorArchivos::listarRegistrosActivos(Constantes::RUTA_CLIENTES, clientes);

    bool encontrado = false;
    for (size_t i = 0; i < clientes.size(); ++i) {
        bool coincide = false;

        if (opcion == 1 && clientes[i].getId() == atoi(criterio)) {
            coincide = true;
        }
        if (opcion == 2 && strcmp(clientes[i].getCedula(), criterio) == 0) {
            coincide = true;
        }
        if (opcion == 3 && strstr(clientes[i].getNombre(), criterio) != nullptr) {
            coincide = true;
        }

        if (coincide) {
            clientes[i].mostrarInformacionCompleta();
            cout << "-----------------------------\n";
            encontrado = true;
        }
    }

    if (!encontrado) {
        cout << "No se encontraron clientes con ese criterio.\n";
    }
}

void actualizarCliente(Tienda& tienda) {
    (void)tienda;
    int id = 0;
    cout << "\nID del cliente a actualizar: ";
    cin >> id;
    Formatos::limpiarBuffer();

    Cliente cliente;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_CLIENTES, id, cliente) ||
        cliente.isEliminado()) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    const int indice = GestorArchivos::obtenerIndicePorId<Cliente>(Constantes::RUTA_CLIENTES, id);
    if (indice < 0) {
        cout << "No se pudo localizar el cliente en archivo.\n";
        return;
    }

    int opcion = -1;
    char buffer[200];
    do {
        cout << "\n1. Nombre\n2. Cedula\n3. Email\n4. Direccion\n5. Telefono\n0. Guardar y salir\nOpcion: ";
        cin >> opcion;
        Formatos::limpiarBuffer();

        switch (opcion) {
            case 1:
                cout << "Nuevo nombre: ";
                cin.getline(buffer, 100);
                if (!cliente.setNombre(buffer)) {
                    cout << "Nombre invalido.\n";
                }
                break;
            case 2:
                cout << "Nueva cedula: ";
                cin.getline(buffer, 20);
                if (buffer[0] == '\0' || cedulaDuplicada(buffer, cliente.getId())) {
                    cout << "Cedula invalida o duplicada.\n";
                } else {
                    cliente.setCedula(buffer);
                }
                break;
            case 3:
                cout << "Nuevo email: ";
                cin.getline(buffer, 100);
                if (!Validaciones::validarEmail(buffer)) {
                    cout << "Email invalido.\n";
                } else {
                    cliente.setEmail(buffer);
                }
                break;
            case 4:
                cout << "Nueva direccion: ";
                cin.getline(buffer, 200);
                cliente.setDireccion(buffer);
                break;
            case 5:
                cout << "Nuevo telefono: ";
                cin.getline(buffer, 20);
                if (!Validaciones::validarTelefono(buffer)) {
                    cout << "Telefono invalido.\n";
                } else {
                    cliente.setTelefono(buffer);
                }
                break;
            case 0:
                break;
            default:
                cout << "Opcion no valida.\n";
                break;
        }
    } while (opcion != 0);

    if (GestorArchivos::actualizarRegistro(Constantes::RUTA_CLIENTES, indice, cliente)) {
        cout << "Cliente actualizado correctamente.\n";
    } else {
        cout << "No se pudo actualizar el cliente.\n";
    }
}

void eliminarCliente(Tienda& tienda) {
    (void)tienda;
    int id = 0;
    cout << "\nID del cliente a eliminar: ";
    cin >> id;
    Formatos::limpiarBuffer();

    Cliente cliente;
    if (!GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_CLIENTES, id, cliente) ||
        cliente.isEliminado()) {
        cout << "Cliente no encontrado.\n";
        return;
    }

    char confirmar = 'N';
    cout << "Confirmar eliminacion de " << cliente.getNombre() << " (S/N): ";
    cin >> confirmar;
    Formatos::limpiarBuffer();

    if (confirmar != 'S' && confirmar != 's') {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (GestorArchivos::eliminarRegistroLogico<Cliente>(Constantes::RUTA_CLIENTES, id)) {
        cout << "Cliente eliminado correctamente.\n";
    } else {
        cout << "No se pudo eliminar el cliente.\n";
    }
}
