#include <iostream>
#include <ctime>
#include <cstring> 
#include<iomanip>
#include "structs.h"

using namespace std;

static int col = 7;
static int anchosProd [] = {4, 12, 20, 12, 10, 8, 12};

//5. Redimensionar el array.
void redimensionarProductos(Tienda* tienda){
    //calcular nueva capacidad (doble de la actual).
    int nuevaCapacidad = tienda -> capacidadProductos * 2;

    //crear nuevo arreglo con la nueva capacidad.
    Producto* newArray = new Producto[nuevaCapacidad];

    //copiar todos los productos.
    for(int i=0; i<tienda -> numProductos; i++){
        newArray[i] = tienda -> productos[i];
    }

    delete [] tienda -> productos;
    tienda -> productos = nullptr;

    tienda -> productos = newArray;
    tienda -> capacidadProductos = nuevaCapacidad;

    cout<<"Redimensionado"<<endl;

}

bool codigoDuplicado(Tienda* tienda, const char* codigo){
    for(int i=0; i<tienda -> numProductos; i++){
        if(strcmp(tienda -> productos[i].codigo, codigo) == 0){
            return true;
        }
    }
    return false;
}

//Para verificar que el codigo que vamos a cambiar no este ya en otro producto.
bool codigoDuplicadoEnOtros(Tienda* tienda, const char* codigo, int idExcluir){
    for(int i = 0; i < tienda -> numProductos; i++){
        if(tienda -> productos[i].id != idExcluir && strcmp(tienda -> productos[i].codigo, codigo) == 0){
            return true;
        }
    }
    return false;
}

bool existeProveedor(Tienda* tienda, int idProveedor){
    for(int i=0; i<tienda -> numProveedores; i++){
        if(tienda -> proveedores[i].id == idProveedor){
            return true;
        }
    }
    return false;
}

void obtenerFechaActual(char* fecha){
    time_t t = time(0);
    tm* now = localtime(&t);

    strftime(fecha, 11, "%d/%m/%Y", now);
}

void crearProducto(Tienda* tienda){
    char opcion;
    char buffer[200];

    //1. Preguntar si desea registrar un nuevo producto.
    cout<<"\n Desea registrar un nuevo producto? (S/N): ";
    cin>>opcion;
    cin.ignore(); //Para limpiar el buffer.

    //2. Confirmacion antes de guardar.
    if(opcion == 'n' || opcion == 'N'){
        cout<<"Gracias por usar nuestro sistema de inventario. ¡Hasta luego!"<<endl;
        return;
    }

    //Si la tienda ya tiene el maximo de productos, se redimensiona el arreglo.
    if(tienda -> numProductos == tienda -> capacidadProductos){
        redimensionarProductos(tienda);
    }

    Producto& p = tienda -> productos[tienda -> numProductos];

    //3. Solicitar los datos.

    //Codigo
    cout<<"Ingrese el codigo del producto: (o 0 para cancelar): ";
    cin.getline(buffer, 20);
    if(strcmp(buffer, "0") == 0 || strcasecmp(buffer, "Cancelar")== 0 ){
        cout<<"Registro de producto cancelado."<<endl;
        return;
    }

    if(codigoDuplicado(tienda, buffer)){
        cout << "Error: el codigo " << buffer << "ya existe" <<endl;
        return;
    }
    strcpy(p.codigo, buffer);

    //Nombre
    cout << "Ingrese el nombre del producto: ";
    cin.getline(p.nombre, 100);

    //Descripcion
    cout << "Ingrese la descripcion del producto: ";
    cin.getline(p.descripcion, 200);

    //ID proveedor
    int temp;
    cout << "Ingrese el ID del proveedor: ";
    cin >> temp;

    if(!existeProveedor(tienda, temp)){
        cout << "Error: el proveedor con ID " << temp << " no existe." << endl;
        return;
    }

    p.idProveedor = temp;

    //Precio
    cout << "Ingrese el precio del producto: ";
    cin >> p.precio;
    if(p.precio <= 0){
        cout << "Error: el precio debe ser mayor a 0." << endl;
        return;
    }

    //Stock
    cout <<"Ingrese el stock del producto: ";
    cin >> p.stock;
    if(p.stock < 0){
        cout << "Error: el stock debe ser mayor o igual a 0." << endl;
        return;
    }
    cin.ignore();

    //4 y 6. Asignacion automatica

    p.id = tienda -> siguienteIdProducto;
    obtenerFechaActual(p.fechaRegistro);

    cout << "\n--- Vista previa del registro ---" <<endl;
    mostrarTabla();
    imprimirFilaProducto(p);
    int anchosProd[] = {4, 12, 20, 12, 10, 8, 12};
    dibujarTabla(anchosProd, col);

    cout << "Descripcion: " << p.descripcion <<endl;

    cout <<" Confirmar guardado? (S/N): ";
    cin >> opcion;

    if(opcion == 's' || opcion == 'S'){
        tienda -> numProductos++;
        tienda -> siguienteIdProducto++;
        cout<<"Producto guardado exitosamente."<<endl;
    }
    else{
        cout<<"Registro descartado."<<endl;
    }
}

