# Sistema de Gestión de Tienda – Versión Binaria (V2)

## 1. Descripción General

Este proyecto es una aplicación de consola en C++ para la gestión básica de una tienda:

- Gestión de productos (inventario, stock mínimo, proveedor asociado).
- Gestión de proveedores y clientes.
- Registro de transacciones de venta (y en V1 también compras/anulaciones).
- Persistencia en archivos binarios `.bin` con headers y registros de tamaño fijo.

Esta versión (V2) migra desde archivos `.dat` planos a una arquitectura binaria con `ArchivoHeader` por archivo y structs sin punteros ni `std::string`, aptos para `fread`/`fwrite`/`read`/`write` directos.

---

## 2. Estructuras y tamaños en bytes

Los tamaños indicados son los típicos en un entorno de 64 bits con `int` de 4 bytes, `float` de 4 bytes y `time_t` de 8 bytes. Se recomienda verificar con el programa (`sizeof`) en el entorno de compilación efectivo.

> Nota: La memoria puede incluir padding por alineación; los tamaños reales deben consultarse con `sizeof` en tiempo de compilación/ejecución.

### 2.1. ArchivoHeader

```cpp
struct ArchivoHeader {
    int cantidadRegistros;  // 4 bytes
    int proximoID;          // 4 bytes
    int registrosActivos;   // 4 bytes
    int version;            // 4 bytes
};
```

Tamaño lógico sin considerar padding: 4 * 4 = **16 bytes**.

En disco se escribe de una sola vez al inicio de cada `.bin`:

- Offset 0      → inicio del header.
- Offset 0–15   → bytes del `ArchivoHeader`.
- Offset 16…    → primer registro de datos.

### 2.2. Producto

Campos principales (sin padding):

- `int id` → 4 bytes
- `char codigo[20]` → 20 bytes
- `char nombre[100]` → 100 bytes
- `char descripcion[200]` → 200 bytes
- `float precio` → 4 bytes
- `int stock` → 4 bytes
- `int idProveedor` → 4 bytes
- `int stockMinimo` → 4 bytes
- `int totalVendido` → 4 bytes
- `bool eliminado` → 1 byte (se suele alinear)
- `time_t fechaCreacion` → 8 bytes
- `time_t fechaUltimaModificacion` → 8 bytes

Suma lógica (sin alinear):

- 4 + 20 + 100 + 200 + 4 + 4 + 4 + 4 + 4 + 1 + 8 + 8 = **361 bytes**.

Por alineación, `sizeof(Producto)` será un múltiplo de 4 u 8. El valor real se puede consultar con `sizeof(Producto)`.

### 2.3. Proveedor

Campos:

- `int id` → 4
- `char telefono[20]` → 20
- `char nombre[100]` → 100
- `char rif[20]` → 20
- `char email[100]` → 100
- `char direccion[200]` → 200
- `int productosIDs[100]` → 100 * 4 = 400
- `int cantidadProductos` → 4
- `bool eliminado` → 1
- `time_t fechaCreacion` → 8
- `time_t fechaUltimaModificacion` → 8

Suma lógica: 4 + 20 + 100 + 20 + 100 + 200 + 400 + 4 + 1 + 8 + 8 = **865 bytes** (antes de padding).

### 2.4. Cliente

Campos:

- `int id` → 4
- `char nombre[100]` → 100
- `char cedula[20]` → 20
- `char telefono[20]` → 20
- `char email[100]` → 100
- `char direccion[200]` → 200
- `bool eliminado` → 1
- `time_t fechaCreacion` → 8
- `time_t fechaUltimaModificacion` → 8

Suma lógica: 4 + 100 + 20 + 20 + 100 + 200 + 1 + 8 + 8 = **461 bytes** (antes de padding).

### 2.5. DetalleTransaccion

- `int idProducto` → 4
- `int cantidad` → 4
- `float precioUnitario` → 4

Suma lógica: 4 + 4 + 4 = **12 bytes**.

### 2.6. Transaccion

Campos:

- `int id` → 4
- `char tipo[10]` → 10
- `int idRelacionado` → 4
- `DetalleTransaccion detalles[50]` → 50 * 12 = 600
- `int cantidadDetalles` → 4
- `float total` → 4
- `char descripcion[200]` → 200
- `bool eliminado` → 1
- `time_t fechaCreacion` → 8

Suma lógica: 4 + 10 + 4 + 600 + 4 + 4 + 200 + 1 + 8 = **835 bytes** (antes de padding).

### 2.7. Tienda

- `char nombre[100]` → 100
- `char rif[20]` → 20

Suma lógica: 120 bytes.

---

## 3. Cálculo de offsets en los archivos binarios

Cada archivo `.bin` sigue la misma estructura general:

```text
[Header ArchivoHeader] [Registro 0] [Registro 1] ... [Registro N-1]
```

Si `H = sizeof(ArchivoHeader)` y `S = sizeof(StructEntidad)` (por ejemplo `sizeof(Producto)`), entonces:

- Offset en bytes del registro en posición `i` (índice 0-based):

  $$
  offset(i) = H + i \times S
  $$

