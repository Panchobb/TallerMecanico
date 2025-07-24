#include "Repuestos.h"
#include "Servicios.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cstring>
#include <cctype>
#include "rlutil.h"

using namespace rlutil;
using namespace std;

// Implementación de métodos de la clase Repuesto
void Repuesto::SetCodigo(const char* c) {
    if(c == nullptr) return;
    strncpy(codigo, c, 9);
    codigo[9] = '\0';
}

void Repuesto::SetMarca(const char* m) {
    if(m == nullptr) return;
    strncpy(marca, m, 19);
    marca[19] = '\0';
}

void Repuesto::SetModelo(const char* mod) {
    if(mod == nullptr) return;
    strncpy(modelo, mod, 19);
    modelo[19] = '\0';
}

void Repuesto::SetDescripcion(const char* d) {
    if(d == nullptr) return;
    strncpy(descripcion, d, 99);
    descripcion[99] = '\0';
}

void Repuesto::SetCantidad(int c) {
    cantidad = (c >= 0) ? c : 0;  // No permitir cantidades negativas
}

bool Repuesto::GuardarEnDisco() {
    ofstream archivo("repuestos.dat", ios::binary | ios::app);
    if(!archivo) return false;
    archivo.write((char*)this, sizeof(Repuesto));
    archivo.close();
    return true;
}

bool Repuesto::LeerDeDisco(int pos) {
    ifstream archivo("repuestos.dat", ios::binary);
    if(!archivo) return false;
    archivo.seekg(pos * sizeof(Repuesto));
    archivo.read((char*)this, sizeof(Repuesto));
    archivo.close();
    return archivo.good();
}

bool Repuesto::ModificarEnDisco(int pos) {
    fstream archivo("repuestos.dat", ios::binary | ios::in | ios::out);
    if(!archivo) return false;
    archivo.seekg(pos * sizeof(Repuesto));
    archivo.write((char*)this, sizeof(Repuesto));
    archivo.close();
    return archivo.good();
}

void Repuesto::Cargar() {
    cout << "CODIGO (max 9 chars): ";
    cin.getline(codigo, 10);
    cout << "MARCA (max 19 chars): ";
    cin.getline(marca, 20);
    cout << "MODELO (max 19 chars): ";
    cin.getline(modelo, 20);
    cout << "DESCRIPCION (max 99 chars): ";
    cin.getline(descripcion, 100);

    do {
        cout << "CANTIDAD: ";
        cin >> cantidad;
        if(cantidad < 0) cout << "La cantidad no puede ser negativa!\n";
    } while(cantidad < 0);
    cin.ignore();
}

void Repuesto::Mostrar() {
    cout << left;
    cout << setw(10) << codigo;
    cout << setw(20) << marca;
    cout << setw(20) << modelo;
    cout << setw(40) << descripcion;
    cout << right << setw(5) << cantidad << endl;
}

// Funciones globales
int CantidadRegistrosRepuestos() {
    ifstream archivo("repuestos.dat", ios::binary | ios::ate);
    if(!archivo) return 0;
    int tam = archivo.tellg();
    archivo.close();
    return tam / sizeof(Repuesto);
}

bool BuscarRepuesto(const char* codigo, Repuesto& rep) {
    if(codigo == nullptr) return false;

    int cant = CantidadRegistrosRepuestos();
    for(int i = 0; i < cant; i++) {
        if(rep.LeerDeDisco(i) && strcmp(rep.GetCodigo(), codigo) == 0) {
            return true;
        }
    }
    return false;
}

bool BuscarRepuestoPorPosicion(int pos, Repuesto& rep) {
    return rep.LeerDeDisco(pos);
}




void mostrarEncabezadoRepuestos(int centro_x, int centro_y, int ANCHO_CUADRO) {
    setColor(LIGHTGREEN);
    locate(centro_x, centro_y);
    cout << "╭" << string(ANCHO_CUADRO-2, '─') << "╮";

    locate(centro_x, centro_y+1);
    cout << "│" << string(ANCHO_CUADRO-2, ' ') << "│";

    const string TITULO = " MENÚ DE REPUESTOS ";
    locate(centro_x + (ANCHO_CUADRO - TITULO.length())/2, centro_y+2);
    cout << TITULO;

    locate(centro_x, centro_y+3);
    cout << "│" << string(ANCHO_CUADRO-2, ' ') << "│";

    locate(centro_x, centro_y+4);
    cout << "├" << string(ANCHO_CUADRO-2, '─') << "┤";
}

