#include "Servicios.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <cctype>
#include "rlutil.h"
using namespace rlutil;
using namespace std;
int GenerarNuevoCodigoServicio() {
    ifstream archivo("servicios.dat", ios::binary | ios::ate);
    if(archivo.is_open() && archivo.tellg() > 0) {
        int cant = archivo.tellg() / sizeof(Servicio);
        archivo.close();
        return cant + 1;
    }
    archivo.close();
    return 1;
}

bool Servicio::AgregarRepuesto(const char* codigo, int cantidad) {
    if(cantRepuestos >= 5 || codigo == nullptr || cantidad <= 0) {
        cout << "No se pueden agregar más repuestos, código inválido o cantidad incorrecta\n";
        return false;
    }

    Repuesto rep;
    if(!BuscarRepuesto(codigo, rep)) {
        cout << "Repuesto no encontrado en el sistema\n";
        return false;
    }

    if(!VerificarStockRepuesto(codigo, cantidad)) {
        cout << "No hay suficiente stock disponible (" << rep.GetCantidad() << " disponibles)\n";
        return false;
    }

    for(int i = 0; i < cantRepuestos; i++) {
        if(strcmp(repuestos[i], codigo) == 0) {
            cantidades[i] += cantidad;
            if(ReducirStockRepuesto(codigo, cantidad)) {
                cout << cantidad << " unidad(es) más agregadas al repuesto existente\n";
                return true;
            }
            return false;
        }
    }

    strncpy(repuestos[cantRepuestos], codigo, 9);
    repuestos[cantRepuestos][9] = '\0';
    cantidades[cantRepuestos] = cantidad;
    cantRepuestos++;

    if(ReducirStockRepuesto(codigo, cantidad)) {
        cout << cantidad << " unidad(es) del repuesto agregado(s)\n";
        return true;
    }
    return false;
}

void Servicio::MostrarRepuestos() const {
    if(cantRepuestos == 0) {
        cout << "No hay repuestos asociados\n";
        return;
    }

    Repuesto rep;
    cout << "REPUESTOS ASOCIADOS:\n";
    cout << setw(10) << "CODIGO" << setw(20) << "MARCA"
         << setw(30) << "DESCRIPCION" << setw(10) << "CANTIDAD" << endl;

    for(int i = 0; i < cantRepuestos; i++) {
        if(BuscarRepuesto(repuestos[i], rep)) {
            cout << setw(10) << rep.GetCodigo();
            cout << setw(20) << rep.GetMarca();
            cout << setw(30) << rep.GetDescripcion();
            cout << setw(10) << cantidades[i] << endl;
        }
    }
}


bool Servicio::VerificarStockRepuesto(const char* codigoRepuesto, int cantidad) {
    Repuesto rep;
    if(BuscarRepuesto(codigoRepuesto, rep)) {
        return rep.GetCantidad() >= cantidad;
    }
    return false;
}

bool Servicio::ReducirStockRepuesto(const char* codigoRepuesto, int cantidad) {
    Repuesto rep;
    int cant = CantidadRegistrosRepuestos();

    for(int i = 0; i < cant; i++) {
        if(rep.LeerDeDisco(i) && strcmp(rep.GetCodigo(), codigoRepuesto) == 0) {
            if(rep.GetCantidad() >= cantidad) {
                rep.SetCantidad(rep.GetCantidad() - cantidad);
                return rep.ModificarEnDisco(i);
            }
            return false;
        }
    }
    return false;
}
bool Servicio::GuardarEnDisco() {
    ofstream archivo("servicios.dat", ios::binary | ios::app);
    if(!archivo) return false;
    archivo.write(reinterpret_cast<const char*>(this), sizeof(Servicio));
    archivo.close();
    return true;
}
bool Servicio::LeerDeDisco(int pos) {
    ifstream archivo("servicios.dat", ios::binary);
    if(!archivo) return false;
    archivo.seekg(pos * sizeof(Servicio));
    archivo.read(reinterpret_cast<char*>(this), sizeof(Servicio));
    archivo.close();
    return archivo.good();
}

bool Servicio::ModificarEnDisco(int pos) {
    fstream archivo("servicios.dat", ios::binary | ios::in | ios::out);
    if(!archivo) return false;
    archivo.seekg(pos * sizeof(Servicio));
    archivo.write(reinterpret_cast<const char*>(this), sizeof(Servicio));
    archivo.close();
    return archivo.good();
}

