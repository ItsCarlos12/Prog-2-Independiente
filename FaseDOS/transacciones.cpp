#include <iostream>
#include <cstring>
#include <iomanip>
#include <fstream>
#include "structs.h"
#include "utilities.h"

using namespace std;

// Anchos para: ID, Tipo, Producto, Prov/Cli, Cant, Total, Fecha
static int anchosTr[] = {4, 10, 15, 10, 8, 10, 12};
static int colTr = 7;

void registrarCompra(Tienda* tienda) {
    if (contarRegistros("productos.dat", sizeof(Producto)) == 0) {
        cout << "Error: No hay productos registrados para comprar." << endl;
        return;
    }

    cout << "\n--- REGISTRAR COMPRA (A PROVEEDOR) ---" << endl;

    Transaccion t;

    int idProd;
    cout << "Ingrese ID del producto: ";
    cin >> idProd;
    int idxProd = obtenerIndiceProducto(idProd);

    if (idxProd == -1) {
        cout << "Error: El producto con ID " << idProd << " no existe." << endl;
        return;
    }

    int idProv;
    cout << "Ingrese ID del proveedor: ";
    cin >> idProv;
    if (!existeProveedor(idProv)) {
        cout << "Error: El proveedor con ID " << idProv << " no existe." << endl;
        return;
    }

    cout << "Cantidad a comprar: ";
    cin >> t.cantidad;
    if (t.cantidad <= 0) {
        cout << "Error: La cantidad debe ser mayor a 0." << endl;
        return;
    }

    cout << "Precio unitario de compra: ";
    cin >> t.precioUnitario;

    t.total = t.cantidad * t.precioUnitario;

    Producto p;
    leerRegistro("productos.dat", &p, idxProd);

    cout << "\n--- RESUMEN DE COMPRA ---" << endl;
    cout << "Producto: " << p.nombre << endl;
    cout << "Total a pagar: " << fixed << setprecision(2) << t.total << endl;

    char confirma;
    cout << "Confirmar registro de compra? (S/N): ";
    cin >> confirma;

    if (confirma == 's' || confirma == 'S') {
        p.stock += t.cantidad;
        guardarRegistro("productos.dat", &p, idxProd);

        t.id = obtenerSiguienteId("transacciones.dat");
        t.idProducto = idProd;
        t.idRelacionado = idProv;
        strcpy(t.tipo, "COMPRA");
        obtenerFechaActual(t.fecha);

        guardarRegistro("transacciones.dat", &t, t.id - 1);

        cout << "Compra registrada. Nuevo stock de " << p.nombre
             << ": " << p.stock << endl;
    } else {
        cout << "Operacion cancelada." << endl;
    }
}

void imprimirFilaTransaccion(const Transaccion& t) {
    cout << setfill(' ');
    cout << "| " << left << setw(anchosTr[0]) << t.id
         << "| " << setw(anchosTr[1]) << t.tipo
         << "| " << setw(anchosTr[2]) << t.idProducto
         << "| " << setw(anchosTr[3]) << t.idRelacionado
         << "| " << right << setw(anchosTr[4]) << t.cantidad
         << "| " << setw(anchosTr[5]) << fixed << setprecision(2) << t.total
         << "| " << left << setw(anchosTr[6]) << t.fecha << " |" << endl;
}

void registrarVenta(Tienda* tienda) {
    if (contarProductos() == 0) {
        cout << "Error: No hay productos en inventario para vender." << endl;
        return;
    }
     if (contarClientes() == 0) {
        cout << "Error: No hay clientes registrados para vender." << endl;
        return;
    }

    cout << "\n--- REGISTRAR VENTA (A CLIENTE) ---" << endl;

    int idProd;
    cout << "Ingrese ID del producto: ";
    cin >> idProd;

    int idCli;
    cout << "Ingrese ID del cliente: ";
    cin >> idCli;
    
    int cantidad;
    cout << "Cantidad a vender: ";
    cin >> cantidad;

    if (cantidad <= 0) {
        cout << "Error: La cantidad debe ser mayor a 0." << endl;
        return;
    }

    char confirma;
    cout << "Confirmar venta? (S/N): ";
    cin >> confirma;

    if (confirma == 's' || confirma == 'S') {
        if (realizarVenta(idCli, idProd, cantidad)) {
            cout << "Operacion completada." << endl;
        } else {
            cout << "La venta no pudo ser procesada." << endl;
        }
    } else {
        cout << "Venta cancelada." << endl;
    }
}

