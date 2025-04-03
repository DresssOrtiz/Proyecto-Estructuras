#ifndef ARBOLHUFFMAN_H
#define ARBOLHUFFMAN_H

#include <string>
#include <vector>
#include "NodoHuffman.h"

using namespace std;

class ArbolHuffman {
private:
    // Datos Mínimos
    NodoHuffman* raiz;               // raíz del árbol de Huffman
    vector<string> codigos;          // vector de códigos Huffman por intensidad

    // Función auxiliar (privada)
    void generarCodigosAux(NodoHuffman* nodo, string camino);

public:
    // Constructor
    ArbolHuffman();

    // Operaciones
    NodoHuffman* obtenerRaiz();                            // da la raíz del árbol
    vector<string> obtenerCodigos();                       // da el vector de códigos
    void fijarRaiz(NodoHuffman* xRaiz);                    // asigna la raíz
    void fijarCodigos(vector<string> xCodigos);            // asigna el vector de códigos
    void construirDesdeFrecuencias(unsigned long xFrecuencias[], int tamano); // construye el árbol
    void generarCodigos();                                 // genera códigos y llena el vector
    string obtenerCodigo(int xIntensidad);                 // da el código de una intensidad
    vector<vector<int>> decodificarBits(string xBits, int xAncho, int xAlto); // reconstruye la imagen
};

#endif
