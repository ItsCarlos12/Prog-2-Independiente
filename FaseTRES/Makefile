CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -I.

COMMON_SRC = \
	persistencia/GestorArchivos.cpp \
	utilidades/Validaciones.cpp \
	utilidades/Formatos.cpp \
	tienda/Tienda.cpp \
	productos/Producto.cpp \
	productos/operacionesProductos.cpp \
	proveedores/Proveedor.cpp \
	proveedores/operacionesProveedores.cpp \
	clientes/Cliente.cpp \
	clientes/operacionesClientes.cpp \
	transacciones/Transaccion.cpp \
	transacciones/operacionesTransacciones.cpp \
	mantenimiento/operacionesMantenimiento.cpp \
	interfaz/Interfaz.cpp

APP_SRC = $(COMMON_SRC) main.cpp
SEED_SRC = $(COMMON_SRC) seeder.cpp

TARGET = tienda_v3
SEED_TARGET = seeder_v3

.PHONY: all clean seed

all: $(TARGET)

$(TARGET): $(APP_SRC)
	$(CXX) $(CXXFLAGS) $(APP_SRC) -o $(TARGET)

seed: $(SEED_TARGET)

$(SEED_TARGET): $(SEED_SRC)
	$(CXX) $(CXXFLAGS) $(SEED_SRC) -o $(SEED_TARGET)

clean:
	-del /Q $(TARGET).exe 2>nul
	-del /Q $(SEED_TARGET).exe 2>nul
