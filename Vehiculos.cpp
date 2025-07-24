#include <iostream>
#include <conio.h>
#include <cstring>  // Necesario para strncpy
#include "Vehiculos.h"
using namespace std;

void Vehiculo::CargarVehiculo()
{
    cout << "Patente: ";
    cin.getline(patente, 10);
    cout << "Marca: ";
    cin.getline(marca, 20);
    cout << "Modelo: ";
    cin.getline(modelo, 20);
    cout << "Anio: ";
    cin >> anio;
    cin.ignore();  // Limpiar el buffer
}

void Vehiculo::MostrarVehiculo()
{
    cout << "Patente: " << patente << endl;
    cout << "Marca: " << marca << endl;
    cout << "Modelo: " << modelo << endl;
    cout << "Anio: " << anio << endl;
}

// Implementación de los métodos Set corregidos
void Vehiculo::SetPatente(const char *p) {
    strncpy(patente, p, 9);
    patente[9] = '\0';  // Aseguramos terminación nula
}

void Vehiculo::SetMarca(const char *m) {
    strncpy(marca, m, 19);
    marca[19] = '\0';  // Aseguramos terminación nula
}

void Vehiculo::SetModelo(const char *mod) {
    strncpy(modelo, mod, 19);
    modelo[19] = '\0';  // Aseguramos terminación nula
}

void Vehiculo::SetAnio(int a) {
    anio = a;
}
