#ifndef CLIENTES_H_INCLUDED
#define CLIENTES_H_INCLUDED

class Cliente {
private:
    char nombre[10];
    char apellido[10];
    int telefono;
public:
    void SetNombre(const char *n);
    void SetApellido(const char *a);
    void SetTelefono(int t);
    char *getNombre() { return nombre; }
    char *getApellido() { return apellido; }
    int getTelefono() { return telefono; }
    void CargarCliente();
    void MostrarCliente();
};

#endif // CLIENTES_H_INCLUDED
