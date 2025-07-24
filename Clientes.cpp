#include <iostream>
#include <conio.h>
#include "Clientes.h"
using namespace std;

void Cliente::CargarCliente()
{
    cout << "Nombre: ";
    cin.getline(nombre, 10);  // Mejor usar getline para capturar strings con espacios
    cout << "Apellido: ";
    cin.getline(apellido, 10);
    cout << "Telefono: ";
    cin >> telefono;
    cin.ignore();  // Limpiar el buffer del teclado después de leer el número
}

void Cliente::MostrarCliente()
{
    cout << "Nombre: " << nombre << endl;
    cout << "Apellido: " << apellido << endl;
    cout << "Telefono: " << telefono << endl;
}
