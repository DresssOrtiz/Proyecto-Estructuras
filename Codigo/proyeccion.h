#ifndef PROYECCION_H
#define PROYECCION_H

#include "volumen.h"

class Proyeccion {
private:
    string nombre_proyeccion;
    Volumen volumen;
    string direccion;
    string criterio;
    Imagen resultado;

public:
    // Constructor
    Proyeccion() : nombre_proyeccion(""), direccion(""), criterio("") {}

    // Operaciones
    string obtenerNombre_proyeccion();
    Volumen obtenerVolumen();
    string obtenerDireccion();
    string obtenerCriterio();
    Imagen obtenerResultado();
    void fijarNombre_proyeccion(string xNombre_proyeccion);
    void fijarVolumen(Volumen xVolumen);
    void fijarDireccion(string xDireccion);
    void fijarCriterio(string xCriterio);
    void fijarResultado(Imagen xResultado);
};

#endif
