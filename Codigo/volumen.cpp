#include "volumen.h"

// Métodos de la clase Volumen
string Volumen::obtenerNombre_volumen() { return nombre_volumen; }
vector<Imagen> Volumen::obtenerImagenes() { return imagenes; }
vector<Imagen> Volumen::obtenerProyecciones() { return proyecciones; }
void Volumen::fijarNombre_volumen(string xNombre_volumen) { nombre_volumen = xNombre_volumen; }
void Volumen::fijarImagenes(const vector<Imagen>& xImagenes) { imagenes = xImagenes; }
void Volumen::fijarProyecciones(const vector<Imagen>& xProyecciones) { proyecciones = xProyecciones; }
