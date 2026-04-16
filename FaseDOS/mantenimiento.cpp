#include "mantenimiento.h"
#include "files.h"
#include <iostream>
#include <string>
#include <direct.h>
#include <sys/stat.h>

using namespace std;

void verificarIntegridadReferencial() {
    cout << "\n--- VERIFICANDO INTEGRIDAD REFERENCIAL ---" << endl;
    int errores = 0;

    // 1. Verificar Productos -> Proveedores
    ArchivoHeader pHeader = leerHeader("productos.bin");
    for (int i = 0; i < pHeader.cantidadRegistros; ++i) {
        Producto p = obtenerProducto(i + 1);
        if (p.id != 0 && !p.eliminado) {
            Proveedor prov = obtenerProveedor(p.idProveedor);
            if (prov.id == 0 || prov.eliminado) {
                cout << "Error: Producto ID " << p.id << " tiene una referencia rota al Proveedor ID " << p.idProveedor << endl;
                errores++;
            }
        }
    }

    // 2. Verificar Transacciones -> Productos y (Clientes/Proveedores)
    ArchivoHeader tHeader = leerHeader("transacciones.bin");
    for (int i = 0; i < tHeader.cantidadRegistros; ++i) {
        Transaccion t = obtenerTransaccion(i + 1);
        if (t.id != 0 && !t.eliminado) {
            for (int j = 0; j < t.cantidadDetalles; ++j) {
                Producto p = obtenerProducto(t.detalles[j].idProducto);
                if (p.id == 0 || p.eliminado) {
                    cout << "Error: Transaccion ID " << t.id << " tiene una referencia rota al Producto ID " << t.detalles[j].idProducto << endl;
                    errores++;
                }
            }

            if (strcmp(t.tipo, "Venta") == 0) {
                Cliente c = obtenerCliente(t.idRelacionado);
                if (c.id == 0 || c.eliminado) {
                    cout << "Error: Transaccion ID " << t.id << " (Venta) tiene una referencia rota al Cliente ID " << t.idRelacionado << endl;
                    errores++;
                }
            } else if (strcmp(t.tipo, "Compra") == 0) {
                Proveedor prov = obtenerProveedor(t.idRelacionado);
                if (prov.id == 0 || prov.eliminado) {
                    cout << "Error: Transaccion ID " << t.id << " (Compra) tiene una referencia rota al Proveedor ID " << t.idRelacionado << endl;
                    errores++;
                }
            }
        }
    }

    cout << "\n--- REPORTE DE SALUD ---" << endl;
    if (errores == 0) {
        cout << "OK: La integridad de la base de datos es correcta. No se encontraron referencias rotas." << endl;
    } else {
        cout << "ERRORES DETECTADOS: Se encontraron " << errores << " referencias rotas." << endl;
    }
}

void crearBackup() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    char buffer[80];
    strftime(buffer, 80, "backup_%Y-%m-%d_%H-%M-%S", ltm);
    string backupDir = buffer;

    if (_mkdir(backupDir.c_str()) == -1) {
        cout << "Error: No se pudo crear el directorio de backup." << endl;
        return;
    }

    const char* files[] = {"tienda.bin", "productos.bin", "proveedores.bin", "clientes.bin", "transacciones.bin"};
    for (const char* filename : files) {
        ifstream src(filename, ios::binary);
        ofstream dst(backupDir + "/" + filename, ios::binary);
        dst << src.rdbuf();
        src.close();
        dst.close();
    }

    cout << "\nBackup creado exitosamente en la carpeta: " << backupDir << endl;
}

void reporteStockCritico() {
    cout << "\n--- PRODUCTOS CON STOCK CRITICO ---" << endl;
    ArchivoHeader pHeader = leerHeader("productos.bin");
    bool encontrados = false;
    for (int i = 0; i < pHeader.cantidadRegistros; ++i) {
        Producto p = obtenerProducto(i + 1);
        if (p.id != 0 && !p.eliminado && p.stock <= p.stockMinimo) {
            cout << "ID: " << p.id << " | Nombre: " << p.nombre << " | Stock: " << p.stock << " | Stock Minimo: " << p.stockMinimo << endl;
            encontrados = true;
        }
    }
    if (!encontrados) {
        cout << "No hay productos con stock crítico." << endl;
    }
}

void reporteHistorialCliente() {
    cout << "\n--- HISTORIAL DE CLIENTE ---" << endl;
    int idCliente;
    cout << "Ingrese el ID del cliente: ";
    cin >> idCliente;

    Cliente c = obtenerCliente(idCliente);
    if (c.id == 0 || c.eliminado) {
        cout << "Error: Cliente no encontrado." << endl;
        return;
    }

    cout << "\nDatos del Cliente:" << endl;
    cout << "ID: " << c.id << " | Nombre: " << c.nombre << " | Cedula: " << c.cedula << endl;

    cout << "\nTransacciones de Venta:" << endl;
    ArchivoHeader tHeader = leerHeader("transacciones.bin");
    bool encontradas = false;
    for (int i = 0; i < tHeader.cantidadRegistros; ++i) {
        Transaccion t = obtenerTransaccion(i + 1);
        if (t.id != 0 && !t.eliminado && strcmp(t.tipo, "Venta") == 0 && t.idRelacionado == idCliente) {
            cout << "  Transaccion ID: " << t.id << " | Fecha: " << ctime(&t.fechaCreacion);
            for (int j = 0; j < t.cantidadDetalles; ++j) {
                Producto p = obtenerProducto(t.detalles[j].idProducto);
                cout << "    -> Producto: " << p.nombre << " | Cantidad: " << t.detalles[j].cantidad << " | Total: " << t.detalles[j].cantidad * t.detalles[j].precioUnitario << endl;
            }
            encontradas = true;
        }
    }

    if (!encontradas) {
        cout << "El cliente no tiene transacciones de venta registradas." << endl;
    }
}


void menuMantenimiento() {
    int opcion;
    do {
        cout << "\n--- MANTENIMIENTO Y REPORTES ---" << endl;
        cout << "1. Verificar Integridad de Datos" << endl;
        cout << "2. Crear Backup" << endl;
        cout << "3. Reporte: Productos con Stock Crítico" << endl;
        cout << "4. Reporte: Historial de un Cliente" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                verificarIntegridadReferencial();
                break;
            case 2:
                crearBackup();
                break;
            case 3:
                reporteStockCritico();
                break;
            case 4:
                reporteHistorialCliente();
                break;
            case 0:
                cout << "Volviendo al menu principal..." << endl;
                break;
            default:
                cout << "Opcion no valida." << endl;
                break;
        }
         if (opcion != 0) {
            cout << "\nPresione ENTER para continuar...";
            cin.get();
        }
    } while (opcion != 0);
}