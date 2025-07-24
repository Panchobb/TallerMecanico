#include "Facturacion.h"
#include "Servicios.h"
#include <iostream>
#include <iomanip>
#include <conio.h>
#include <vector>
#include "rlutil.h"
#include <algorithm>

using namespace std;
using namespace rlutil;
int CantidadRegistrosServicios();

// Implementación de métodos
void Factura::SetFromServicio(const Servicio& serv) {
    idFactura = generarNuevoIdFactura();
    codigoServicio = serv.GetCodigo();
    strncpy(nombreCliente, serv.GetNombreCliente(), sizeof(nombreCliente)-1);
    strncpy(apellidoCliente, serv.GetApellidoCliente(), sizeof(apellidoCliente)-1);
    strncpy(patenteVehiculo, serv.GetPatenteVehiculo(), sizeof(patenteVehiculo)-1);
    strncpy(marcaVehiculo, serv.GetMarcaVehiculo(), sizeof(marcaVehiculo)-1);
    strncpy(modeloVehiculo, serv.GetModeloVehiculo(), sizeof(modeloVehiculo)-1);
    anioVehiculo = serv.GetAnioVehiculo();
    telefonoCliente = serv.GetTelefonoCliente();
    strncpy(descripcionServicio, serv.GetDescripcion(), sizeof(descripcionServicio)-1);
    pagado = false;



    // Asegurar terminación nula
    nombreCliente[sizeof(nombreCliente)-1] = '\0';
    apellidoCliente[sizeof(apellidoCliente)-1] = '\0';
    patenteVehiculo[sizeof(patenteVehiculo)-1] = '\0';
    marcaVehiculo[sizeof(marcaVehiculo)-1] = '\0';
    modeloVehiculo[sizeof(modeloVehiculo)-1] = '\0';
    descripcionServicio[sizeof(descripcionServicio)-1] = '\0';
}

bool Factura::guardarEnDisco() {
    ofstream archivo("facturas.dat", ios::binary | ios::app);
    if(!archivo) return false;
    archivo.write(reinterpret_cast<const char*>(this), sizeof(Factura));
    archivo.close();
    return true;
}

bool Factura::leerDeDisco(int pos) {
    ifstream archivo("facturas.dat", ios::binary);
    if(!archivo) return false;
    archivo.seekg(pos * sizeof(Factura));
    archivo.read(reinterpret_cast<char*>(this), sizeof(Factura));
    archivo.close();
    return archivo.good();
}

void Factura::mostrar() const {
    cout << left;
    cout << "========================================\n";
    cout << "          FACTURA #" << idFactura << endl;
    cout << "========================================\n";
    cout << setw(15) << "Fecha:";
    fechaFactura.Mostrar();
    cout << endl;
    cout << setw(15) << "Servicio:" << "SER-" << codigoServicio << endl;
    cout << setw(15) << "Cliente:" << nombreCliente << " " << apellidoCliente << endl;
    cout << setw(15) << "Teléfono:" << telefonoCliente << endl;
    cout << setw(15) << "Vehículo:" << marcaVehiculo << " " << modeloVehiculo
         << " (" << anioVehiculo << ") - " << patenteVehiculo << endl;
    cout << setw(15) << "Descripción:" << descripcionServicio << endl;
    cout << setw(15) << "Monto base:" << "$" << fixed << setprecision(2) << monto << endl;
    cout << setw(15) << "Monto final:" << "$" << fixed << setprecision(2) << getMontoFinal();
    cout << " (" << getMedioPagoStr() << ")" << endl;
    cout << setw(15) << "Estado:" << (pagado ? "PAGADO" : "PENDIENTE") << endl;
    cout << "========================================\n";
}

// Funciones globales
int generarNuevoIdFactura() {
    ifstream archivo("facturas.dat", ios::binary | ios::ate);
    if(archivo.is_open() && archivo.tellg() > 0) {
        int cant = archivo.tellg() / sizeof(Factura);
        archivo.close();
        return cant + 1;
    }
    return 1;
}

int cantidadRegistrosFacturas() {
    ifstream archivo("facturas.dat", ios::binary | ios::ate);
    if(!archivo) return 0;
    int tam = archivo.tellg();
    archivo.close();
    return tam / sizeof(Factura);
}