void buscarTransacciones(Tienda* tienda) {
    int numRegistros = contarRegistros("transacciones.dat", sizeof(Transaccion));
    if (numRegistros == 0) {
        cout << "\nNo hay transacciones registradas para realizar una busqueda." << endl;
        return;
    }

    int opcion;
    char criterio[100];

    cout << "\n--- Buscar Transacciones ---" << endl;
    cout << "1. Buscar por ID de transaccion" << endl;
    cout << "2. Buscar por ID de producto" << endl;
    cout << "3. Buscar por ID de cliente" << endl;
    cout << "4. Buscar por ID de proveedor" << endl;
    cout << "5. Buscar por fecha exacta (DD/MM/YYYY)" << endl;
    cout << "6. Buscar por tipo (COMPRA/VENTA)" << endl;
    cout << "0. Cancelar" << endl;
    cout << "Seleccione una opcion: ";
    cin >> opcion;
    cin.ignore();

    if (opcion == 0) return;

    cout << "Ingrese el valor a buscar: ";
    cin.getline(criterio, 100);

    bool encontrado = false;
    bool encabezadoMostrado = false;
    Transaccion t;

    for (int i = 0; i < numRegistros; i++) {
        if (leerRegistro("transacciones.dat", &t, i)) {
            bool coincide = false;
            switch (opcion) {
                case 1:
                    if (t.id == atoi(criterio)) coincide = true;
                    break;
                case 2:
                    if (t.idProducto == atoi(criterio)) coincide = true;
                    break;
                case 3:
                    if (strcmp(t.tipo, "VENTA") == 0 && t.idRelacionado == atoi(criterio)) coincide = true;
                    break;
                case 4:
                    if (strcmp(t.tipo, "COMPRA") == 0 && t.idRelacionado == atoi(criterio)) coincide = true;
                    break;
                case 5:
                    if (strcmp(t.fecha, criterio) == 0) coincide = true;
                    break;
                case 6:
                    if (strcasecmp(t.tipo, criterio) == 0) coincide = true;
                    break;
                default:
                    cout << "Opcion no valida." << endl;
                    return;
            }

            if (coincide) {
                if (!encabezadoMostrado) {
                    cout << "\nResultados encontrados:" << endl;
                    dibujarTabla(anchosTr, colTr);
                    cout << "| " << left << setw(anchosTr[0]) << "ID"
                         << "| " << setw(anchosTr[1]) << "Tipo"
                         << "| " << setw(anchosTr[2]) << "Prod"
                         << "| " << setw(anchosTr[3]) << "Rel ID"
                         << "| " << setw(anchosTr[4]) << "Cant"
                         << "| " << setw(anchosTr[5]) << "Total"
                         << "| " << setw(anchosTr[6]) << "Fecha" << " |" << endl;
                    dibujarTabla(anchosTr, colTr);
                    encabezadoMostrado = true;
                }
                imprimirFilaTransaccion(t);
                encontrado = true;
            }
        }
    }

    if (encontrado) {
        dibujarTabla(anchosTr, colTr);
    } else {
        cout << "\nNo se encontraron transacciones que coincidan con el criterio." << endl;
    }
}

void listarTransacciones(Tienda* tienda) {
    int numRegistros = contarRegistros("transacciones.dat", sizeof(Transaccion));
    if (numRegistros == 0) {
        cout << "\nNo hay transacciones registradas en el historial." << endl;
        return;
    }

    cout << "\n--- HISTORIAL DE TRANSACCIONES ---" << endl;
    dibujarTabla(anchosTr, colTr);
    cout << setfill(' ');
    cout << "| " << left << setw(anchosTr[0]) << "ID"
         << "| " << setw(anchosTr[1]) << "Tipo"
         << "| " << setw(anchosTr[2]) << "Producto"
         << "| " << setw(anchosTr[3]) << "Cli/Prov"
         << "| " << setw(anchosTr[4]) << "Cant"
         << "| " << setw(anchosTr[5]) << "Total"
         << "| " << setw(anchosTr[6]) << "Fecha" << " |" << endl;
    dibujarTabla(anchosTr, colTr);

    Transaccion t;
    for (int i = 0; i < numRegistros; i++) {
        if (leerRegistro("transacciones.dat", &t, i)) {
            imprimirFilaTransaccion(t);
        }
    }

    dibujarTabla(anchosTr, colTr);
    cout << " Total de movimientos registrados: " << numRegistros << endl;
}

