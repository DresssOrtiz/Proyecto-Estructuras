#include "imagen.h"

// Métodos de la clase Imagen
string Imagen::obtenerNombre_imagen() const { return nombre_imagen; }
int Imagen::obtenerAlto() const{ return alto; }
int Imagen::obtenerAncho() const{ return ancho; }
void Imagen::fijarNombre_imagen(string xNombre_imagen) { nombre_imagen = xNombre_imagen; }
void Imagen::fijarDimensiones(int xAncho, int xAlto) {
    ancho = xAncho;
    alto = xAlto;
    pixeles = vector<vector<int>>(alto, vector<int>(ancho, 0));
}
void Imagen::fijarPixeles(const vector<vector<int>>& xPixeles) { pixeles = xPixeles; }
