#include "MenuPrincipal.h"
#include "rlutil.h"
#include <iostream>
#include <string>

using namespace std;
using namespace rlutil;

void MenuPrincipal() {
    int opcion = 0;
    const int TOTAL_OPCIONES = 4;
    const int ANCHO_CUADRO = 50;
    const string TITULO = " TALLER MECANICO 'AUTO UTN' ";

    // Centrado automático
    int centro_x = max(1, (tcols() - ANCHO_CUADRO) / 2);
    int centro_y = max(1, (trows() - 15) / 2);

    const string opciones[] = {
        " 🛠️  MENU DE REPUESTOS  ",
        " 🔧 MENU DE SERVICIOS  ",
        " 🧾 MENU DE FACTURACION ",
        " 🚪 SALIR DEL SISTEMA  "
    };

    do {
        cls();

        // ──────────────────────────────────────────────────
        //                 MARCO SIMPLIFICADO
        // ──────────────────────────────────────────────────
        setColor(WHITE);

        // Borde superior
        locate(centro_x, centro_y);
        cout << "╭" << string(ANCHO_CUADRO-2, '─') << "╮";

        // Título centrado
        locate(centro_x + (ANCHO_CUADRO - TITULO.length())/2, centro_y+2);
        cout << TITULO;

        // Separador
        locate(centro_x, centro_y+4);
        cout << "├" << string(ANCHO_CUADRO-2, '─') << "┤";

        // ──────────────────────────────────────────────────
        //                OPCIONES DEL MENU
        // ──────────────────────────────────────────────────
        for (int i = 0; i < TOTAL_OPCIONES; ++i) {
            int opcion_x = centro_x + (ANCHO_CUADRO - opciones[i].length()) / 2;
            locate(opcion_x, centro_y + 6 + i);

            if (i == opcion) {
                setColor(YELLOW);
                cout << "» " << opciones[i] << " «";
            } else {
                setColor(GREY);
                cout << "  " << opciones[i] << "  ";
            }
        }

        // ──────────────────────────────────────────────────
        //                  INSTRUCCIONES
        // ──────────────────────────────────────────────────
        string instruccion = "[ FLECHA ABAJO ; FLECHA ARRIBA ] NAVEGAR   [ ENTER ] SELECCIONAR";
        locate(centro_x + (ANCHO_CUADRO - instruccion.length())/2, centro_y + 11);
        setColor(DARKGREY);
        cout << instruccion;

        // Borde inferior
        setColor(WHITE);
        locate(centro_x, centro_y+13);
        cout << "╰" << string(ANCHO_CUADRO-2, '─') << "╯";

        // ──────────────────────────────────────────────────
        //                LÓGICA DE CONTROL
        // ──────────────────────────────────────────────────
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
                switch(opcion) {
                    case 0: MenuRepuestos(); break;
                    case 1: MenuServicios(); break;
                    case 2: menuFacturacion(); break;
                    case 3:
                        cls();
                        setColor(YELLOW);
                        locate((tcols()-23)/2, trows()/2);
                        cout << "⚡ Cerrando el sistema...";
                        msleep(1200);
                        return;
                }
                break;
        }

    } while (true);
}