int obtenerIndiceTransaccion(int idBuscar) {
    Transaccion t;
    int numRegistros = contarRegistros("transacciones.dat", sizeof(Transaccion));
    for (int i = 0; i < numRegistros; i++) {
        if (leerRegistro("transacciones.dat", &t, i)) {
            if (t.id == idBuscar) {
                return i;
            }
        }
    }
    return -1;
}

void cancelarTransaccion(Tienda* tienda) {
    if (contarRegistros("transacciones.dat", sizeof(Transaccion)) == 0) {
        cout << "No hay transacciones para anular." << endl;
        return;
    }

    int idBuscar;
    cout << "\n--- ANULAR TRANSACCION ---" << endl;
    cout << "Ingrese ID de la transaccion a anular: ";
    cin >> idBuscar;

    int idxTr = obtenerIndiceTransaccion(idBuscar);

    if (idxTr == -1) {
        cout << "Error: No se encontro la transaccion con ID " << idBuscar << endl;
        return;
    }

    Transaccion t;
    leerRegistro("transacciones.dat", &t, idxTr);
    int idxProd = obtenerIndiceProducto(t.idProducto);

    cout << "\nDetalles de la transaccion encontrada:" << endl;
    cout << "Tipo: " << t.tipo << " | Producto ID: " << t.idProducto
         << " | Cantidad: " << t.cantidad << " | Total: " << t.total << endl;

    cout << "\nATENCION: Se procedera a revertir el stock del producto." << endl;
    if (strcmp(t.tipo, "COMPRA") == 0) {
        cout << "Se RESTARAN " << t.cantidad << " unidades al stock." << endl;
    } else {
        cout << "Se SUMARAN " << t.cantidad << " unidades al stock." << endl;
    }

    char confirma;
    cout << "\nEsta seguro de que desea anular esta transaccion? (S/N): ";
    cin >> confirma;

    if (confirma == 's' || confirma == 'S') {
        if (idxProd != -1) {
            Producto p;
            leerRegistro("productos.dat", &p, idxProd);
            if (strcmp(t.tipo, "COMPRA") == 0) {
                p.stock -= t.cantidad;
            } else {
                p.stock += t.cantidad;
            }
            guardarRegistro("productos.dat", &p, idxProd);
        }

        ifstream inFile("transacciones.dat", ios::binary);
        ofstream outFile("temp.dat", ios::binary);
        Transaccion tempT;
        while (inFile.read(reinterpret_cast<char*>(&tempT), sizeof(Transaccion))) {
            if (tempT.id != idBuscar) {
                outFile.write(reinterpret_cast<char*>(&tempT), sizeof(Transaccion));
            }
        }
        inFile.close();
        outFile.close();

        remove("transacciones.dat");
        rename("temp.dat", "transacciones.dat");

        cout << "\nTransaccion anulada exitosamente. El inventario ha sido actualizado." << endl;
    } else {
        cout << "\nOperacion cancelada por el usuario." << endl;
    }
}

void menuTransacciones(Tienda* tienda) {
    int opcion;

    do {
        cout << "\n--- GESTION DE TRANSACCIONES ---" << endl;
        cout << "1. Registrar Compra (a Proveedor)" << endl;
        cout << "2. Registrar Venta (a Cliente)" << endl;
        cout << "3. Listar Historial de Movimientos" << endl;
        cout << "4. Buscar Transaccion" << endl;
        cout << "5. Anular/Cancelar Transaccion" << endl;
        cout << "0. Volver al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;
        cin.ignore();

        switch (opcion) {
            case 1:
                registrarCompra(tienda);
                break;
            case 2:
                registrarVenta(tienda);
                break;
            case 3:
                listarTransacciones(tienda);
                break;
            case 4:
                buscarTransacciones(tienda);
                break;
            case 5:
                cancelarTransaccion(tienda);
                break;
            case 0:
                cout << "Saliendo del modulo de transacciones..." << endl;
                break;
            default:
                cout << "Opcion no valida. Intente de nuevo." << endl;
                break;
        }

        if (opcion != 0) {
            cout << "\nPresione ENTER para continuar...";
            cin.get();
        }

    } while (opcion != 0);
}