void Servicio::Cargar() {
    system("cls");
    cout << "NUEVO SERVICIO\n";

    codigo = GenerarNuevoCodigoServicio();
    cout << "Codigo generado automaticamente: " << codigo << endl;

    cout << "Fecha de ingreso: ";
    Fecha::Cargar();
     cin.ignore();

    cout << "\nDATOS DEL CLIENTE:\n";
    cout << "Nombre: ";
    cin.getline(nombreCliente, 20);
    cout << "Apellido: ";
    cin.getline(apellidoCliente, 20);

    cout << "Telefono: ";
    while(!(cin >> telefonoCliente)) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Telefono inválido. Ingrese solo números: ";
    }
    cin.ignore();
    cout << "Marca del Vehiculo: ";
    cin.getline(marcaVehiculo, 20);

    cout << "Modelo del Vehiculo: ";
    cin.getline(modeloVehiculo, 20);

    cout << "Anio del Vehiculo: ";
    while(!(cin >> anioVehiculo) || anioVehiculo < 1900 || anioVehiculo > 2100) {
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Anio inválido. Ingrese un número entre 1900 y 2100: ";
    }
    cin.ignore();

    cout << "Patente del Vehiculo: ";
    cin.getline(patenteVehiculo, 10);

    terminado = false;
    cantRepuestos = 0;

    cout << "Descripcion: ";
    cin.getline(descripcion, 100);

    char opcion;
    cout << "\nDesea asociar repuestos? (s/n): ";
    cin >> opcion;
    cin.ignore();

    if(tolower(opcion) == 's') {
        Repuesto rep;
        char codigo[10];
        int cantidad;

        do {
            cout << "\nCodigo de repuesto (0 para terminar): ";
            cin.getline(codigo, 10);

            if(strcmp(codigo, "0") != 0) {
                cout << "Cantidad: ";
                while(!(cin >> cantidad) || cantidad <= 0) {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Cantidad inválida. Ingrese un número positivo: ";
                }
                cin.ignore();

                if(!BuscarRepuesto(codigo, rep)) {
                    cout << "Repuesto no encontrado!\n";
                } else if(!AgregarRepuesto(codigo, cantidad)) {
                    cout << "No se pudo agregar el repuesto!\n";
                } else {
                    cout << cantidad << " unidad(es) de " << rep.GetDescripcion() << " agregadas\n";
                }
            }
        } while(strcmp(codigo, "0") != 0 && cantRepuestos < 5);
    }
}
void Servicio::Mostrar() const {
    cout << left;
    cout << "CODIGO: " << codigo << endl;
    cout << "Fecha de Ingreso: ";
    Fecha::Mostrar();
      cout << "CLIENTE: " << nombreCliente << " " << apellidoCliente << " (Tel: " << telefonoCliente << ")" << endl;
    cout << "VEHICULO: " << marcaVehiculo << " " << modeloVehiculo
         << " (" << anioVehiculo << ") - " << patenteVehiculo << endl;
    cout << "DESCRIPCION: " << descripcion << endl;
    cout << "ESTADO: " << (terminado ? "TERMINADO" : "EN PROCESO") << endl;

    if(cantRepuestos > 0) {
        int totalUnidades = 0;
        for(int i = 0; i < cantRepuestos; i++) {
            totalUnidades += cantidades[i];
        }
        cout << "REPUESTOS: " << cantRepuestos << " tipos (" << totalUnidades << " unidades)\n";
    }

    MostrarRepuestos();
    cout << "----------------------------------------\n";
}
int CantidadRegistrosServicios() {
    ifstream archivo("servicios.dat", ios::binary | ios::ate);
    if(!archivo) return 0;
    int tam = archivo.tellg();
    archivo.close();
    return tam / sizeof(Servicio);
}

void ListarServiciosPorCliente(const char* nombre, const char* apellido) {
    Servicio serv;
    int cant = CantidadRegistrosServicios();
    bool encontrado = false;
    int totalRepuestos = 0;
    int totalServicios = 0;

    cout << "\nREPORTE DE SERVICIOS PARA: " << nombre << " " << apellido << "\n";
    cout << string(90, '-') << endl;
    cout << setw(8) << "CODIGO" << setw(12) << "FECHA"
         << setw(20) << "VEHICULO" << setw(15) << "PATENTE"
         << setw(10) << "ESTADO" << setw(15) << "REPUESTOS" << endl;
    cout << string(90, '-') << endl;

    for(int i = 0; i < cant; i++) {
        if(serv.LeerDeDisco(i) &&
           strcasecmp(serv.GetNombreCliente(), nombre) == 0 &&
           strcasecmp(serv.GetApellidoCliente(), apellido) == 0) {

            // Formatear fecha para mostrar
            char fechaStr[12];
            serv.GetFecha().ToString(fechaStr); // Asume que Fecha tiene método ToString()

            cout << setw(8) << serv.GetCodigo();
            cout << setw(12) << fechaStr;
            cout << setw(20) << string(serv.GetMarcaVehiculo()) + " " + serv.GetModeloVehiculo();
            cout << setw(15) << serv.GetPatenteVehiculo();
            cout << setw(10) << (serv.GetTerminado() ? "TERMINADO" : "EN PROCESO");
            cout << setw(15) << serv.GetCantRepuestos() << endl;

            totalRepuestos += serv.GetCantRepuestos();
            totalServicios++;
            encontrado = true;
        }
    }

    if(!encontrado) {
        cout << "No se encontraron servicios para este cliente.\n";
    } else {
        cout << string(90, '-') << endl;
        cout << "TOTAL SERVICIOS: " << totalServicios
             << " | TOTAL REPUESTOS: " << totalRepuestos << endl;
    }
}

