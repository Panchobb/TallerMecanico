#ifndef REPUESTOS_H_INCLUDED
#define REPUESTOS_H_INCLUDED

#include <iostream>
#include <fstream>
#include <cstring>

class Repuesto {
private:
    char codigo[10];
    char marca[20];
    char modelo[20];
    char descripcion[100];
    int cantidad;
public:
    // Setters
    void SetCodigo(const char* c);
    void SetMarca(const char* m);
    void SetModelo(const char* mod);
    void SetDescripcion(const char* d);
    void SetCantidad(int c);

    // Getters
    const char* GetCodigo() { return codigo; }
    const char* GetMarca() { return marca; }
    const char* GetModelo() { return modelo; }
    const char* GetDescripcion() { return descripcion; }
    int GetCantidad() { return cantidad; }

    // Funciones de disco
    bool GuardarEnDisco();
    bool LeerDeDisco(int pos);
    bool ModificarEnDisco(int pos);

    // Interfaz
    void Cargar();
    void Mostrar();
};

// Funciones globales
int CantidadRegistrosRepuestos();
bool BuscarRepuesto(const char* codigo, Repuesto& rep);
void MenuRepuestos();
void AsociarRepuestosAServicio();

#endif // REPUESTOS_H_INCLUDED
