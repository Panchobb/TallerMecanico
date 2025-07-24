#ifndef SERVICIOS_H_INCLUDED
#define SERVICIOS_H_INCLUDED

#include "Clientes.h"
#include "Vehiculos.h"
#include "Repuestos.h"
#include "Fecha.h"
#include <iostream>
#include <fstream>
#include <cstring>

class Servicio : public Fecha {
private:
    int codigo;
    char descripcion[100];
    char nombreCliente[20];
    char apellidoCliente[20];
    char marcaVehiculo[20];
    char modeloVehiculo[20];
    int telefonoCliente;
    int anioVehiculo;
    char patenteVehiculo[10];
    bool terminado;
    char repuestos[5][10];
    int cantidades[5];
    int cantRepuestos;

public:
    // Setters
    void SetTelefonoCliente(int tel) { telefonoCliente = tel; }
    int GetTelefonoCliente() const { return telefonoCliente; }
    void SetCodigo(int c) { codigo = c; }
    void SetDescripcion(const char* d) { strncpy(descripcion, d, sizeof(descripcion)-1); descripcion[sizeof(descripcion)-1] = '\0'; }
    void SetNombreCliente(const char* nom) { strncpy(nombreCliente, nom, sizeof(nombreCliente)-1); nombreCliente[sizeof(nombreCliente)-1] = '\0'; }
    void SetApellidoCliente(const char* ape) { strncpy(apellidoCliente, ape, sizeof(apellidoCliente)-1); apellidoCliente[sizeof(apellidoCliente)-1] = '\0'; }
    void SetMarcaVehiculo(const char* marca) { strncpy(marcaVehiculo, marca, sizeof(marcaVehiculo)-1); marcaVehiculo[sizeof(marcaVehiculo)-1] = '\0'; }
    void SetModeloVehiculo(const char* modelo) { strncpy(modeloVehiculo, modelo, sizeof(modeloVehiculo)-1); modeloVehiculo[sizeof(modeloVehiculo)-1] = '\0'; }
    void SetAnioVehiculo(int anio) { anioVehiculo = anio; }
    void SetPatenteVehiculo(const char* pat) { strncpy(patenteVehiculo, pat, sizeof(patenteVehiculo)-1); patenteVehiculo[sizeof(patenteVehiculo)-1] = '\0'; }
    void SetTerminado(bool t) { terminado = t; }

    // Getters
     Fecha& GetFecha() { return *this; }  // Devuelve la referencia al objeto Fecha base
    const Fecha& GetFecha() const { return *this; }
    int GetCodigo() const { return codigo; }
    const char* GetDescripcion() const { return descripcion; }
    const char* GetNombreCliente() const { return nombreCliente; }
    const char* GetApellidoCliente() const { return apellidoCliente; }
    const char* GetMarcaVehiculo() const { return marcaVehiculo; }
    const char* GetModeloVehiculo() const { return modeloVehiculo; }
    int GetAnioVehiculo() const { return anioVehiculo; }
    const char* GetPatenteVehiculo() const { return patenteVehiculo; }
    bool GetTerminado() const { return terminado; }
    int GetCantRepuestos() const { return cantRepuestos; }
    const char* GetRepuesto(int index) const {
        if (index < 0 || index >= cantRepuestos) return "";
        return repuestos[index];
    }
    int GetCantidadRepuesto(int index) const {
        if (index < 0 || index >= cantRepuestos) return 0;
        return cantidades[index];
    }

    // Métodos de repuestos
    bool AgregarRepuesto(const char* codigo, int cantidad = 1);
    bool VerificarStockRepuesto(const char* codigoRepuesto, int cantidad = 1);
    bool ReducirStockRepuesto(const char* codigoRepuesto, int cantidad = 1);
    void MostrarRepuestos() const;

    // Métodos de archivo
    bool GuardarEnDisco();
    bool LeerDeDisco(int pos);
    bool ModificarEnDisco(int pos);

    // Métodos de interfaz
    void Cargar();
    void Mostrar() const;
};

// Funciones globales
int GenerarNuevoCodigoServicio();
int CantidadRegistrosServicios();
void ListarServiciosPorCliente(const char* nombre, const char* apellido);
void ListarServiciosPorVehiculo(const char* patente);
void ListarServiciosEnProceso();
void MenuServicios();

#endif // SERVICIOS_H_INCLUDED
