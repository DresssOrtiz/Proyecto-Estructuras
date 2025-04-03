#ifndef ARBOLHUFFMAN_H
#define ARBOLHUFFMAN_H

#include <string>
#include <vector>
#include "NodoHuffman.h"

using namespace std;

class ArbolHuffman {
private:
    NodoHuffman* raiz;
    vector<string> codigos;  // codigos[i] contiene el código de la intensidad i

    // Función auxiliar para generar los códigos desde el árbol
    void generarCodigosAux(NodoHuffman* nodo, string camino);

public:
    // Constructor
    ArbolHuffman();

    // Operaciones del TAD
    void construirDesdeFrecuencias(unsigned long frecuencias[], int tamano);
    void generarCodigos();
    string obtenerCodigo(int intensidad) const;
    NodoHuffman* obtenerRaiz() const;
    vector<string> obtenerCodigos() const;
    void fijarRaiz(NodoHuffman* xRaiz);
    void fijarCodigos(const vector<string>& nuevosCodigos);
};

#endif
