# Manual de Usuario - Sistema de Gestion de Inventario (Fase 3)

## 1. Objetivo del manual
Este manual explica como usar el sistema una vez que ya esta abierto en pantalla.
No incluye instalacion, compilacion ni ejecucion tecnica.

## 2. Que puede hacer el usuario
El sistema permite:

- Gestionar productos
- Gestionar proveedores
- Gestionar clientes
- Registrar compras y ventas
- Consultar reportes de mantenimiento

## 3. Navegacion general
Al iniciar, se muestra el menu principal con estas opciones:

- [1] Gestion de Productos
- [2] Gestion de Proveedores
- [3] Gestion de Clientes
- [4] Gestion de Transacciones
- [5] Mantenimiento y Reportes
- [0] Salir

Reglas de navegacion:

- Siempre ingrese el numero de opcion que aparece entre corchetes.
- Si se ingresa un valor fuera de rango, el sistema pedira repetir la entrada.
- La opcion [0] vuelve al menu anterior o cierra el sistema (si esta en el menu principal).
- Despues de muchas operaciones aparece: "Presione ENTER para continuar...".

## 4. Orden recomendado para primer uso
Para evitar errores por datos faltantes, use este orden:

1. Registrar proveedores
2. Registrar productos (cada producto necesita proveedor)
3. Registrar clientes
4. Registrar compras y ventas
5. Consultar reportes

## 5. Gestion de Proveedores
Menu Proveedores:

- [1] Registrar proveedor
- [2] Buscar proveedor
- [3] Actualizar proveedor
- [4] Listar proveedores
- [5] Eliminar proveedor
- [0] Volver

### 5.1 Registrar proveedor
Datos solicitados:

- RIF
- Nombre
- Email
- Telefono
- Direccion

Validaciones principales:

- RIF: debe iniciar con J, V, E, G o P; luego guion y numeros. Ejemplo: J-12345678
- RIF no puede repetirse
- Email debe contener @ y un punto luego del @
- Telefono debe tener al menos 7 digitos (puede incluir -, espacios, parentesis)

Resultado esperado:

- Mensaje: "Proveedor registrado con ID: X"

### 5.2 Buscar proveedor
Criterios disponibles:

- Por ID
- Por Nombre
- Por RIF

### 5.3 Actualizar proveedor
Se pide el ID y luego se puede modificar:

- Nombre
- RIF
- Email
- Direccion
- Telefono

Guardar cambios con opcion [0].

### 5.4 Eliminar proveedor
Restriccion importante:

- No se puede eliminar si tiene productos activos asociados.

## 6. Gestion de Productos
Menu Productos:

- [1] Registrar producto
- [2] Buscar producto
- [3] Actualizar producto
- [4] Listar productos
- [5] Eliminar producto
- [6] Reporte de stock critico
- [0] Volver

### 6.1 Registrar producto
Precondicion:

- Debe existir al menos un proveedor activo.

Datos solicitados:

- Codigo
- Nombre
- Descripcion
- ID del proveedor
- Precio
- Stock inicial
- Stock minimo

Validaciones principales:

- Codigo obligatorio y no duplicado
- Nombre obligatorio
- ID proveedor debe existir
- Precio >= 0
- Stock >= 0
- Stock minimo >= 0

Resultado esperado:

- Mensaje: "Producto registrado con ID: X"

### 6.2 Buscar producto
Criterios disponibles:

- Por ID
- Por Codigo
- Por Nombre

### 6.3 Actualizar producto
Campos editables:

- Codigo
- Nombre
- Descripcion
- Precio
- Stock
- Stock minimo
- Proveedor

Guardar cambios con opcion [0].

### 6.4 Eliminar producto
Pide confirmacion con S/N antes de borrar logicamente.

### 6.5 Reporte de stock critico
Muestra productos cuyo stock actual es menor o igual al stock minimo.

## 7. Gestion de Clientes
Menu Clientes:

- [1] Registrar cliente
- [2] Buscar cliente
- [3] Actualizar cliente
- [4] Listar clientes
- [5] Eliminar cliente
- [0] Volver

### 7.1 Registrar cliente
Datos solicitados:

- Cedula
- Nombre
- Email
- Telefono
- Direccion