void buscarProducto(Tienda* tienda){
    if(inventarioVacio(tienda)) return;

    int opcion;
    char criterio[100];
    cout << "\n--- Menu de Busqueda ---" << endl;
    cout << "1. Por Id\n2. Por codigo\n3. Por Nombre (Parcial)\n4. Por ID Proveedor\n0. Cancelar\nSeleccione: ";
    cin >> opcion;
    cin.ignore();

    if(opcion == 0){
        cout << "Busqueda cancelada. Volviendo al menu principal." << endl;
        return;
    }

    cout << "Ingrese el valor a buscar: ";
    cin.getline(criterio, 100);

    bool encontrado = false;
    bool tablaMostrada = false;

    for(int i = 0; i < tienda -> numProductos; i++){
        Producto& p = tienda -> productos[i];
        bool coincide = false;

        switch(opcion){
            case 1:
                if(p.id == atoi(criterio)){
                    coincide = true;
                }
                break;
            case 2:
                if(compararLetras(p.codigo, criterio)){
                    coincide = true;
                }
                break;
            case 3:
                if(compararLetras(p.nombre, criterio)){
                    coincide = true;
                }
                break;
            case 4:
                if(p.idProveedor == atoi(criterio)){
                    coincide = true;
                }
                break;
            default:
                cout << "Opcion no valida." << endl;
                return;
        }

        if(coincide){
            if(!tablaMostrada){
                mostrarTabla();
                tablaMostrada = true;
            }
            imprimirFilaProducto(p);
            dibujarTabla(anchosProd, col);
            encontrado = true;
        }
    }

    if(!encontrado){
        cout << "\nNo se encontraron coincidencias." << endl;
    }
}

//Mostrar Tabla
void mostrarTabla(){
    int anchoTotal = 0;
    for(int i = 0; i < col; i++){
        anchoTotal += anchosProd[i];
    }
    anchoTotal += col * 3 + 1;

    cout << "\n" << setfill('=') << setw(anchoTotal) << "" << endl;
    cout << setfill(' ');
    int padding = (anchoTotal - strlen("Listado de Productos")) / 2;
    cout << setw(padding) << "" << "Listado de Productos" << endl;
    cout << setw(anchoTotal) << setfill('=') << "" << endl;
    cout << setfill(' ');

    dibujarTabla(anchosProd, col);

    cout << "| " << left << setw(anchosProd[0]) << "ID"
         << "| " << setw(anchosProd[1]) << "Codigo"
         << "| " << setw(anchosProd[2]) << "Nombre"
         << "| " << setw(anchosProd[3]) << "Proveedor"
         << "| " << setw(anchosProd[4]) << "Precio"
         << "| " << setw(anchosProd[5]) << "Stock"
         << "| " << setw(anchosProd[6]) << "Fecha" << " |" <<endl;
         
    dibujarTabla(anchosProd, col);
}

//Imprimir los productos en tabla.
void imprimirFilaProducto(const Producto& p){

    cout << "| " << left << setw(anchosProd[0]) << p.id
         << "| " << left << setw(anchosProd[1]) << p.codigo
         << "| " << left << setw(anchosProd[2]) << p.nombre
         << "| " << left << setw(anchosProd[3]) << p.idProveedor
         << "| " << left << setw(anchosProd[4]) << fixed << setprecision(2) <<  p.precio
         << "| " << left << setw(anchosProd[5]) << p.stock
         << "| " << left << setw(anchosProd[6]) << p.fechaRegistro << " |" <<endl;
}

