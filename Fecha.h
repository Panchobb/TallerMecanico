#ifndef FECHA_H_INCLUDED
#define FECHA_H_INCLUDED

#include <iostream>
#include <cstdio> // Para snprintf
using namespace std;
class Fecha {
private:
    int dia;
    int mes;
    int anio;

public:
    // Constructor
    Fecha(int d = 1, int m = 1, int a = 2000) : dia(d), mes(m), anio(a) {}

    // Método para cargar fecha desde entrada
        void Cargar() {
        cout << "Dia: ";
        cin >> dia;
        cout << "Mes: ";
        cin >> mes;
        cout << "Anio: ";
        cin >> anio;
    }

    // Método para mostrar fecha formateada
    void Mostrar() const {
        cout << (dia < 10 ? "0" : "") << dia << "/"
             << (mes < 10 ? "0" : "") << mes << "/"
             << anio;
    }

    // Método para convertir fecha a string (versión con buffer)
    void ToString(char* buffer) const {
        snprintf(buffer, 12, "%02d/%02d/%04d", dia, mes, anio);
    }

    // Getters
    int getDia() const { return dia; }
    int getMes() const { return mes; }
    int getAnio() const { return anio; }

    // Setters
    void setDia(int d) { dia = d; }
    void setMes(int m) { mes = m; }
    void setAnio(int a) { anio = a; }
};

#endif // FECHA_H_INCLUDED
