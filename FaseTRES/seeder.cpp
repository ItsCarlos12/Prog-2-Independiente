#include "clientes/Cliente.h"
#include "persistencia/Constantes.h"
#include "persistencia/GestorArchivos.h"
#include "productos/Producto.h"
#include "proveedores/Proveedor.h"
#include "transacciones/Transaccion.h"

#include <cstring>
#include <iostream>

void seedProveedores() {
    const char* nombres[5] = {"TecnoWorld", "ElectroPlus", "CompuZone", "GamerHub", "OfiSupply"};
    const char* rifs[5] = {"J-100000001", "J-100000002", "J-100000003", "J-100000004",
                           "J-100000005"};
    const char* emails[5] = {"contacto@tecnoworld.com", "ventas@electroplus.com",
                             "info@compuzone.com", "soporte@gamerhub.com",
                             "admin@ofisupply.com"};
    const char* telefonos[5] = {"0212-1111111", "0212-2222222", "0212-3333333", "0414-4444444",
                                "0412-5555555"};

    for (int i = 0; i < 5; ++i) {
        Proveedor p;
        p.setNombre(nombres[i]);
        p.setRif(rifs[i]);
        p.setEmail(emails[i]);
        p.setTelefono(telefonos[i]);
        p.setDireccion("Venezuela");
        GestorArchivos::guardarRegistro(Constantes::RUTA_PROVEEDORES, p);
    }
}

void seedClientes() {
    const char* nombres[8] = {"Carlos Perez", "Ana Gomez", "Luis Rodriguez", "Maria Torres",
                              "Jose Martinez", "Laura Sanchez", "Pedro Ramirez", "Elena Diaz"};
    const char* cedulas[8] = {"V-10000001", "V-10000002", "V-10000003", "V-10000004",
                              "V-10000005", "V-10000006", "V-10000007", "V-10000008"};
    const char* emails[8] = {"carlos@example.com", "ana@example.com", "luis@example.com",
                             "maria@example.com", "jose@example.com", "laura@example.com",
                             "pedro@example.com", "elena@example.com"};

    for (int i = 0; i < 8; ++i) {
        Cliente c;
        c.setNombre(nombres[i]);
        c.setCedula(cedulas[i]);
        c.setEmail(emails[i]);
        c.setTelefono("0414-0000000");
        c.setDireccion("Venezuela");
        GestorArchivos::guardarRegistro(Constantes::RUTA_CLIENTES, c);
    }
}

void seedProductos() {
    const char* codigos[15] = {"P-001", "P-002", "P-003", "P-004", "P-005", "P-006", "P-007",
                               "P-008", "P-009", "P-010", "P-011", "P-012", "P-013", "P-014",
                               "P-015"};

    const char* nombres[15] = {"Mouse Optico",     "Teclado Mecanico", "Monitor 24",    "Laptop 14",
                               "Impresora Laser",  "Router WiFi",       "Disco SSD 480GB", "Memoria RAM 8GB",
                               "Tarjeta de Video", "Fuente 600W",       "Case ATX",      "Parlantes USB",
                               "Webcam HD",        "Microfono USB",     "Silla Gamer"};

    for (int i = 0; i < 15; ++i) {
        Producto p;
        p.setCodigo(codigos[i]);
        p.setNombre(nombres[i]);
        p.setDescripcion("Producto de demostracion");
        p.setIdProveedor((i % 5) + 1);
        p.setPrecio(10.0f + static_cast<float>(i) * 5.0f);
        p.setStock(100);
        p.setStockMinimo(10);
        p.setTotalVendido(0);
        GestorArchivos::guardarRegistro(Constantes::RUTA_PRODUCTOS, p);

        Proveedor prov;
        int indiceProv = GestorArchivos::obtenerIndicePorId<Proveedor>(Constantes::RUTA_PROVEEDORES,
                                                                        p.getIdProveedor());
        if (indiceProv >= 0 &&
            GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PROVEEDORES, p.getIdProveedor(),
                                                 prov) &&
            !prov.isEliminado()) {
            prov.agregarProductoID(p.getId());
            GestorArchivos::actualizarRegistro(Constantes::RUTA_PROVEEDORES, indiceProv, prov);
        }
    }
}

void seedTransacciones() {
    for (int i = 0; i < 25; ++i) {
        const int idProducto = (i % 15) + 1;
        const int idCliente = (i % 8) + 1;
        int cantidad = (i % 5) + 1;

        Producto producto;
        int indiceProd = GestorArchivos::obtenerIndicePorId<Producto>(Constantes::RUTA_PRODUCTOS, idProducto);
        if (indiceProd < 0 ||
            !GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_PRODUCTOS, idProducto, producto) ||
            producto.isEliminado()) {
            continue;
        }

        if (producto.getStock() < cantidad) {
            cantidad = 1;
        }
        if (producto.getStock() < cantidad) {
            continue;
        }

        Transaccion t;
        t.setTipo("Venta");
        t.setIdRelacionado(idCliente);
        t.setDescripcion("Venta de prueba generada por seeder.");
        t.agregarDetalle(idProducto, cantidad, producto.getPrecio());
        if (!GestorArchivos::guardarRegistro(Constantes::RUTA_TRANSACCIONES, t)) {
            continue;
        }

        producto.reducirStock(cantidad);
        producto.agregarTransaccionID(t.getId());
        GestorArchivos::actualizarRegistro(Constantes::RUTA_PRODUCTOS, indiceProd, producto);

        Cliente cliente;
        int indiceCli = GestorArchivos::obtenerIndicePorId<Cliente>(Constantes::RUTA_CLIENTES, idCliente);
        if (indiceCli >= 0 &&
            GestorArchivos::obtenerRegistroPorId(Constantes::RUTA_CLIENTES, idCliente, cliente) &&
            !cliente.isEliminado()) {
            cliente.agregarTransaccionID(t.getId());
            GestorArchivos::actualizarRegistro(Constantes::RUTA_CLIENTES, indiceCli, cliente);
        }
    }
}

int main() {
    std::cout << "=== Seeder FaseTRES ===\n";

    if (!GestorArchivos::reiniciarSistemaArchivos()) {
        std::cout << "No se pudo reiniciar la base binaria.\n";
        return 1;
    }

    seedProveedores();
    seedClientes();
    seedProductos();
    seedTransacciones();

    std::cout << "Datos generados en carpeta datos/.\n";
    return 0;
}