void ListarServiciosPorVehiculo(const char* patente) {
    Servicio serv;
    int cant = CantidadRegistrosServicios();
    bool encontrado = false;

   cout << "\nSERVICIOS PARA VEHICULO " << patente << ":\n";
    cout << string(90, '-') << endl;
    cout << setw(8) << "CODIGO" << setw(20) << "CLIENTE" << setw(15) << "MARCA"
         << setw(15) << "MODELO" << setw(40) << "DESCRIPCION" << setw(12) << "ESTADO" << endl;
    cout << string(90, '-') << endl;


    for(int i = 0; i < cant; i++) {
        if(serv.LeerDeDisco(i) && strcmp(serv.GetPatenteVehiculo(), patente) == 0) {
            cout << setw(8) << serv.GetCodigo();
            cout << setw(20) << serv.GetNombreCliente();
            cout << setw(20) << serv.GetModeloVehiculo();
            cout << setw(40) << serv.GetDescripcion();
            cout << setw(12) << (serv.GetTerminado() ? "TERMINADO" : "EN PROCESO") << endl;
            encontrado = true;
        }
    }

    if(!encontrado) {
        cout << "No se encontraron servicios para este vehiculo.\n";
    }
}

void ListarServiciosEnProceso() {
    Servicio serv;
    int cant = CantidadRegistrosServicios();
    bool encontrado = false;

    cout << "\nSERVICIOS EN PROCESO:\n";
    cout << string(120, '-') << endl;
    cout << setw(8) << "CODIGO" << setw(20) << "CLIENTE" << setw(15) << "MARCA"
         << setw(15) << "MODELO" << setw(10) << "PATENTE" << setw(40) << "DESCRIPCION" << endl;
    cout << string(120, '-') << endl;

    for(int i = 0; i < cant; i++) {
        if(serv.LeerDeDisco(i) && !serv.GetTerminado()) {
            cout << setw(8) << serv.GetCodigo();
            cout << setw(20) << string(serv.GetNombreCliente()) + " " + serv.GetApellidoCliente();
            cout << setw(15) << serv.GetMarcaVehiculo();
            cout << setw(15) << serv.GetModeloVehiculo();
            cout << setw(10) << serv.GetPatenteVehiculo();
            cout << setw(40) << serv.GetDescripcion() << endl;
            encontrado = true;
        }
    }

    if(!encontrado) {
        cout << "No hay servicios en proceso actualmente.\n";
    }
    cout << string(120, '-') << endl;
}


void mostrarEncabezadoServicios(int centro_x, int centro_y, int ANCHO_CUADRO) {
    setColor(LIGHTBLUE);
    locate(centro_x, centro_y);
    cout << "╭" << string(ANCHO_CUADRO-2, '─') << "╮";

    locate(centro_x, centro_y+1);
    cout << "│" << string(ANCHO_CUADRO-2, ' ') << "│";

    const string TITULO = " MENÚ DE SERVICIOS ";
    locate(centro_x + (ANCHO_CUADRO - TITULO.length())/2, centro_y+2);
    cout << TITULO;

    locate(centro_x, centro_y+3);
    cout << "│" << string(ANCHO_CUADRO-2, ' ') << "│";

    locate(centro_x, centro_y+4);
    cout << "├" << string(ANCHO_CUADRO-2, '─') << "┤";
}