void listarFacturasPorCliente(const char* nombre, const char* apellido) {
    system("cls");
    Factura fac;
    int cant = cantidadRegistrosFacturas();
    bool encontrado = false;
    float totalMonto = 0.0f;

    cout << "\nFACTURAS PARA " << nombre << " " << apellido << ":\n";
    cout << setw(10) << "FACTURA" << setw(12) << "SERVICIO" << setw(20) << "VEHICULO"
         << setw(15) << "MONTO" << setw(15) << "FECHA" << setw(12) << "ESTADO" << endl;
    cout << string(90, '-') << endl;

    for(int i = 0; i < cant; i++) {
        if(fac.leerDeDisco(i) &&
           strcasecmp(fac.getNombreCliente(), nombre) == 0 &&
           strcasecmp(fac.getApellidoCliente(), apellido) == 0) {

            char fechaStr[12];
            fac.getFechaFactura().ToString(fechaStr);

            cout << setw(10) << fac.getIdFactura();
            cout << setw(12) << fac.getCodigoServicio();
            cout << setw(20) << fac.getPatenteVehiculo();
            cout << setw(15) << "$" << fixed << setprecision(2) << fac.getMontoFinal();
            cout << setw(15) << fechaStr;
            cout << setw(12) << (fac.getPagado() ? "PAGADO" : "PENDIENTE") << endl;

            totalMonto += fac.getMontoFinal();
            encontrado = true;
        }
    }

    if(!encontrado) {
        cout << "No se encontraron facturas para este cliente.\n";
    } else {
        cout << string(90, '-') << endl;
        cout << "TOTAL: $" << fixed << setprecision(2) << totalMonto << endl;
    }
}
void listarFacturasPorRangoFechas(const Fecha& fechaInicio, const Fecha& fechaFin) {
    system("cls");
    Factura fac;
    int cant = cantidadRegistrosFacturas();
    bool encontrado = false;
    float totalMonto = 0.0f;

    cout << "\nFACTURAS ENTRE ";
    fechaInicio.Mostrar();
    cout << " Y ";
    fechaFin.Mostrar();
    cout << ":\n";
    cout << setw(10) << "FACTURA" << setw(25) << "CLIENTE" << setw(20) << "VEHICULO"
         << setw(15) << "MONTO" << setw(15) << "FECHA" << setw(12) << "ESTADO" << endl;
    cout << string(100, '-') << endl;

    for(int i = 0; i < cant; i++) {
        if(fac.leerDeDisco(i)) {
            Fecha fechaFac = fac.getFechaFactura();

            // Comparar fechas
            bool mayorIgualInicio = (fechaFac.getAnio() > fechaInicio.getAnio()) ||
                                  (fechaFac.getAnio() == fechaInicio.getAnio() && fechaFac.getMes() > fechaInicio.getMes()) ||
                                  (fechaFac.getAnio() == fechaInicio.getAnio() && fechaFac.getMes() == fechaInicio.getMes() && fechaFac.getDia() >= fechaInicio.getDia());

            bool menorIgualFin = (fechaFac.getAnio() < fechaFin.getAnio()) ||
                                (fechaFac.getAnio() == fechaFin.getAnio() && fechaFac.getMes() < fechaFin.getMes()) ||
                                (fechaFac.getAnio() == fechaFin.getAnio() && fechaFac.getMes() == fechaFin.getMes() && fechaFac.getDia() <= fechaFin.getDia());

            if(mayorIgualInicio && menorIgualFin) {
                char fechaStr[12];
                fac.getFechaFactura().ToString(fechaStr);

                cout << setw(10) << fac.getIdFactura();
                cout << setw(25) << string(fac.getNombreCliente()) + " " + fac.getApellidoCliente();
                cout << setw(20) << fac.getPatenteVehiculo();
                cout << setw(15) << "$" << fixed << setprecision(2) << fac.getMontoFinal();
                cout << setw(15) << fechaStr;
                cout << setw(12) << (fac.getPagado() ? "PAGADO" : "PENDIENTE") << endl;

                totalMonto += fac.getMontoFinal();
                encontrado = true;
            }
        }
    }

    if(!encontrado) {
        cout << "No se encontraron facturas en este rango de fechas.\n";
    } else {
        cout << string(100, '-') << endl;
        cout << "TOTAL FACTURADO: $" << fixed << setprecision(2) << totalMonto << endl;
    }
}
void listarFacturasPorVehiculo(const char* patente) {
    system("cls");
    Factura fac;
    int cant = cantidadRegistrosFacturas();
    bool encontrado = false;
    float totalMonto = 0.0f;

    cout << "\nFACTURAS PARA VEHICULO " << patente << ":\n";
    cout << setw(10) << "FACTURA" << setw(25) << "CLIENTE" << setw(12) << "SERVICIO"
         << setw(15) << "MONTO" << setw(15) << "FECHA" << setw(12) << "ESTADO" << endl;
    cout << string(90, '-') << endl;

    for(int i = 0; i < cant; i++) {
        if(fac.leerDeDisco(i) && strcasecmp(fac.getPatenteVehiculo(), patente) == 0) {
            char fechaStr[12];
            fac.getFechaFactura().ToString(fechaStr);

            cout << setw(10) << fac.getIdFactura();
            cout << setw(25) << string(fac.getNombreCliente()) + " " + fac.getApellidoCliente();
            cout << setw(12) << fac.getCodigoServicio();
            cout << setw(15) << "$" << fixed << setprecision(2) << fac.getMontoFinal();
            cout << setw(15) << fechaStr;
            cout << setw(12) << (fac.getPagado() ? "PAGADO" : "PENDIENTE") << endl;

            totalMonto += fac.getMontoFinal();
            encontrado = true;
        }
    }

    if(!encontrado) {
        cout << "No se encontraron facturas para este vehículo.\n";
    } else {
        cout << string(90, '-') << endl;
        cout << "TOTAL: $" << fixed << setprecision(2) << totalMonto << endl;
    }
}