int obtenerIndice(Tienda * tienda, int idBuscar){
    for(int i = 0; i < tienda -> numProductos; i++){
        if(tienda -> productos[i].id == idBuscar){
            return i; 
        }
    }
    return -1;
}

void actualizarProducto(Tienda* tienda){
    if(inventarioVacio(tienda)) return;

    int idBuscar;
    cout << "\n---Actualizar producto---" << endl;
    cout << "Ingrese el ID del producto: ";
    cin >> idBuscar;
    cin.ignore();

    int indice = obtenerIndice(tienda, idBuscar);

    if(indice == -1){
        cout << "Error: el producto con ID " << idBuscar << " no existe." << endl;
        return;
    }

    Producto pEdit = tienda -> productos[indice];
    int opcion;

    do{
        cout << "\nDatos actuales del producto" << endl;
        mostrarTabla();
        imprimirFilaProducto(pEdit);
        dibujarTabla(anchosProd, col);

        cout << "\nQue desea editar?" << endl;
        cout << "1. Codigo\n2. Nombre\n3. Descripcion\n4. Proveedor\n5. Precio\n6. Stock\n7. Guardar cambios\n0. Cancelar sin guardar\nSeleccione: ";
        cin >> opcion;
        cin.ignore();

        switch(opcion){
            case 0:{
                cout << "Cambios descartados. Volviendo al menu principal." << endl;
                return;
            }
            case 7:{
                char confirmar;
                cout << "Guardar cambios? (S/N): ";
                cin >> confirmar;
                cin.ignore();
                if(confirmar == 's' || confirmar == 'S'){
                    tienda -> productos[indice] = pEdit;
                    cout << "Producto actualizado correctamente." << endl;
                    return;
                }
                break;
            }
            case 1:{
                char buffer[20];
                cout << "Nuevo codigo (actual: " << pEdit.codigo << "). 0 o CANCELAR para no cambiar: ";
                cin.getline(buffer, 20);
                if(strcmp(buffer, "0") == 0 || strcasecmp(buffer, "CANCELAR") == 0){
                    break;
                }
                if(codigoDuplicadoEnOtros(tienda, buffer, pEdit.id)){
                    cout << "Error: el codigo ya existe en otro producto." << endl;
                    break;
                }
                strcpy(pEdit.codigo, buffer);
                cout << "Codigo actualizado." << endl;
                break;
            }
            case 2:{
                char buffer[100];
                cout << "Nuevo nombre (actual: " << pEdit.nombre << "). 0 o CANCELAR para no cambiar: ";
                cin.getline(buffer, 100);
                if(strcmp(buffer, "0") == 0 || strcasecmp(buffer, "CANCELAR") == 0){
                    break;
                }
                strcpy(pEdit.nombre, buffer);
                cout << "Nombre actualizado." << endl;
                break;
            }
            case 3:{
                char buffer[200];
                cout << "Nueva descripcion (actual: " << pEdit.descripcion << "). 0 o CANCELAR para no cambiar: ";
                cin.getline(buffer, 200);
                if(strcmp(buffer, "0") == 0 || strcasecmp(buffer, "CANCELAR") == 0){
                    break;
                }
                strcpy(pEdit.descripcion, buffer);
                cout << "Descripcion actualizada." << endl;
                break;
            }
            case 4:{
                int temp;
                cout << "Nuevo ID de proveedor (actual: " << pEdit.idProveedor << "). 0 para no cambiar: ";
                cin >> temp;
                cin.ignore();
                if(temp == 0){
                    break;
                }
                if(!existeProveedor(tienda, temp)){
                    cout << "Error: el proveedor con ID " << temp << " no existe." << endl;
                    break;
                }
                pEdit.idProveedor = temp;
                cout << "Proveedor actualizado." << endl;
                break;
            }
            case 5:{
                float nuevoPrecio;
                cout << "Nuevo precio (actual: " << pEdit.precio << "). 0 para no cambiar: ";
                cin >> nuevoPrecio;
                cin.ignore();
                if(nuevoPrecio == 0){
                    break;
                }
                if(nuevoPrecio <= 0){
                    cout << "Error: el precio debe ser mayor a 0." << endl;
                    break;
                }
                pEdit.precio = nuevoPrecio;
                cout << "Precio actualizado." << endl;
                break;
            }
            case 6:{
                int nuevoStock;
                cout << "Nuevo stock (actual: " << pEdit.stock << "). Ingrese valor negativo para no cambiar: ";
                cin >> nuevoStock;
                cin.ignore();
                if(nuevoStock < 0){
                    break;
                }
                pEdit.stock = nuevoStock;
                cout << "Stock actualizado." << endl;
                break;
            }
            default:
                cout << "Opcion no valida." << endl;
        }
    } while(opcion!=0 && opcion != 7);
}

