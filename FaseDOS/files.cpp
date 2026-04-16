#include "files.h"
#include <iostream>

using namespace std;

const char* TIENDA_FILE = "tienda.bin";
const char* PRODUCTOS_FILE = "productos.bin";
const char* PROVEEDORES_FILE = "proveedores.bin";
const char* CLIENTES_FILE = "clientes.bin";
const char* TRANSACCIONES_FILE = "transacciones.bin";

// --- Gestión de Headers ---
ArchivoHeader leerHeader(const char* nombreArchivo) {
    ifstream file(nombreArchivo, ios::binary);
    ArchivoHeader header;
    if (file.is_open()) {
        file.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    }
    file.close();
    return header;
}

void actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    fstream file(nombreArchivo, ios::binary | ios::in | ios::out);
    if (file.is_open()) {
        file.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    }
    file.close();
}

bool inicializarArchivo(const char* nombreArchivo) {
    ofstream file(nombreArchivo, ios::binary | ios::app);
    if (!file.is_open()) return false;

    file.seekp(0, ios::end);
    if (file.tellp() == 0) {
        ArchivoHeader header = {0, 1, 0, 1};
        file.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    }
    file.close();
    return true;
}

void inicializarArchivos() {
    inicializarArchivo(TIENDA_FILE);
    inicializarArchivo(PRODUCTOS_FILE);
    inicializarArchivo(PROVEEDORES_FILE);
    inicializarArchivo(CLIENTES_FILE);
    inicializarArchivo(TRANSACCIONES_FILE);
}

// --- Búsqueda de Índices ---
int obtenerIndiceGenerico(const char* nombreArchivo, int id, size_t structSize) {
    ifstream file(nombreArchivo, ios::binary);
    if (!file.is_open()) return -1;

    ArchivoHeader header = leerHeader(nombreArchivo);
    file.seekg(sizeof(ArchivoHeader));

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        char buffer[structSize];
        file.read(buffer, structSize);
        int currentId = *reinterpret_cast<int*>(buffer); // Asume que el ID es el primer miembro
        if (currentId == id) {
            file.close();
            return i;
        }
    }

    file.close();
    return -1;
}

int obtenerIndiceProducto(int id) {
    return obtenerIndiceGenerico(PRODUCTOS_FILE, id, sizeof(Producto));
}

int obtenerIndiceProveedor(int id) {
    return obtenerIndiceGenerico(PROVEEDORES_FILE, id, sizeof(Proveedor));
}

int obtenerIndiceCliente(int id) {
    return obtenerIndiceGenerico(CLIENTES_FILE, id, sizeof(Cliente));
}


// --- Implementaciones CRUD ---

// Productos
void guardarProducto(Producto& producto) {
    ArchivoHeader header = leerHeader(PRODUCTOS_FILE);
    
    producto.id = header.proximoID;
    producto.eliminado = false;
    producto.fechaCreacion = time(nullptr);
    producto.fechaUltimaModificacion = time(nullptr);

    ofstream file(PRODUCTOS_FILE, ios::binary | ios::app);
    file.write(reinterpret_cast<const char*>(&producto), sizeof(Producto));
    file.close();

    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    actualizarHeader(PRODUCTOS_FILE, header);
}

Producto obtenerProducto(int id) {
    int indice = obtenerIndiceProducto(id);
    if (indice == -1) return {}; 

    ifstream file(PRODUCTOS_FILE, ios::binary);
    Producto producto;
    file.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Producto)));
    file.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
    file.close();
    return producto;
}

void actualizarProducto(const Producto& producto) {
    int indice = obtenerIndiceProducto(producto.id);
    if (indice == -1) return;

    Producto p = producto;
    p.fechaUltimaModificacion = time(nullptr);

    fstream file(PRODUCTOS_FILE, ios::binary | ios::in | ios::out);
    file.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Producto)));
    file.write(reinterpret_cast<const char*>(&p), sizeof(Producto));
    file.close();
}