void listarFacturasPagadas() {
    system("cls");
    Factura fac;
    int cant = cantidadRegistrosFacturas();
    int totalFacturas = 0;
    float totalMonto = 0.0f;

    cout << "FACTURAS PAGADAS:\n";
    cout << setw(10) << "FACTURA" << setw(25) << "CLIENTE" << setw(20) << "VEHICULO"
         << setw(15) << "MONTO" << setw(15) << "FECHA" << endl;
    cout << string(90, '-') << endl;

    for(int i = 0; i < cant; i++) {
        if(fac.leerDeDisco(i) && fac.getPagado()) {
            cout << setw(10) << fac.getIdFactura();
            cout << setw(25) << string(fac.getNombreCliente()) + " " + fac.getApellidoCliente();
            cout << setw(20) << fac.getPatenteVehiculo();
            cout << setw(15) << "$" << fixed << setprecision(2) << fac.getMontoFinal();

            char fechaStr[12];
            fac.getFechaFactura().ToString(fechaStr);
            cout << setw(15) << fechaStr << endl;

            totalFacturas++;
            totalMonto += fac.getMontoFinal();
        }
    }

    if(totalFacturas == 0) {
        cout << "No hay facturas pagadas registradas.\n";
    } else {
        cout << string(90, '-') << endl;
        cout << "TOTAL FACTURAS: " << totalFacturas
             << " | MONTO TOTAL: $" << fixed << setprecision(2) << totalMonto << endl;
    }
}
void mostrarEncabezadoFacturacion(int centro_x, int centro_y, int ANCHO_CUADRO) {
    setColor(WHITE);
    locate(centro_x, centro_y);
    cout << "╭" << string(ANCHO_CUADRO-2, '─') << "╮";

    locate(centro_x, centro_y+1);
    cout << "│" << string(ANCHO_CUADRO-2, ' ') << "│";

    const string TITULO = " MENÚ DE FACTURACIÓN ";
    locate(centro_x + (ANCHO_CUADRO - TITULO.length())/2, centro_y+2);
    cout << TITULO;

    locate(centro_x, centro_y+3);
    cout << "│" << string(ANCHO_CUADRO-2, ' ') << "│";

    locate(centro_x, centro_y+4);
    cout << "├" << string(ANCHO_CUADRO-2, '─') << "┤";
}

void mostrarPieFacturacion(int centro_x, int centro_y, int ANCHO_CUADRO) {
    locate(centro_x, centro_y+18);
    cout << "│" << string(ANCHO_CUADRO-2, ' ') << "│";

    locate(centro_x, centro_y+19);
    cout << "╰" << string(ANCHO_CUADRO-2, '─') << "╯";
}

