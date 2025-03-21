#ifndef IMAGEN_H
#define IMAGEN_H

#include <string>
#include <vector>

using namespace std;

class Imagen {
private:
    string nombre_imagen;
    int alto;
    int ancho;
    vector<vector<int>> pixeles;

public:
    // Constructor
    Imagen() : nombre_imagen(""), alto(0), ancho(0) {}

    // Operaciones
    string obtenerNombre_imagen();
    int obtenerAlto();
    int obtenerAncho();
    vector<vector<int>> obtenerPixeles() const { return pixeles; }
    void fijarNombre_imagen(string xNombre_imagen);
    void fijarDimensiones(int xAncho, int xAlto);
    void fijarPixeles(const vector<vector<int>>& xPixeles);
};

#endif
