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

// Implementaci�n de los m�todos Set corregidos
void Vehiculo::SetPatente(const char *p) {
    strncpy(patente, p, 9);
    patente[9] = '\0';  // Aseguramos terminaci�n nula
}

void Vehiculo::SetMarca(const char *m) {
    strncpy(marca, m, 19);
    marca[19] = '\0';  // Aseguramos terminaci�n nula
}

void Vehiculo::SetModelo(const char *mod) {
    strncpy(modelo, mod, 19);
    modelo[19] = '\0';  // Aseguramos terminaci�n nula
}

void Vehiculo::SetAnio(int a) {
    anio = a;
}