void menuFacturacion() {
    int opcion = 0;
    const int TOTAL_OPCIONES = 6;
    const int ANCHO_CUADRO = 60;

    // Centrado automático
    int centro_x = max(1, (tcols() - ANCHO_CUADRO) / 2);
    int centro_y = max(1, (trows() - 22) / 2);

    const string opciones[] = {
        " 🧾 GENERAR NUEVA FACTURA ",
        " 🔍 BUSCAR POR CLIENTE ",
        " 🚗 BUSCAR POR VEHICULO ",
        " 💰 FACTURAS PAGADAS ",
         " 📅 FACTURAS POR FECHAS ",
        " ↩ VOLVER AL MENÚ PRINCIPAL "
    };

    do {
        cls();
        mostrarEncabezadoFacturacion(centro_x, centro_y, ANCHO_CUADRO);

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
        locate(centro_x + (ANCHO_CUADRO - instruccion.length())/2, centro_y + 12);
        setColor(GREY);
        cout << instruccion;

        mostrarPieFacturacion(centro_x, centro_y, ANCHO_CUADRO);

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
                    case 0: { // Generar nueva factura
                        mostrarEncabezadoFacturacion(centro_x, centro_y, ANCHO_CUADRO);

                        locate(centro_x+2, centro_y+6);
                        setColor(WHITE);
                        cout << "SERVICIOS TERMINADOS PENDIENTES DE FACTURACIÓN:";

                        Servicio serv;
                        int cantServ = CantidadRegistrosServicios();
                        vector<int> serviciosPendientes;

                        // Mostrar servicios pendientes
                        int fila = centro_y+8;
                        for(int i = 0; i < cantServ; i++) {
                            if(serv.LeerDeDisco(i) && serv.GetTerminado()) {
                                bool facturado = false;
                                Factura fac;
                                int cantFac = cantidadRegistrosFacturas();

                                for(int j = 0; j < cantFac; j++) {
                                    if(fac.leerDeDisco(j) && fac.getCodigoServicio() == serv.GetCodigo()) {
                                        facturado = true;
                                        break;
                                    }
                                }

                                if(!facturado) {
                                    locate(centro_x+4, fila++);
                                    cout << " " << serv.GetCodigo() << " - "
                                         << serv.GetNombreCliente() << " " << serv.GetApellidoCliente()
                                         << " (" << serv.GetPatenteVehiculo() << ")";
                                    serviciosPendientes.push_back(serv.GetCodigo());
                                }
                            }
                        }

                        if(serviciosPendientes.empty()) {
                            locate(centro_x+4, fila);
                            setColor(LIGHTRED);
                            cout << "No hay servicios pendientes de facturar.";
                        } else {
                            locate(centro_x+4, fila+2);
                            setColor(WHITE);
                            cout << "Ingrese código de servicio a facturar (0 para cancelar): ";

                            int codigo;
                            cin >> codigo;
                            cin.ignore();

                            if(codigo != 0 && find(serviciosPendientes.begin(), serviciosPendientes.end(), codigo) != serviciosPendientes.end()) {
                                Servicio servFacturar;
                                bool encontrado = false;

                                for(int i = 0; i < cantServ; i++) {
                                    if(servFacturar.LeerDeDisco(i) && servFacturar.GetCodigo() == codigo) {
                                        encontrado = true;
                                        cls();
                                        mostrarEncabezadoFacturacion(centro_x, centro_y, ANCHO_CUADRO);

                                        // Mostrar datos del servicio
                                        locate(centro_x+2, centro_y+6);
                                        setColor(LIGHTCYAN);
                                        cout << "DATOS DEL SERVICIO:";
                                        locate(centro_x+4, centro_y+8);
                                        cout << "Cliente: " << servFacturar.GetNombreCliente() << " " << servFacturar.GetApellidoCliente();
                                        locate(centro_x+4, centro_y+9);
                                        cout << "Vehículo: " << servFacturar.GetMarcaVehiculo() << " " << servFacturar.GetModeloVehiculo();
                                        locate(centro_x+4, centro_y+10);
                                        cout << "Patente: " << servFacturar.GetPatenteVehiculo();
                                        locate(centro_x+4, centro_y+11);
                                        cout << "Descripción: " << servFacturar.GetDescripcion();

                                        // Ingresar datos de factura
                                        locate(centro_x+2, centro_y+13);
                                        setColor(LIGHTCYAN);
                                        cout << "DATOS DE FACTURA:";

                                        Factura nueva;
                                        nueva.SetFromServicio(servFacturar);
                                        nueva.setTelefonoCliente(servFacturar.GetTelefonoCliente());

                                        // Fecha
                                        locate(centro_x+4, centro_y+15);
                                        cout << "Fecha (DD MM AAAA): ";
                                        Fecha fechaFactura;
                                        fechaFactura.Cargar();
                                        nueva.setFecha(fechaFactura);

                                        // Monto
                                        locate(centro_x+4, centro_y+16);
                                        cout << "Monto: $";
                                        float monto;
                                        while(!(cin >> monto) || monto <= 0) {
                                            cin.clear();
                                            cin.ignore(1000, '\n');
                                            locate(centro_x+4, centro_y+17);
                                            setColor(LIGHTRED);
                                            cout << "Monto inválido. Ingrese un valor positivo: $";
                                            locate(centro_x+11, centro_y+16);
                                            cout << "          ";
                                            locate(centro_x+11, centro_y+16);
                                            setColor(WHITE);
                                        }
                                        nueva.setMonto(monto);
                                        cin.ignore();

                                        // Medio de pago
                                        locate(centro_x+4, centro_y+17);
                                        cout << "Medio de pago (1-Efectivo, 2-Transferencia): ";
                                        int mp;
                                        while(!(cin >> mp) || (mp != 1 && mp != 2)) {
                                            cin.clear();
                                            cin.ignore(1000, '\n');
                                            locate(centro_x+4, centro_y+18);
                                            setColor(LIGHTRED);
                                            cout << "Opción inválida. Ingrese 1 o 2: ";
                                            locate(centro_x+48, centro_y+17);
                                            cout << "  ";
                                            locate(centro_x+48, centro_y+17);
                                            setColor(WHITE);
                                        }
                                        nueva.setMedioPago(mp == 1 ? EFECTIVO : TRANSFERENCIA);
                                        cin.ignore();

                                        // Pagado
                                        locate(centro_x+4, centro_y+18);
                                        cout << "Marcar como pagado? (s/n): ";
                                        char op;
                                        cin >> op;
                                        cin.ignore();
                                        nueva.setPagado(tolower(op) == 's');

                                        if(nueva.guardarEnDisco()) {
                                            cls();
                                            mostrarEncabezadoFacturacion(centro_x, centro_y, ANCHO_CUADRO);
                                            locate(centro_x+4, centro_y+6);
                                            setColor(LIGHTGREEN);
                                            cout << "FACTURA GENERADA EXITOSAMENTE!";
                                            locate(centro_x+4, centro_y+8);
                                            cout << "Número: #" << nueva.getIdFactura();
                                            locate(centro_x+4, centro_y+9);
                                            cout << "Monto final: $" << fixed << setprecision(2) << nueva.getMontoFinal();
                                            locate(centro_x+4, centro_y+10);
                                            cout << "Fecha: ";
                                            fechaFactura.Mostrar();
                                        } else {
                                            locate(centro_x+4, centro_y+18);
                                            setColor(LIGHTRED);
                                            cout << "Error al generar factura!";
                                        }
                                        break;
                                    }
                                }
                            }
                        }
                        break;
                    }

                    case 1: { // Buscar por cliente
                        mostrarEncabezadoFacturacion(centro_x, centro_y, ANCHO_CUADRO);

                        locate(centro_x+4, centro_y+6);
                        setColor(WHITE);
                        cout << "BUSCAR FACTURAS POR CLIENTE";

                        char nombre[20], apellido[20];
                        locate(centro_x+4, centro_y+8);
                        cout << "Nombre: ";
                        cin.getline(nombre, 20);
                        locate(centro_x+4, centro_y+9);
                        cout << "Apellido: ";
                        cin.getline(apellido, 20);

                        cls();
                        listarFacturasPorCliente(nombre, apellido);
                        break;
                    }

                    case 2: { // Buscar por vehículo
                        mostrarEncabezadoFacturacion(centro_x, centro_y, ANCHO_CUADRO);

                        locate(centro_x+4, centro_y+6);
                        setColor(WHITE);
                        cout << "BUSCAR FACTURAS POR VEHÍCULO";

                        char patente[10];
                        locate(centro_x+4, centro_y+8);
                        cout << "Patente: ";
                        cin.getline(patente, 10);

                        cls();
                        listarFacturasPorVehiculo(patente);
                        break;
                    }

                    case 3: { // Facturas pagadas
                        cls();
                        listarFacturasPagadas();
                        break;
                    }
                    case 4: { // Facturas por rango de fechas
    mostrarEncabezadoFacturacion(centro_x, centro_y, ANCHO_CUADRO);

    locate(centro_x+4, centro_y+6);
    setColor(WHITE);
    cout << "BUSCAR FACTURAS POR RANGO DE FECHAS";

    Fecha fechaInicio, fechaFin;

    locate(centro_x+4, centro_y+8);
    cout << "FECHA INICIAL (DD MM AAAA): ";
    fechaInicio.Cargar();

    locate(centro_x+4, centro_y+10);
    cout << "FECHA FINAL (DD MM AAAA): ";
    fechaFin.Cargar();

    cls();
    listarFacturasPorRangoFechas(fechaInicio, fechaFin);
    break;
}

                    case 5: // Volver al menú principal
                        return;
                }

                if(opcion != 5) {
                    locate(centro_x+4, centro_y+20);
                    setColor(WHITE);
                    cout << "Presione cualquier tecla para continuar...";
                    anykey();
                }
                break;
        }
    } while(true);
}
