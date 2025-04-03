#ifndef NODOHUFFMAN_H
#define NODOHUFFMAN_H

using namespace std;

class NodoHuffman {
private:
    // Datos Mínimos
    int intensidad;                  // representa el valor de gris (0 a 255)
    unsigned long frecuencia;        // cuántas veces aparece la intensidad
    NodoHuffman* izquierdo;         // hijo izquierdo del árbol
    NodoHuffman* derecho;           // hijo derecho del árbol

public:
    // Constructor
    NodoHuffman(int xIntensidad, unsigned long xFrecuencia);

    // Operaciones
    int obtenerIntensidad();                             // da la intensidad del nodo
    unsigned long obtenerFrecuencia();                   // da la frecuencia del nodo
    NodoHuffman* obtenerIzquierdo();                     // da el hijo izquierdo
    NodoHuffman* obtenerDerecho();                       // da el hijo derecho
    void fijarIntensidad(int xIntensidad);               // cambia la intensidad
    void fijarFrecuencia(unsigned long xFrecuencia);     // cambia la frecuencia
    void fijarIzquierdo(NodoHuffman* xNodo);             // asigna hijo izquierdo
    void fijarDerecho(NodoHuffman* xNodo);               // asigna hijo derecho
    bool esHoja();                                       // dice si el nodo es hoja
};

#endif