void eliminarProducto(int id) {
    Producto producto = obtenerProducto(id);
    if (producto.id == 0 || producto.eliminado) return;

    producto.eliminado = true;
    actualizarProducto(producto);

    ArchivoHeader header = leerHeader(PRODUCTOS_FILE);
    header.registrosActivos--;
    actualizarHeader(PRODUCTOS_FILE, header);
}

int contarProductos() {
    return leerHeader(PRODUCTOS_FILE).registrosActivos;
}

// Proveedores
void guardarProveedor(Proveedor& proveedor) {
    ArchivoHeader header = leerHeader(PROVEEDORES_FILE);
    
    proveedor.id = header.proximoID;
    proveedor.eliminado = false;
    proveedor.fechaCreacion = time(nullptr);
    proveedor.fechaUltimaModificacion = time(nullptr);
    proveedor.cantidadProductos = 0;

    ofstream file(PROVEEDORES_FILE, ios::binary | ios::app);
    file.write(reinterpret_cast<const char*>(&proveedor), sizeof(Proveedor));
    file.close();

    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    actualizarHeader(PROVEEDORES_FILE, header);
}

Proveedor obtenerProveedor(int id) {
    int indice = obtenerIndiceProveedor(id);
    if (indice == -1) return {};

    ifstream file(PROVEEDORES_FILE, ios::binary);
    Proveedor proveedor;
    file.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Proveedor)));
    file.read(reinterpret_cast<char*>(&proveedor), sizeof(Proveedor));
    file.close();
    return proveedor;
}

void actualizarProveedor(const Proveedor& proveedor) {
    int indice = obtenerIndiceProveedor(proveedor.id);
    if (indice == -1) return;

    Proveedor p = proveedor;
    p.fechaUltimaModificacion = time(nullptr);

    fstream file(PROVEEDORES_FILE, ios::binary | ios::in | ios::out);
    file.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Proveedor)));
    file.write(reinterpret_cast<const char*>(&p), sizeof(Proveedor));
    file.close();
}

void eliminarProveedor(int id) {
    Proveedor proveedor = obtenerProveedor(id);
    if (proveedor.id == 0 || proveedor.eliminado) return;

    proveedor.eliminado = true;
    actualizarProveedor(proveedor);

    ArchivoHeader header = leerHeader(PROVEEDORES_FILE);
    header.registrosActivos--;
    actualizarHeader(PROVEEDORES_FILE, header);
}

int contarProveedores() {
    return leerHeader(PROVEEDORES_FILE).registrosActivos;
}

// Clientes
void guardarCliente(Cliente& cliente) {
    ArchivoHeader header = leerHeader(CLIENTES_FILE);
    
    cliente.id = header.proximoID;
    cliente.eliminado = false;
    cliente.fechaCreacion = time(nullptr);
    cliente.fechaUltimaModificacion = time(nullptr);

    ofstream file(CLIENTES_FILE, ios::binary | ios::app);
    file.write(reinterpret_cast<const char*>(&cliente), sizeof(Cliente));
    file.close();

    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    actualizarHeader(CLIENTES_FILE, header);
}

Cliente obtenerCliente(int id) {
    int indice = obtenerIndiceCliente(id);
    if (indice == -1) return {};

    ifstream file(CLIENTES_FILE, ios::binary);
    Cliente cliente;
    file.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Cliente)));
    file.read(reinterpret_cast<char*>(&cliente), sizeof(Cliente));
    file.close();
    return cliente;
}

void actualizarCliente(const Cliente& cliente) {
    int indice = obtenerIndiceCliente(cliente.id);
    if (indice == -1) return;

    Cliente c = cliente;
    c.fechaUltimaModificacion = time(nullptr);

    fstream file(CLIENTES_FILE, ios::binary | ios::in | ios::out);
    file.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Cliente)));
    file.write(reinterpret_cast<const char*>(&c), sizeof(Cliente));
    file.close();
}

