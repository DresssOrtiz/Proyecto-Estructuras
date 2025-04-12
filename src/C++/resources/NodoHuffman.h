#ifndef NODOHUFFMAN_H
#define NODOHUFFMAN_H

using namespace std;

class NodoHuffman {
private:
    // Datos Mínimos
    int intensidad;                 
    unsigned long frecuencia;     
    NodoHuffman* izquierdo;      
    NodoHuffman* derecho;      

public:
    // Constructor
    NodoHuffman(int xIntensidad, unsigned long xFrecuencia);

    // Operaciones
    int obtenerIntensidad();                            
    unsigned long obtenerFrecuencia();              
    NodoHuffman* obtenerIzquierdo();                   
    NodoHuffman* obtenerDerecho();                      
    void fijarIntensidad(int xIntensidad);             
    void fijarFrecuencia(unsigned long xFrecuencia);    
    void fijarIzquierdo(NodoHuffman* xNodo);             
    void fijarDerecho(NodoHuffman* xNodo);             
    bool esHoja();                                       
};

#endif
