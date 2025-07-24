#ifndef FACTURACION_H_INCLUDED
#define FACTURACION_H_INCLUDED

#include "Servicios.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <vector>

enum MedioPago { EFECTIVO, TRANSFERENCIA };

class Factura : public Fecha {
private:
    int idFactura;
    int codigoServicio;
    char nombreCliente[20];
    char apellidoCliente[20];
    char patenteVehiculo[10];
    char marcaVehiculo[20];
    char modeloVehiculo[20];
    int anioVehiculo;
    int telefonoCliente;
    char descripcionServicio[100];
    float monto;
    Fecha fechaFactura;
    MedioPago medioPago;
    bool pagado;

public:
    // Setters
    void setTelefonoCliente(int tel) { telefonoCliente = tel; }
    void setFecha(Fecha fn) { fechaFactura = fn; }
    void setNombreCliente(const char* n) { strncpy(nombreCliente, n, sizeof(nombreCliente)-1); nombreCliente[sizeof(nombreCliente)-1] = '\0'; }
    void setApellidoCliente(const char* a) { strncpy(apellidoCliente, a, sizeof(apellidoCliente)-1); apellidoCliente[sizeof(apellidoCliente)-1] = '\0'; }
    void setPatenteVehiculo(const char* p) { strncpy(patenteVehiculo, p, sizeof(patenteVehiculo)-1); patenteVehiculo[sizeof(patenteVehiculo)-1] = '\0'; }
    void setMarcaVehiculo(const char* m) { strncpy(marcaVehiculo, m, sizeof(marcaVehiculo)-1); marcaVehiculo[sizeof(marcaVehiculo)-1] = '\0'; }
    void setModeloVehiculo(const char* mo) { strncpy(modeloVehiculo, mo, sizeof(modeloVehiculo)-1); modeloVehiculo[sizeof(modeloVehiculo)-1] = '\0'; }
    void setAnioVehiculo(int an) { anioVehiculo = an; }
    void setDescripcionServicio(const char* d) { strncpy(descripcionServicio, d, sizeof(descripcionServicio)-1); descripcionServicio[sizeof(descripcionServicio)-1] = '\0'; }
    void setMonto(float m) { monto = m; }
    void setMedioPago(MedioPago mp) { medioPago = mp; }
    void setPagado(bool p) { pagado = p; }
    void setCodigoServicio(int cod) { codigoServicio = cod; }
    void setIdFactura(int id) { idFactura = id; }

    // Método para inicializar desde un servicio
    void SetFromServicio(const Servicio& serv);

    // Getters
    int getIdFactura() const { return idFactura; }
    int getCodigoServicio() const { return codigoServicio; }
    const char* getNombreCliente() const { return nombreCliente; }
    const char* getApellidoCliente() const { return apellidoCliente; }
    const char* getPatenteVehiculo() const { return patenteVehiculo; }
    const char* getMarcaVehiculo() const { return marcaVehiculo; }
    const char* getModeloVehiculo() const { return modeloVehiculo; }
    int getAnioVehiculo() const { return anioVehiculo; }
    int getTelefonoCliente() const { return telefonoCliente; }
    const char* getDescripcionServicio() const { return descripcionServicio; }
    float getMonto() const { return monto; }
    Fecha getFechaFactura() const { return fechaFactura; }
    MedioPago getMedioPago() const { return medioPago; }
    bool getPagado() const { return pagado; }

    float getMontoFinal() const {
        return medioPago == EFECTIVO ? monto * 0.9f : monto * 1.1f;
    }
    const char* getMedioPagoStr() const {
        return medioPago == EFECTIVO ? "Efectivo (10% desc)" : "Transferencia (10% rec)";
    }

    bool guardarEnDisco();
    bool leerDeDisco(int pos);
    void mostrar() const;
};
void listarFacturasPorRangoFechas(const Fecha& fechaInicio, const Fecha& fechaFin);
int generarNuevoIdFactura();
int cantidadRegistrosFacturas();
void listarFacturasPorCliente(const char* nombre, const char* apellido);
void listarFacturasPorVehiculo(const char* patente);
void listarFacturasPagadas();
void menuFacturacion();

#endif // FACTURACION_H_INCLUDED
