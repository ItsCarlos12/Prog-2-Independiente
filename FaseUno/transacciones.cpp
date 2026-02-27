#include <iostream>
#include <cstring>
#include <iomanip>
#include "structs.h"

using namespace std;

// Anchos para: ID, Tipo, Producto, Prov/Cli, Cant, Total, Fecha
static int anchosTr[] = {4, 10, 15, 10, 8, 10, 12};
static int colTr = 7;

void registrarCompra(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "Error: No hay productos registrados para comprar." << endl;
        return;
    }

    mostrarBanner("REGISTRAR COMPRA (A PROVEEDOR)");

    // 12. Redimensionar si es necesario
    if (tienda->numTransacciones == tienda->capacidadTransacciones) {
        // Aquí llamarías a tu función de redimensión de transacciones
    }

    Transaccion& t = tienda->transacciones[tienda->numTransacciones];

    // 1 y 2. Solicitar y verificar ID de producto
    int idProd;
    cout << "Ingrese ID del producto: ";
    cin >> idProd;
    int idxProd = obtenerIndice(tienda, idProd); // Función que ya tienes en productos.cpp

    if (idxProd == -1) {
        cout << "Error: El producto con ID " << idProd << " no existe." << endl;
        return;
    }

    // 3 y 4. Solicitar y verificar ID de proveedor
    int idProv;
    cout << "Ingrese ID del proveedor: ";
    cin >> idProv;
    if (!existeProveedor(tienda, idProv)) { // Función que ya tienes en proveedores.cpp
        cout << "Error: El proveedor con ID " << idProv << " no existe." << endl;
        return;
    }

    // 5 y 6. Cantidad y Precio unitario
    cout << "Cantidad a comprar: ";
    cin >> t.cantidad;
    if (t.cantidad <= 0) {
        cout << "Error: La cantidad debe ser mayor a 0." << endl;
        return;
    }

    cout << "Precio unitario de compra: ";
    cin >> t.precioUnitario;

    // 7. Calcular total automáticamente
    t.total = t.cantidad * t.precioUnitario;

    // 8. Mostrar resumen
    cout << "\n--- RESUMEN DE COMPRA ---" << endl;
    cout << "Producto: " << tienda->productos[idxProd].nombre << endl;
    cout << "Total a pagar: " << fixed << setprecision(2) << t.total << endl;

    // 9. Confirmar transacción
    char confirma;
    cout << "Confirmar registro de compra? (S/N): ";
    cin >> confirma;

    if (confirma == 's' || confirma == 'S') {
        // 10. INCREMENTAR el stock automáticamente
        tienda->productos[idxProd].stock += t.cantidad;

        // 11. Guardar transacción con tipo "COMPRA"
        t.id = tienda->siguienteIdTransaccion;
        t.idProducto = idProd;
        t.idRelacionado = idProv;
        strcpy(t.tipo, "COMPRA");
        obtenerFechaActual(t.fecha); // Tu función de utilidad para fechas

        tienda->numTransacciones++;
        tienda->siguienteIdTransaccion++;

        cout << "Compra registrada. Nuevo stock de " << tienda->productos[idxProd].nombre 
             << ": " << tienda->productos[idxProd].stock << endl;
    } else {
        cout << "Operacion cancelada." << endl;
    }
}

void imprimirFilaTransaccion(const Transaccion& t) {
    cout << setfill(' '); // Evita que el '=' del banner arruine la tabla
    cout << "| " << left << setw(anchosTr[0]) << t.id
         << "| " << setw(anchosTr[1]) << t.tipo
         << "| " << setw(anchosTr[2]) << t.idProducto
         << "| " << setw(anchosTr[3]) << t.idRelacionado
         << "| " << right << setw(anchosTr[4]) << t.cantidad
         << "| " << setw(anchosTr[5]) << fixed << setprecision(2) << t.total
         << "| " << left << setw(anchosTr[6]) << t.fecha << " |" << endl;
}