void eliminarProducto(Tienda* tienda){

    if(inventarioVacio(tienda)) return;

    int idBuscar;
    cout << "\n--- Eliminar Producto ---" << endl;
    cout << "Ingrese el ID del producto a eliminar: ";
    cin >> idBuscar;
    cin.ignore();

    int indice = obtenerIndice(tienda, idBuscar);

    if(indice == -1){
        cout << "Error: no se encontro el producto con el ID: " << idBuscar << endl;
    }

    cout << "\nProducto a eliminar: " << endl;
    mostrarTabla();
    imprimirFilaProducto(tienda -> productos[indice]);
    dibujarTabla(anchosProd, col);

    char confirmar;
    cout << "\n Esta seguro de eliminar este producto permanentemente? (S/N)";
    cin >> confirmar;
    cin.ignore();

    if(confirmar == 's' || confirmar == 'S'){
        for(int i = indice; i<tienda -> numProductos -1; i++){
            tienda -> productos[i] = tienda -> productos[i+1];
        }

        tienda -> numProductos--;

        cout << "Producto eliminado exitosamente." << endl;
    }else{
        cout <<  "Operacion cancelada." << endl; 
    }
}

void listarProductos(Tienda* tienda){
    if(inventarioVacio(tienda)) return;

    mostrarTabla();

    for(int i = 0; i < tienda -> numProductos; i++){
        imprimirFilaProducto(tienda -> productos[i]);
    }

    dibujarTabla(anchosProd, col);

    cout << "Total de productos registrados: " << tienda -> numProductos << endl;

    cout << "\nPresione ENTER para continuar...";
    cin.get();
}

void actualizarStockProducto(Tienda* tienda){
    if(inventarioVacio(tienda)) return;

    int idBuscar;
    cout << "\n--- Actualizar Stock(Manual) ---" << endl;
    cout << "Ingrese el ID del producto: ";
    cin >> idBuscar;
    cin.ignore();

    int indice = obtenerIndice(tienda, idBuscar);

    if(indice == -1){
        cout << "\n Error: producto no encontrado." << endl;
        return;
    }

    Producto& p = tienda -> productos[indice];

    cout << "\nProducto: " << p.nombre << "| Stock Actual: " << p.stock << endl;

    int ajuste;
    cout << "Ingrese la cantidad a ajustar(por ejemplo 10 para sumar, -5 para restar): ";
    cin >> ajuste;
    cin.ignore();

    if(p.stock + ajuste < 0){
        cout << "Error: el stock final no puede ser menor a 0. Operacion cancelada." << endl;
        return;
    }

    char confirmar; 
    cout << "Confirmar ajuste. Final sera: " << (p.stock + ajuste) << " (S/N)";
    cin >> confirmar;
    cin.ignore();

    if(confirmar == 's'|| confirmar == 'S'){
        p.stock += ajuste;
        cout << "Stock actualizado correctamente." << endl;
    }else{
        cout << "Ajuste cancelado. " << endl;
    }
}

void menuProductos(Tienda* tienda){
    int opcion;
    
    do{
        mostrarBanner("Gestion de Productos");
        cout << endl;
        cout << "1. Registrar producto" << endl;
        cout << "2. Buscar producto" <<endl;
        cout << "3. Actualizar datos (General)" << endl;
        cout << "4. Actualizar stock (Manual)" << endl;
        cout << "5. Listar inventario completo" << endl;
        cout << "6. Eliminar producto" << endl;
        cout << "0. Volver al menu principal" << endl;
        cout << "\n Seleccione una opcion: ";

        cin >> opcion;
        cin.ignore();

        switch (opcion){
            case 1: crearProducto(tienda); break;
            case 2: buscarProducto(tienda); break;
            case 3: actualizarProducto(tienda); break;
            case 4: actualizarStockProducto(tienda); break;
            case 5: listarProductos(tienda); break;
            case 6: eliminarProducto(tienda); break;
            case 0: break;
            default: cout << "\n Opcion no valida" << endl;
        }
    }while(opcion != 0);
}