void eliminarCliente(int id) {
    Cliente cliente = obtenerCliente(id);
    if (cliente.id == 0 || cliente.eliminado) return;

    cliente.eliminado = true;
    actualizarCliente(cliente);

    ArchivoHeader header = leerHeader(CLIENTES_FILE);
    header.registrosActivos--;
    actualizarHeader(CLIENTES_FILE, header);
}

int contarClientes() {
    return leerHeader(CLIENTES_FILE).registrosActivos;
}

// Transacciones
void guardarTransaccion(Transaccion& transaccion) {
    ArchivoHeader header = leerHeader(TRANSACCIONES_FILE);
    
    transaccion.id = header.proximoID;
    transaccion.eliminado = false;
    transaccion.fechaCreacion = time(nullptr);

    ofstream file(TRANSACCIONES_FILE, ios::binary | ios::app);
    file.write(reinterpret_cast<const char*>(&transaccion), sizeof(Transaccion));
    file.close();

    header.cantidadRegistros++;
    header.proximoID++;
    header.registrosActivos++;
    actualizarHeader(TRANSACCIONES_FILE, header);
}

Transaccion obtenerTransaccion(int id) {
    // La búsqueda por índice para transacciones puede ser más compleja si se permite eliminar
    // Por ahora, asumimos que no se eliminan o se buscan de otra manera
    ifstream file(TRANSACCIONES_FILE, ios::binary);
    Transaccion transaccion;
    // Esto requeriría una función de índice similar a las otras entidades
    // int indice = obtenerIndiceTransaccion(id);
    // file.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Transaccion)));
    file.read(reinterpret_cast<char*>(&transaccion), sizeof(Transaccion));
    file.close();
    return transaccion;
}

int contarTransacciones() {
    return leerHeader(TRANSACCIONES_FILE).registrosActivos;
}

bool realizarVenta(int idCliente, int idProducto, int cantidad) {
    // 1. Buscar y leer el Producto y el Cliente
    Producto producto = obtenerProducto(idProducto);
    Cliente cliente = obtenerCliente(idCliente);

    // 2. Validar reglas de negocio
    if (cliente.id == 0 || cliente.eliminado) {
        cout << "Error: El cliente no existe o ha sido eliminado." << endl;
        return false;
    }

    if (producto.id == 0 || producto.eliminado) {
        cout << "Error: El producto no existe o ha sido eliminado." << endl;
        return false;
    }

    if (producto.stock < cantidad) {
        cout << "Error: Stock insuficiente. Stock disponible: " << producto.stock << endl;
        return false;
    }

    // 3. Si es válido, instanciar un registro Transaccion
    Transaccion nuevaVenta;
    strcpy(nuevaVenta.tipo, "Venta");
    nuevaVenta.idRelacionado = idCliente; // ID del Cliente
    
    // Llenar el primer y único detalle de la transacción
    nuevaVenta.detalles[0].idProducto = idProducto;
    nuevaVenta.detalles[0].cantidad = cantidad;
    nuevaVenta.detalles[0].precioUnitario = producto.precio;
    nuevaVenta.cantidadDetalles = 1;

    nuevaVenta.total = producto.precio * cantidad;
    strcpy(nuevaVenta.descripcion, "Venta de producto al cliente.");

    // 4. Guardar la Transaccion
    guardarTransaccion(nuevaVenta);
    // 'guardarTransaccion' se encarga de asignarle el ID y la fecha internamente.

    // 5. Modificar el Producto y actualizarlo en el archivo
    producto.stock -= cantidad;
    producto.totalVendido += cantidad;
    actualizarProducto(producto); // Esta función actualiza el registro en productos.bin

    // 6. Modificar el Cliente y actualizarlo en el archivo
    // (Actualmente no hay campos de estadísticas en Cliente para actualizar)

    cout << "Venta registrada exitosamente." << endl;
    return true;
}