void registrarVenta(Tienda* tienda) {
    if (tienda->numProductos == 0) {
        cout << "Error: No hay productos en inventario para vender." << endl;
        return;
    }

    mostrarBanner("REGISTRAR VENTA (A CLIENTE)");

    // 14. Redimensionar array de transacciones si es necesario
    if (tienda->numTransacciones == tienda->capacidadTransacciones) {
        // Llamar a redimensionarTransacciones(tienda);
    }

    Transaccion& t = tienda->transacciones[tienda->numTransacciones];

    // 1 y 2. Solicitar ID de producto y verificar existencia
    int idProd;
    cout << "Ingrese ID del producto: ";
    cin >> idProd;
    int idxProd = obtenerIndice(tienda, idProd);

    if (idxProd == -1) {
        cout << "Error: Producto no encontrado." << endl;
        return;
    }

    // 3. Mostrar stock disponible
    cout << "Producto: " << tienda->productos[idxProd].nombre << endl;
    cout << "Stock actual: " << tienda->productos[idxProd].stock << endl;

    // 4 y 5. Solicitar ID de cliente y verificar existencia
    int idCli;
    cout << "Ingrese ID del cliente: ";
    cin >> idCli;
    // Asumiendo que tienes una función existeCliente similar a existeProveedor
    if (!existeCliente(tienda, idCli)) { 
        cout << "Error: El cliente con ID " << idCli << " no existe." << endl;
        return;
    }

    // 6 y 7. Solicitar cantidad y validar stock suficiente
    cout << "Cantidad a vender: ";
    cin >> t.cantidad;

    if (t.cantidad <= 0) {
        cout << "Error: La cantidad debe ser mayor a 0." << endl;
        return;
    }

    if (t.cantidad > tienda->productos[idxProd].stock) {
        cout << "Error: Stock insuficiente. Solo hay " << tienda->productos[idxProd].stock << " unidades." << endl;
        return;
    }

    // 8. Usar precio del producto (o permitir uno diferente)
    cout << "Precio de venta actual: " << tienda->productos[idxProd].precio << endl;
    cout << "Ingrese precio de venta final (o 0 para usar el actual): ";
    float precioEspecial;
    cin >> precioEspecial;
    
    t.precioUnitario = (precioEspecial > 0) ? precioEspecial : tienda->productos[idxProd].precio;

    // 9. Calcular total automáticamente
    t.total = t.cantidad * t.precioUnitario;

    // 10. Mostrar resumen
    cout << "\n--- RESUMEN DE VENTA ---" << endl;
    cout << "Cliente ID: " << idCli << endl;
    cout << "Total a cobrar: " << fixed << setprecision(2) << t.total << endl;

    // 11. Confirmar transacción
    char confirma;
    cout << "Confirmar venta? (S/N): ";
    cin >> confirma;

    if (confirma == 's' || confirma == 'S') {
        // 12. **DECREMENTAR** el stock automáticamente
        tienda->productos[idxProd].stock -= t.cantidad;

        // 13. Guardar transacción con tipo = "VENTA"
        t.id = tienda->siguienteIdTransaccion;
        t.idProducto = idProd;
        t.idRelacionado = idCli;
        strcpy(t.tipo, "VENTA");
        obtenerFechaActual(t.fecha);

        tienda->numTransacciones++;
        tienda->siguienteIdTransaccion++;

        cout << "Venta realizada con exito. Stock restante: " << tienda->productos[idxProd].stock << endl;
    } else {
        cout << "Venta cancelada." << endl;
    }
}

