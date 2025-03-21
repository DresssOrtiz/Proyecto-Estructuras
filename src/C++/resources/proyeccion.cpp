#include "proyeccion.h"

// Métodos de la clase Proyección
string Proyeccion::obtenerNombre_proyeccion() { return nombre_proyeccion; }
Volumen Proyeccion::obtenerVolumen() { return volumen; }
string Proyeccion::obtenerDireccion() { return direccion; }
string Proyeccion::obtenerCriterio() { return criterio; }
Imagen Proyeccion::obtenerResultado() { return resultado; }
void Proyeccion::fijarNombre_proyeccion(string xNombre_proyeccion) { nombre_proyeccion = xNombre_proyeccion; }
void Proyeccion::fijarVolumen(Volumen xVolumen) { volumen = xVolumen; }
void Proyeccion::fijarDireccion(string xDireccion) { direccion = xDireccion; }
void Proyeccion::fijarCriterio(string xCriterio) { criterio = xCriterio; }
void Proyeccion::fijarResultado(Imagen xResultado) { resultado = xResultado; }
