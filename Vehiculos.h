#ifndef VEHICULO_H_INCLUDED
#define VEHICULO_H_INCLUDED

class Vehiculo {
private:
    char patente[10];
    char marca[20];
    char modelo[20];
    int anio;
public:
    void SetPatente(const char *p);
    void SetMarca(const char *m);
    void SetModelo(const char *mod);
    void SetAnio(int a);
    char* getPatente() { return patente; }
    char* getMarca() { return marca; }
    char* getModelo() { return modelo; }
    int getAnio() { return anio; }
    void CargarVehiculo();
    void MostrarVehiculo();
};

#endif // VEHICULO_H_INCLUDED