Validaciones principales:

- Cedula obligatoria y no duplicada
- Nombre obligatorio
- Email valido (con @ y punto)
- Telefono valido (minimo 7 digitos)

Resultado esperado:

- Mensaje: "Cliente registrado con ID: X"

### 7.2 Buscar cliente
Criterios disponibles:

- Por ID
- Por Cedula
- Por Nombre

### 7.3 Actualizar cliente
Campos editables:

- Nombre
- Cedula
- Email
- Direccion
- Telefono

Guardar cambios con opcion [0].

### 7.4 Eliminar cliente
Pide confirmacion con S/N antes de borrar logicamente.

## 8. Gestion de Transacciones
Menu Transacciones:

- [1] Registrar compra
- [2] Registrar venta
- [3] Listar transacciones
- [4] Buscar transaccion
- [5] Cancelar transaccion
- [0] Volver

### 8.1 Registrar compra
Precondicion:

- Debe existir al menos un producto activo y un proveedor activo.

Datos solicitados:

- ID producto
- ID proveedor
- Cantidad
- Precio unitario de compra

Validaciones principales:

- Cantidad > 0
- Precio unitario >= 0
- Producto y proveedor deben existir

Efecto esperado:

- Aumenta el stock del producto
- Se guarda la transaccion de tipo Compra

### 8.2 Registrar venta
Precondicion:

- Debe existir al menos un producto activo y un cliente activo.

Datos solicitados:

- ID producto
- ID cliente
- Cantidad

Validaciones principales:

- Cantidad > 0
- Producto y cliente deben existir
- Debe haber stock suficiente

Efecto esperado:

- Disminuye el stock del producto
- Se guarda la transaccion de tipo Venta

### 8.3 Listar transacciones
Muestra historial con:

- ID
- Tipo
- Producto relacionado
- ID relacionado (cliente o proveedor)
- Cantidad
- Total
- Fecha

### 8.4 Buscar transaccion
Criterios disponibles:

- Por ID
- Por Tipo (Compra o Venta)
- Por ID relacionado
- Por Fecha (DD/MM/AAAA)

Nota:

- En Tipo, escribir exactamente Compra o Venta.

### 8.5 Cancelar transaccion
Pide ID y confirmacion S/N.

Efecto de cancelacion:

- Si era Compra, revierte stock restando la cantidad comprada.
- Si era Venta, revierte stock sumando la cantidad vendida.
- Si no es posible revertir una compra por falta de stock actual, la cancelacion se rechaza.

## 9. Mantenimiento y Reportes
Menu Mantenimiento:

- [1] Verificar integridad referencial
- [2] Crear backup
- [3] Reporte de stock critico
- [4] Historial por cliente
- [0] Volver

### 9.1 Verificar integridad referencial
Revisa enlaces entre entidades y muestra inconsistencias si existen.

### 9.2 Crear backup
Genera copia de seguridad de archivos de datos en una carpeta con fecha.

### 9.3 Reporte de stock critico
Lista productos con nivel critico de inventario.

### 9.4 Historial por cliente
Solicita ID de cliente y muestra las ventas asociadas.

## 10. Mensajes frecuentes y que significan

- "Entrada invalida": se ingreso texto o un numero fuera del rango permitido.
- "No encontrado": el ID o criterio no coincide con registros activos.
- "Operacion cancelada": se eligio no confirmar (o no se cumplen condiciones).
- "No se puede eliminar": existe relacion activa que impide borrado.
- "Stock insuficiente": no hay inventario para completar una venta.

## 11. Buenas practicas para usuario regular

- Registrar primero proveedores y clientes antes de hacer movimientos.
- Confirmar IDs con la opcion de listado antes de crear ventas/compras.
- Revisar stock critico al final de la jornada.
- Crear backup despues de una jornada de carga importante.
- Evitar eliminar registros si aun se usan para historial operativo.

## 12. Flujo rapido sugerido (diario)

1. Listar productos y revisar stock
2. Registrar compras de reposicion
3. Registrar ventas del dia
4. Consultar stock critico
5. Crear backup

---
Documento orientado a usuarios operativos (caja, almacen o asistencia administrativa) para uso diario del sistema.