void buscarTransacciones(Tienda* tienda) {
    if (tienda->numTransacciones == 0) {
        cout << "\nNo hay transacciones registradas para realizar una busqueda." << endl;
        return;
    }

    int opcion;
    char criterio[100];
    
    // Menú basado exactamente en la imagen proporcionada
    cout << "\n--- Buscar Transacciones ---" << endl;
    cout << "1. Buscar por ID de transaccion" << endl;
    cout << "2. Buscar por ID de producto" << endl;
    cout << "3. Buscar por ID de cliente" << endl;
    cout << "4. Buscar por ID de proveedor" << endl;
    cout << "5. Buscar por fecha exacta (DD/MM/YYYY)" << endl; // Adaptado a tu formato de fecha
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

    for (int i = 0; i < tienda->numTransacciones; i++) {
        Transaccion& t = tienda->transacciones[i];
        bool coincide = false;

        switch (opcion) {
            case 1: // ID de transacción
                if (t.id == atoi(criterio)) coincide = true;
                break;
            case 2: // ID de producto
                if (t.idProducto == atoi(criterio)) coincide = true;
                break;
            case 3: // ID de cliente
                // Solo coincide si el tipo es VENTA y el ID relacionado coincide
                if (strcmp(t.tipo, "VENTA") == 0 && t.idRelacionado == atoi(criterio)) coincide = true;
                break;
            case 4: // ID de proveedor
                // Solo coincide si el tipo es COMPRA y el ID relacionado coincide
                if (strcmp(t.tipo, "COMPRA") == 0 && t.idRelacionado == atoi(criterio)) coincide = true;
                break;
            case 5: // Fecha exacta
                if (strcmp(t.fecha, criterio) == 0) coincide = true;
                break;
            case 6: // Tipo
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

    if (encontrado) {
        dibujarTabla(anchosTr, colTr);
    } else {
        cout << "\nNo se encontraron transacciones que coincidan con el criterio." << endl;
    }
}

void listarTransacciones(Tienda* tienda) {
    if (tienda->numTransacciones == 0) {
        cout << "\nNo hay transacciones registradas en el historial." << endl;
        return;
    }

    mostrarBanner("HISTORIAL DE TRANSACCIONES");
    
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

    // Recorremos el arreglo de transacciones guardadas
    for (int i = 0; i < tienda->numTransacciones; i++) {
        imprimirFilaTransaccion(tienda->transacciones[i]);
    }

    dibujarTabla(anchosTr, colTr);
    
    cout << " Total de movimientos registrados: " << tienda->numTransacciones << endl;
}

void cancelarTransaccion(Tienda* tienda) {
    if (tienda->numTransacciones == 0) {
        cout << "No hay transacciones para anular." << endl;
        return;
    }

    // 1. Buscar transaccion por ID
    int idBuscar;
    cout << "\n--- ANULAR TRANSACCION ---" << endl;
    cout << "Ingrese ID de la transaccion a anular: ";
    cin >> idBuscar;

    int idxTr = -1;
    for (int i = 0; i < tienda->numTransacciones; i++) {
        if (tienda->transacciones[i].id == idBuscar) {
            idxTr = i;
            break;
        }
    }

    if (idxTr == -1) {
        cout << "Error: No se encontro la transaccion con ID " << idBuscar << endl;
        return;
    }

    Transaccion& t = tienda->transacciones[idxTr];
    int idxProd = obtenerIndice(tienda, t.idProducto);

    //Mostrar detalles
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
        // 5. Ajustar stock automáticamente
        if (idxProd != -1) {
            if (strcmp(t.tipo, "COMPRA") == 0) {
                tienda->productos[idxProd].stock -= t.cantidad; // Revertir compra
            } else {
                tienda->productos[idxProd].stock += t.cantidad; // Revertir venta
            }
        }

        // 6. Marcar como cancelada o eliminarla
        // En este caso, la eliminaremos del arreglo moviendo los elementos
        for (int i = idxTr; i < tienda->numTransacciones - 1; i++) {
            tienda->transacciones[i] = tienda->transacciones[i + 1];
        }
        tienda->numTransacciones--;

        cout << "\nTransaccion anulada exitosamente. El inventario ha sido actualizado." << endl;
    } else {
        cout << "\nOperacion cancelada por el usuario." << endl;
    }
}

void menuTransacciones(Tienda* tienda) {
    int opcion;

    do {
        mostrarBanner("GESTION DE TRANSACCIONES");
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
                registrarCompra(tienda); break;
            case 2:
                registrarVenta(tienda); break; 
            case 3:
                listarTransacciones(tienda); break; 
            case 4:
                buscarTransacciones(tienda); break;
            case 5:
                cancelarTransaccion(tienda); break;
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

