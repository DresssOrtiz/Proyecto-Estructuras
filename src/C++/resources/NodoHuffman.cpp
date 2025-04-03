#include "NodoHuffman.h"

// Constructor
NodoHuffman::NodoHuffman(int xIntensidad, unsigned long xFrecuencia) {
    intensidad = xIntensidad;
    frecuencia = xFrecuencia;
    izquierdo = 0;
    derecho = 0;
}

// Operaciones

int NodoHuffman::obtenerIntensidad() {
    return intensidad;
}

unsigned long NodoHuffman::obtenerFrecuencia() {
    return frecuencia;
}

NodoHuffman* NodoHuffman::obtenerIzquierdo() {
    return izquierdo;
}

NodoHuffman* NodoHuffman::obtenerDerecho() {
    return derecho;
}

void NodoHuffman::fijarIntensidad(int xIntensidad) {
    intensidad = xIntensidad;
}

void NodoHuffman::fijarFrecuencia(unsigned long xFrecuencia) {
    frecuencia = xFrecuencia;
}

void NodoHuffman::fijarIzquierdo(NodoHuffman* xNodo) {
    izquierdo = xNodo;
}

void NodoHuffman::fijarDerecho(NodoHuffman* xNodo) {
    derecho = xNodo;
}

bool NodoHuffman::esHoja() {
    return (izquierdo == 0 && derecho == 0);
}
