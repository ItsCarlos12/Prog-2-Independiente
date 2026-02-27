#ifndef STRUCTS_H
#define STRUCTS_H

struct Producto{
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    int idProveedor;
    float precio;
    int stock;
    char fechaRegistro[11];
};

struct Proveedor{
    int id;
    char telefono[20];
    char nombre[100];
    char rif[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
};

struct Cliente{
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
};

struct Transaccion{
    int id;
    char tipo[10];
    int idProducto;
    int idRelacionado;

    int cantidad;
    float precioUnitario;

    float total;
    char fecha[11];
    char descripcion[200];
};

struct Tienda{
    char nombre[100];
    char rif[20];
    
    //Arrays dinamicos de entidades.

    Producto* productos;
    int numProductos;
    int capacidadProductos;

    Proveedor* proveedores;
    int numProveedores;
    int capacidadProveedores;

    Cliente* clientes;
    int numClientes;
    int capacidadClientes;

    Transaccion* transacciones;
    int numTransacciones;
    int capacidadTransacciones;

    //Contadores para IDs autoincrementales.

    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

//Prototipos
bool inventarioVacio(Tienda* tienda);
void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif);
void liberarTienda(Tienda* tienda);

//Prototipos - Productos
void menuProductos(Tienda* tienda);
void mostrarTabla();
void imprimirFilaProducto(const Producto& p);

//Prototipos - Proveedores
void menuProveedores(Tienda* tienda);
bool noProveedores(Tienda* tienda);
void imprimirFilaProveedor(const Proveedor& prov);
void mostrarProveedores();
int obtenerIndiceProv(Tienda* tienda, int idBuscar);

//Prototipos - Utilidades
void dibujarTabla(int anchos[], int columnas);
bool compararLetras(const char* original, const char* busqueda);
void mostrarBanner(const char* titulo);


#endif 