- Para leer un registro en el índice `i`:

  - `seekg(H + i * S)` y luego `read` de `S` bytes.

- Para escribir/actualizar un registro en el índice `i`:

  - `seekp(H + i * S)` y luego `write` de `S` bytes.

### 3.1. Uso en `files.cpp`

Ejemplo simplificado (productos):

```cpp
file.seekg(sizeof(ArchivoHeader) + (indice * sizeof(Producto)));
file.read(reinterpret_cast<char*>(&producto), sizeof(Producto));
```

El cálculo lógico es:

- `sizeof(ArchivoHeader)` → salta el bloque de metadata inicial.
- `indice * sizeof(Producto)` → salta `indice` registros completos hasta la posición deseada.

Para actualizar:

```cpp
file.seekp(sizeof(ArchivoHeader) + (indice * sizeof(Producto)));
file.write(reinterpret_cast<const char*>(&p), sizeof(Producto));
```

La misma fórmula se aplica a `Proveedor`, `Cliente` y `Transaccion`, cambiando únicamente el tipo de struct.

---

## 4. Archivos `.bin` y datos de prueba

Los archivos gestionados en esta versión son:

- `tienda.bin`
- `productos.bin`
- `proveedores.bin`
- `clientes.bin`
- `transacciones.bin`

Cada uno comienza con un `ArchivoHeader` inicializado con:

- `cantidadRegistros = 0`
- `proximoID = 1`
- `registrosActivos = 0`
- `version = 1`

### 4.1. Población de datos de prueba (recomendado)

Para cumplir el requerimiento de al menos:

- 15 productos
- 5 proveedores
- 8 clientes
- 25 transacciones

se recomienda crear un pequeño programa “semilla” (por ejemplo `seeder.cpp`) que:

1. Llame a `inicializarArchivos()` de `files.h`.
2. Inserte proveedores con `guardarProveedor`.
3. Inserte productos asociados a esos proveedores con `guardarProducto`.
4. Inserte clientes con `guardarCliente`.
5. Genere transacciones de venta con `realizarVenta(...)` o armando `Transaccion` + `guardarTransaccion` manualmente.

De esta forma:

- Los IDs se asignan correctamente por el `ArchivoHeader`.
- Se respetan las relaciones (producto → proveedor, transaccion → cliente/proveedor y productos).

> Nota: Como el entorno de evaluación puede variar, los `.bin` deben generarse ejecutando el programa semilla antes de empaquetar la entrega final.

---

## 5. Manual de usuario sencillo

### 5.1. Compilación

En la carpeta `FaseUno`:

```bash
g++ *.cpp -o tienda.exe
```

(Requiere un compilador compatible con C++11 o superior en Windows.)

### 5.2. Ejecución

```bash
./tienda.exe
```

Al iniciar, el sistema:

1. Inicializa los archivos `.bin` (si no existen, crea los headers).
2. Muestra el menú principal de gestión de la tienda.

### 5.3. Menú principal

Opciones principales:

1. **Gestión de Productos**
   - Registrar nuevo producto: solicita código, nombre, descripción, ID de proveedor, precio, stock inicial y stock mínimo.
   - Listar inventario completo: muestra tabla con ID, código, nombre, **nombre del proveedor**, precio, stock y fecha de creación.

2. **Gestión de Proveedores**
   - Registrar proveedor: RIF, nombre, email, dirección y teléfono.
   - Listar proveedores: muestra tabla con ID, RIF, nombre, email y teléfono.
   - Buscar proveedor: por ID, nombre o RIF.

3. **Gestión de Clientes**
   - Registrar cliente: cédula, nombre, email, dirección y teléfono.
   - Listar clientes: tabla con ID, cédula, nombre, teléfono y email.
   - Buscar cliente: por ID, cédula o nombre.
   - Actualizar / eliminar cliente.

4. **Gestión de Transacciones**
   - Registrar venta: selecciona producto, cliente y cantidad. Valida stock y registra la transacción en `transacciones.bin`, actualizando el stock y `totalVendido` del producto.

(Opciones de compras/anulaciones dependen de la versión exacta de `transacciones.cpp` utilizada.)

### 5.4. Colores y tablas

- Los mensajes de éxito, error y advertencia utilizan color en consola (Windows) mediante `ui.h` y `ui.cpp`.
- Las tablas usan `setw` y `setfill` para alinear columnas y mejorar la legibilidad.

---

## 6. Notas finales

- Los tamaños en bytes indicados son aproximados y sirven para entender la lógica de offset; siempre deben validarse con `sizeof` en el entorno real.
- La combinación de `ArchivoHeader` + registros de tamaño fijo permite:
  - Búsqueda directa por ID mediante aritmética de bytes.
  - Borrado lógico (`eliminado`) sin reescribir el archivo completo.
  - Estadísticas rápidas (`cantidadRegistros`, `registrosActivos`).

Para más detalles sobre el código, revisar especialmente:

- [FaseUno/structs.h](FaseUno/structs.h)
- [FaseUno/files.h](FaseUno/files.h)
- [FaseUno/files.cpp](FaseUno/files.cpp)
- [FaseUno/productos.cpp](FaseUno/productos.cpp)
