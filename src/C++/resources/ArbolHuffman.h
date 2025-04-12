#ifndef ARBOLHUFFMAN_H
#define ARBOLHUFFMAN_H

#include <string>
#include <vector>
#include "NodoHuffman.h"

using namespace std;

class ArbolHuffman {
private:
    // Datos Mínimos
    NodoHuffman* raiz;              
    vector<string> codigos;          

    // Función auxiliar (privada)
    void generarCodigosAux(NodoHuffman* nodo, string camino);

public:
    // Constructor
    ArbolHuffman();

    // Operaciones
    NodoHuffman* obtenerRaiz();                            
    vector<string> obtenerCodigos();                       
    void fijarRaiz(NodoHuffman* xRaiz);                   
    void fijarCodigos(vector<string> xCodigos);            
    void construirDesdeFrecuencias(unsigned long xFrecuencias[], int tamano); 
    void generarCodigos();                                 
    string obtenerCodigo(int xIntensidad);                 
    vector<vector<int>> decodificarBits(string xBits, int xAncho, int xAlto); 
};

#endif
