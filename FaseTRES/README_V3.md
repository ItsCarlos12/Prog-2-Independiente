# Proyecto Tienda v3 - POO y Modularizacion

## Estructura de la fase

La fase 3 esta implementada dentro de `FaseTRES/` y separada por dominio:

- `tienda/` entidad principal.
- `productos/`, `proveedores/`, `clientes/`, `transacciones/` entidades y operaciones.
- `persistencia/` `GestorArchivos` + `Constantes`.
- `utilidades/` validaciones y formatos.
- `interfaz/` menus y navegacion.
- `mantenimiento/` integridad, backup y reportes.
- `datos/` archivos binarios generados.

## Compilacion

Desde `FaseTRES/`:

```bash
make
```

Genera el ejecutable `tienda_v3`.

## Ejecutar

```bash
./tienda_v3
```

## Seeder de datos de prueba

Para generar al menos 5 proveedores, 15 productos, 8 clientes y 25 transacciones:

```bash
make seed
./seeder_v3
```

## Notas tecnicas

- Persistencia binaria con encabezados por archivo.
- CRUD generico usando templates en `GestorArchivos.hpp`.
- Encapsulamiento completo con clases y getters/setters.
- `main.cpp` solo inicializa archivos y delega en `Interfaz::ejecutar()`.
