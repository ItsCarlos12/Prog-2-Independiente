#include <iostream>
#include <cstring>
#include <cstdio>   // remove
#include "structs.h"
#include "files.h"

using namespace std;

// El seeder reinicia los .bin y los puebla con datos de ejemplo
// Requisitos mínimos: 5 proveedores, 15 productos, 8 clientes, 25 transacciones

void resetBinFiles() {
    // Eliminar archivos existentes para empezar desde cero
    const char* archivos[] = {
        "tienda.bin",
        "productos.bin",
        "proveedores.bin",
        "clientes.bin",
        "transacciones.bin"
    };

    for (const char* nombre : archivos) {
        remove(nombre); // Ignoramos el resultado; si no existe no pasa nada
    }

    // Volver a crear headers con inicializarArchivos()
    inicializarArchivos();
}

void seedProveedores() {
    const int numProv = 5;

    const char* nombres[numProv] = {
        "TecnoWorld",
        "ElectroPlus",
        "CompuZone",
        "GamerHub",
        "OfiSupply"
    };

    const char* rif[numProv] = {
        "J-100000001",
        "J-100000002",
        "J-100000003",
        "J-100000004",
        "J-100000005"
    };

    const char* emails[numProv] = {
        "contacto@tecnoworld.com",
        "ventas@electroplus.com",
        "info@compuzone.com",
        "soporte@gamerhub.com",
        "admin@ofisupply.com"
    };

    const char* telefonos[numProv] = {
        "0212-1111111",
        "0212-2222222",
        "0212-3333333",
        "0414-4444444",
        "0412-5555555"
    };

    const char* direcciones[numProv] = {
        "Av. Principal, Caracas",
        "C.C. Comercial Plaza, Caracas",
        "Zona Industrial Norte, Valencia",
        "C.C. Gamer Mall, Maracay",
        "Centro Empresarial Ofi, Barquisimeto"
    };

    for (int i = 0; i < numProv; ++i) {
        Proveedor p{};
        // id lo asigna guardarProveedor
        strcpy(p.telefono, telefonos[i]);
        strcpy(p.nombre, nombres[i]);
        strcpy(p.rif, rif[i]);
        strcpy(p.email, emails[i]);
        strcpy(p.direccion, direcciones[i]);
        p.cantidadProductos = 0;
        p.eliminado = false;
        guardarProveedor(p);
    }
}

void seedClientes() {
    const int numCli = 8;

    const char* nombres[numCli] = {
        "Carlos Perez",
        "Ana Gomez",
        "Luis Rodriguez",
        "Maria Torres",
        "Jose Martinez",
        "Laura Sanchez",
        "Pedro Ramirez",
        "Elena Diaz"
    };

    const char* cedulas[numCli] = {
        "V-10000001",
        "V-10000002",
        "V-10000003",
        "V-10000004",
        "V-10000005",
        "V-10000006",
        "V-10000007",
        "V-10000008"
    };

    const char* emails[numCli] = {
        "carlos@example.com",
        "ana@example.com",
        "luis@example.com",
        "maria@example.com",
        "jose@example.com",
        "laura@example.com",
        "pedro@example.com",
        "elena@example.com"
    };

    const char* telefonos[numCli] = {
        "0414-1010101",
        "0414-2020202",
        "0414-3030303",
        "0414-4040404",
        "0414-5050505",
        "0414-6060606",
        "0414-7070707",
        "0414-8080808"
    };

    const char* direcciones[numCli] = {
        "Caracas",
        "Valencia",
        "Maracay",
        "Barquisimeto",
        "Maracaibo",
        "Puerto La Cruz",
        "Merida",
        "San Cristobal"
    };

    for (int i = 0; i < numCli; ++i) {
        Cliente c{};
        // id lo asigna guardarCliente
        strcpy(c.nombre, nombres[i]);
        strcpy(c.cedula, cedulas[i]);
        strcpy(c.telefono, telefonos[i]);
        strcpy(c.email, emails[i]);
        strcpy(c.direccion, direcciones[i]);
        c.eliminado = false;
        guardarCliente(c);
    }
}

void seedProductos() {
    const int numProd = 15;

    const char* codigos[numProd] = {
        "P-001", "P-002", "P-003", "P-004", "P-005",
        "P-006", "P-007", "P-008", "P-009", "P-010",
        "P-011", "P-012", "P-013", "P-014", "P-015"
    };

    const char* nombres[numProd] = {
        "Mouse Optico",
        "Teclado Mecanico",
        "Monitor 24",
        "Laptop 14",
        "Impresora Laser",
        "Router WiFi",
        "Disco SSD 480GB",
        "Memoria RAM 8GB",
        "Tarjeta de Video",
        "Fuente 600W",
        "Case ATX",
        "Parlantes USB",
        "Webcam HD",
        "Microfono USB",
        "Silla Gamer"
    };

    const char* descripciones[numProd] = {
        "Mouse optico USB", "Teclado mecánico retroiluminado", "Monitor LED 24 pulgadas",
        "Laptop 14 pulgadas, 8GB RAM", "Impresora laser monocromatica", "Router WiFi doble banda",
        "SSD 480GB SATA", "Memoria RAM DDR4 8GB", "Tarjeta de video 4GB",
        "Fuente de poder 600W", "Case ATX mediano", "Parlantes USB estereo",
        "Webcam HD 720p", "Microfono USB condensador", "Silla gamer ergonómica"
    };

    // Asumimos que los proveedores creados tienen IDs 1..5
    for (int i = 0; i < numProd; ++i) {
        Producto p{};
        strcpy(p.codigo, codigos[i]);
        strcpy(p.nombre, nombres[i]);
        strcpy(p.descripcion, descripciones[i]);

        // Ciclar proveedores 1..5
        int proveedorId = (i % 5) + 1;
        p.idProveedor = proveedorId;

        p.precio = 10.0f + i * 5.0f;   // precios distintos
        p.stock = 100;                 // stock alto para soportar las ventas de prueba
        p.stockMinimo = 10;
        p.totalVendido = 0;
        p.eliminado = false;

        guardarProducto(p);
    }
}

void seedTransaccionesVentas() {
    // Suponemos que ya existen 15 productos (IDs 1..15) y 8 clientes (IDs 1..8)
    const int numVentas = 25;

    for (int i = 0; i < numVentas; ++i) {
        int idProducto = (i % 15) + 1;   // 1..15
        int idCliente  = (i % 8) + 1;    // 1..8
        int cantidad   = (i % 5) + 1;    // 1..5 unidades

        realizarVenta(idCliente, idProducto, cantidad);
    }
}

int main() {
    cout << "=== Seeder de datos para archivos .bin ===" << endl;

    resetBinFiles();

    cout << "Insertando proveedores..." << endl;
    seedProveedores();

    cout << "Insertando clientes..." << endl;
    seedClientes();

    cout << "Insertando productos..." << endl;
    seedProductos();

    cout << "Generando transacciones de venta..." << endl;
    seedTransaccionesVentas();

    cout << "\nSeeder completado. Archivos .bin poblados." << endl;
    return 0;
}