void MenuServicios() {
    int opcion = 0;
    const int TOTAL_OPCIONES = 7;
    const int ANCHO_CUADRO = 70;

    // Centrado automático
    int centro_x = max(1, (tcols() - ANCHO_CUADRO) / 2);
    int centro_y = max(1, (trows() - 22) / 2);

    const string opciones[] = {
        " 🛠️ NUEVO SERVICIO ",
        " 📋 LISTAR TODOS LOS SERVICIOS ",
        " 👤 BUSCAR POR CLIENTE ",
        " 🚗 BUSCAR POR VEHÍCULO ",
        " ⏳ SERVICIOS EN PROCESO ",
        " ✅ MODIFICAR ESTADO DE SERVICIO ",
        " ↩ VOLVER AL MENÚ PRINCIPAL "
    };

    do {
        cls();
        mostrarEncabezadoServicios(centro_x, centro_y, ANCHO_CUADRO);

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
        locate(centro_x + (ANCHO_CUADRO - instruccion.length())/2, centro_y + 14);
        setColor(GREY);
        cout << instruccion;

        // Marco inferior
        setColor(LIGHTBLUE);
        locate(centro_x, centro_y+16);
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
                    case 0: { // Nuevo servicio
                        mostrarEncabezadoServicios(centro_x, centro_y, ANCHO_CUADRO);

                        Servicio nuevo;
                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "NUEVO SERVICIO";

                        nuevo.Cargar();

                        if(nuevo.GuardarEnDisco()) {
                            locate(centro_x+4, centro_y+20);
                            setColor(LIGHTGREEN);
                            cout << "¡Servicio registrado exitosamente!";
                        } else {
                            locate(centro_x+4, centro_y+20);
                            setColor(LIGHTRED);
                            cout << "Error al guardar el servicio";
                        }
                        break;
                    }

                    case 1: { // Listar todos los servicios
                        mostrarEncabezadoServicios(centro_x, centro_y, ANCHO_CUADRO);

                        int cant = CantidadRegistrosServicios();
                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "LISTADO DE SERVICIOS (" << cant << " registros)";

                        Servicio serv;
                        int fila = centro_y+8;
                        for(int i = 0; i < cant; i++) {
                            if(serv.LeerDeDisco(i)) {
                                locate(centro_x+4, fila);
                                serv.Mostrar();
                                fila += 10; // Ajustar según la altura de Mostrar()
                            }
                        }
                        break;
                    }

                    case 2: { // Buscar por cliente
                        mostrarEncabezadoServicios(centro_x, centro_y, ANCHO_CUADRO);

                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "BUSCAR SERVICIOS POR CLIENTE";

                        char nombre[20], apellido[20];
                        locate(centro_x+4, centro_y+8);
                        cout << "Nombre: ";
                        cin.getline(nombre, 20);
                        locate(centro_x+4, centro_y+9);
                        cout << "Apellido: ";
                        cin.getline(apellido, 20);

                        cls();
                        ListarServiciosPorCliente(nombre, apellido);
                        break;
                    }

                    case 3: { // Buscar por vehículo
                        mostrarEncabezadoServicios(centro_x, centro_y, ANCHO_CUADRO);

                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "BUSCAR SERVICIOS POR VEHÍCULO";

                        char patente[10];
                        locate(centro_x+4, centro_y+8);
                        cout << "Patente: ";
                        cin.getline(patente, 10);

                        cls();
                        ListarServiciosPorVehiculo(patente);
                        break;
                    }

                    case 4: { // Servicios en proceso
                        cls();
                        ListarServiciosEnProceso();
                        break;
                    }

                    case 5: { // Modificar estado de servicio
                        mostrarEncabezadoServicios(centro_x, centro_y, ANCHO_CUADRO);

                        locate(centro_x+4, centro_y+6);
                        setColor(LIGHTCYAN);
                        cout << "MODIFICAR ESTADO DE SERVICIO";

                        int codigo;
                        locate(centro_x+4, centro_y+8);
                        cout << "Código de servicio: ";
                        cin >> codigo;
                        cin.ignore();

                        int pos = -1;
                        Servicio serv;
                        int cant = CantidadRegistrosServicios();
                        for(int i = 0; i < cant; i++) {
                            if(serv.LeerDeDisco(i) && serv.GetCodigo() == codigo) {
                                pos = i;
                                break;
                            }
                        }

                        if(pos == -1) {
                            locate(centro_x+4, centro_y+10);
                            setColor(LIGHTRED);
                            cout << "¡Servicio no encontrado!";
                        } else {
                            locate(centro_x+4, centro_y+10);
                            cout << "Estado actual: " << (serv.GetTerminado() ? "TERMINADO" : "EN PROCESO");

                            locate(centro_x+4, centro_y+12);
                            cout << "¿Marcar como TERMINADO? (s/n): ";
                            char opcion;
                            cin >> opcion;
                            cin.ignore();

                            if(tolower(opcion) == 's') {
                                serv.SetTerminado(true);
                                if(serv.ModificarEnDisco(pos)) {
                                    locate(centro_x+4, centro_y+14);
                                    setColor(LIGHTGREEN);
                                    cout << "¡Estado actualizado correctamente!";
                                } else {
                                    locate(centro_x+4, centro_y+14);
                                    setColor(LIGHTRED);
                                    cout << "Error al actualizar el servicio";
                                }
                            }
                        }
                        break;
                    }

                    case 6: // Volver al menú principal
                        return;
                }

                if(opcion != 6) {
                    locate(centro_x+4, centro_y+20);
                    setColor(WHITE);
                    cout << "Presione cualquier tecla para continuar...";
                    anykey();
                }
                break;
        }
    } while(true);
}
