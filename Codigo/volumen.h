#ifndef VOLUMEN_H
#define VOLUMEN_H

#include "imagen.h"
#include <vector>

class Volumen {
private:
    string nombre_volumen;
    vector<Imagen> imagenes;
    vector<Imagen> proyecciones;

public:
    // Constructor
    Volumen() : nombre_volumen("") {}

    // Operaciones
    string obtenerNombre_volumen();
    vector<Imagen> obtenerImagenes();
    vector<Imagen> obtenerProyecciones();
    void fijarNombre_volumen(string xNombre_volumen);
    void fijarImagenes(const vector<Imagen>& xImagenes);
    void fijarProyecciones(const vector<Imagen>& xProyecciones);
};

#endif