void MenuRepuestos() {
    int opcion = 0;
    const int TOTAL_OPCIONES = 4; // Reducido a 4 opciones
    const int ANCHO_CUADRO = 65;

    // Centrado automático
    int centro_x = max(1, (tcols() - ANCHO_CUADRO) / 2);
    int centro_y = max(1, (trows() - 22) / 2);

    const string opciones[] = {
        " ➕ NUEVO REPUESTO ",
        " 📜 LISTAR TODOS LOS REPUESTOS ",
        " 🔍 BUSCAR POR CÓDIGO ",
        " ↩ VOLVER AL MENÚ ANTERIOR "
    };

    do {
        cls();
        mostrarEncabezadoRepuestos(centro_x, centro_y, ANCHO_CUADRO);

        // Opciones del menú
        for (int i = 0; i < TOTAL_OPCIONES; ++i) {
            int opcion_x = centro_x + (ANCHO_CUADRO - opciones[i].length()) / 2;
            locate(opcion_x, centro_y + 6 + i);

            if (i == opcion) {
                setColor(YELLOW);
                setBackgroundColor(DARKGREY);
                cout << "» " << opciones[i] << " «";
            } else {
                setColor(WHITE);
                setBackgroundColor(BLACK);
                cout << "  " << opciones[i] << "  ";
            }
        }
        resetColor();

        // Instrucciones
        string instruccion = "[ ↑ ↓ ] NAVEGAR   [ ENTER ] SELECCIONAR";
        locate(centro_x + (ANCHO_CUADRO - instruccion.length())/2, centro_y + 11);
        setColor(GREY);
        cout << instruccion;

        // Marco inferior
        setColor(LIGHTGREEN);
        locate(centro_x, centro_y+13);
        cout << "╰" << string(ANCHO_CUADRO-2, '─') << "╯";

        // Lógica de control
        int key;
        do {
            key = getkey();
        } while (key != KEY_ENTER && key != KEY_UP && key != KEY_DOWN);

        switch (key) {
            case KEY_UP:
                opcion = (opcion > 0) ? opcion-1 : TOTAL_OPCIONES-1;
                break;
            case KEY_DOWN:
                opcion = (opcion < TOTAL_OPCIONES-1) ? opcion+1 : 0;
                break;
            case KEY_ENTER:
                cls();
                switch(opcion) {
                    case 0: { // Nuevo repuesto
                        mostrarEncabezadoRepuestos(centro_x, centro_y, ANCHO_CUADRO);

                        Repuesto nuevo;
                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "NUEVO REPUESTO";

                        // Código
                        locate(centro_x+4, centro_y+8);
                        cout << "Código (9 caracteres): ";
                        char codigo[10];
                        cin.getline(codigo, 10);
                        nuevo.SetCodigo(codigo);

                        // Verificar si ya existe
                        Repuesto existente;
                        if(BuscarRepuesto(codigo, existente)) {
                            locate(centro_x+4, centro_y+10);
                            setColor(LIGHTRED);
                            cout << "¡ERROR! Ya existe un repuesto con ese código";
                        } else {
                            // Resto de datos
                            locate(centro_x+4, centro_y+10);
                            cout << "Marca: ";
                            char marca[20];
                            cin.getline(marca, 20);
                            nuevo.SetMarca(marca);

                            locate(centro_x+4, centro_y+11);
                            cout << "Modelo: ";
                            char modelo[20];
                            cin.getline(modelo, 20);
                            nuevo.SetModelo(modelo);

                            locate(centro_x+4, centro_y+12);
                            cout << "Descripción: ";
                            char desc[100];
                            cin.getline(desc, 100);
                            nuevo.SetDescripcion(desc);

                            locate(centro_x+4, centro_y+13);
                            cout << "Cantidad: ";
                            int cant;
                            cin >> cant;
                            nuevo.SetCantidad(cant);
                            cin.ignore();

                            if(nuevo.GuardarEnDisco()) {
                                locate(centro_x+4, centro_y+15);
                                setColor(LIGHTGREEN);
                                cout << "¡Repuesto registrado exitosamente!";
                            } else {
                                locate(centro_x+4, centro_y+15);
                                setColor(LIGHTRED);
                                cout << "Error al guardar el repuesto";
                            }
                        }
                        break;
                    }

                    case 1: { // Listar todos
                        cls();
                        mostrarEncabezadoRepuestos(centro_x, centro_y, ANCHO_CUADRO);

                        int cant = CantidadRegistrosRepuestos();
                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "LISTADO DE REPUESTOS (" << cant << " registros)";

                        // Encabezado de tabla
                        locate(centro_x+4, centro_y+8);
                        cout << left << setw(10) << "CÓDIGO" << setw(20) << "MARCA"
                             << setw(20) << "MODELO" << setw(25) << "DESCRIPCIÓN"
                             << right << setw(10) << "STOCK";

                        // Datos
                        Repuesto rep;
                        int fila = centro_y+10;
                        for(int i = 0; i < cant; i++) {
                            if(rep.LeerDeDisco(i)) {
                                locate(centro_x+4, fila++);
                                cout << left << setw(10) << rep.GetCodigo();
                                cout << setw(20) << rep.GetMarca();
                                cout << setw(20) << rep.GetModelo();
                                cout << setw(25) << rep.GetDescripcion();
                                cout << right << setw(10) << rep.GetCantidad() << endl;
                            }
                        }
                        break;
                    }

                    case 2: { // Buscar por código (con opción de modificar)
                        mostrarEncabezadoRepuestos(centro_x, centro_y, ANCHO_CUADRO);

                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "BUSCAR REPUESTO POR CÓDIGO";

                        locate(centro_x+4, centro_y+8);
                        cout << "Ingrese código: ";
                        char codigo[10];
                        cin.getline(codigo, 10);

                        Repuesto encontrado;
                        int posicion = -1;
                        int cant = CantidadRegistrosRepuestos();

                        // Buscar repuesto y su posición
                        for(int i = 0; i < cant; i++) {
                            if(encontrado.LeerDeDisco(i) && strcmp(encontrado.GetCodigo(), codigo) == 0) {
                                posicion = i;
                                break;
                            }
                        }

                        if(posicion != -1) {
                            cls();
                            mostrarEncabezadoRepuestos(centro_x, centro_y, ANCHO_CUADRO);
                            locate(centro_x+4, centro_y+6);
                            setColor(LIGHTGREEN);
                            cout << "REPUESTO ENCONTRADO:";

                            // Mostrar datos
                            locate(centro_x+4, centro_y+8);
                            cout << "Código: " << encontrado.GetCodigo();
                            locate(centro_x+4, centro_y+9);
                            cout << "Marca: " << encontrado.GetMarca();
                            locate(centro_x+4, centro_y+10);
                            cout << "Modelo: " << encontrado.GetModelo();
                            locate(centro_x+4, centro_y+11);
                            cout << "Descripción: " << encontrado.GetDescripcion();
                            locate(centro_x+4, centro_y+12);
                            cout << "Stock: " << encontrado.GetCantidad();

                            // Preguntar por modificación
                            locate(centro_x+4, centro_y+14);
                            setColor(WHITE);
                            cout << "¿Desea modificar este repuesto? (s/n): ";
                            char opcion;
                            cin >> opcion;
                            cin.ignore();

                            if(tolower(opcion) == 's') {
                                Repuesto modificado = encontrado;
                                char input[100];

                                locate(centro_x+4, centro_y+16);
                                cout << "Nueva marca (actual: " << modificado.GetMarca() << "): ";
                                cin.getline(input, 100);
                                if(strlen(input) > 0) modificado.SetMarca(input);

                                locate(centro_x+4, centro_y+17);
                                cout << "Nuevo modelo (actual: " << modificado.GetModelo() << "): ";
                                cin.getline(input, 100);
                                if(strlen(input) > 0) modificado.SetModelo(input);

                                locate(centro_x+4, centro_y+18);
                                cout << "Nueva descripción (actual: " << modificado.GetDescripcion() << "): ";
                                cin.getline(input, 100);
                                if(strlen(input) > 0) modificado.SetDescripcion(input);

                                locate(centro_x+4, centro_y+19);
                                cout << "Nuevo stock (actual: " << modificado.GetCantidad() << "): ";
                                int nuevaCant;
                                cin >> nuevaCant;
                                cin.ignore();
                                modificado.SetCantidad(nuevaCant);

                                if(modificado.ModificarEnDisco(posicion)) {
                                    locate(centro_x+4, centro_y+21);
                                    setColor(LIGHTGREEN);
                                    cout << "¡Repuesto modificado exitosamente!";
                                } else {
                                    locate(centro_x+4, centro_y+21);
                                    setColor(LIGHTRED);
                                    cout << "Error al modificar el repuesto";
                                }
                            }
                        } else {
                            locate(centro_x+4, centro_y+10);
                            setColor(LIGHTRED);
                            cout << "¡Repuesto no encontrado!";
                        }
                        break;
                    }

                    case 3: // Volver al menú anterior
                        return;
                }

                if(opcion != 3) {
                    locate(centro_x+4, centro_y+23);
                    setColor(WHITE);
                    cout << "Presione cualquier tecla para continuar...";
                    anykey();
                }
                break;
        }
    } while(true);
}
