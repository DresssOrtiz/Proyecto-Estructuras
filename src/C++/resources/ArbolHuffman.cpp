#include "ArbolHuffman.h"

// Constructor
ArbolHuffman::ArbolHuffman() {
    raiz = 0;
    codigos.resize(256);
    for (int i = 0; i < 256; i++) {
        codigos[i] = "";
    }
}

// Obtener la raíz del árbol
NodoHuffman* ArbolHuffman::obtenerRaiz() {
    return raiz;
}

// Obtener el vector de códigos
vector<string> ArbolHuffman::obtenerCodigos() {
    return codigos;
}

// Fijar la raíz del árbol
void ArbolHuffman::fijarRaiz(NodoHuffman* xRaiz) {
    raiz = xRaiz;
}

// Fijar el vector de códigos
void ArbolHuffman::fijarCodigos(vector<string> xCodigos) {
    codigos = xCodigos;
}

// Obtener el código Huffman de una intensidad específica
string ArbolHuffman::obtenerCodigo(int xIntensidad) {
    if (xIntensidad >= 0 && xIntensidad < codigos.size()) {
        return codigos[xIntensidad];
    }
    return "";
}

// Construir el árbol de Huffman a partir de un arreglo de frecuencias
void ArbolHuffman::construirDesdeFrecuencias(unsigned long xFrecuencias[], int tamano) {
    NodoHuffman* nodos[256];
    int total = 0;

    for (int i = 0; i < tamano; i++) {
        if (xFrecuencias[i] > 0) {
            nodos[total] = new NodoHuffman(i, xFrecuencias[i]);
            total++;
        }
    }

    while (total > 1) {
        int min1 = 0, min2 = 1;
        if (nodos[min2]->obtenerFrecuencia() < nodos[min1]->obtenerFrecuencia()) {
            int temp = min1;
            min1 = min2;
            min2 = temp;
        }

        for (int i = 2; i < total; i++) {
            unsigned long f = nodos[i]->obtenerFrecuencia();
            if (f < nodos[min1]->obtenerFrecuencia()) {
                min2 = min1;
                min1 = i;
            } else if (f < nodos[min2]->obtenerFrecuencia()) {
                min2 = i;
            }
        }

        NodoHuffman* combinado = new NodoHuffman(-1, nodos[min1]->obtenerFrecuencia() + nodos[min2]->obtenerFrecuencia());
        combinado->fijarIzquierdo(nodos[min1]);
        combinado->fijarDerecho(nodos[min2]);

        if (min1 < min2) {
            nodos[min1] = combinado;
            nodos[min2] = nodos[total - 1];
        } else {
            nodos[min2] = combinado;
            nodos[min1] = nodos[total - 1];
        }

        total--;
    }

    if (total == 1) {
        raiz = nodos[0];
    }
}

// Generar los códigos Huffman desde el árbol
void ArbolHuffman::generarCodigos() {
    if (raiz != 0) {
        generarCodigosAux(raiz, "");
    }
}

// Función auxiliar recursiva para recorrer el árbol y asignar códigos
void ArbolHuffman::generarCodigosAux(NodoHuffman* nodo, string camino) {
    if (nodo->esHoja()) {
        int intensidad = nodo->obtenerIntensidad();
        codigos[intensidad] = camino;
    } else {
        if (nodo->obtenerIzquierdo() != 0) {
            generarCodigosAux(nodo->obtenerIzquierdo(), camino + "0");
        }
        if (nodo->obtenerDerecho() != 0) {
            generarCodigosAux(nodo->obtenerDerecho(), camino + "1");
        }
    }
}

// Reconstruir la imagen a partir de los bits codificados
vector<vector<int>> ArbolHuffman::decodificarBits(string xBits, int xAncho, int xAlto) {
    vector<vector<int>> pixeles(xAlto, vector<int>(xAncho, 0));

    if (raiz == 0) return pixeles;

    NodoHuffman* actual = raiz;
    int fila = 0, columna = 0;

    for (int i = 0; i < xBits.size(); i++) {
        if (xBits[i] == '0') {
            actual = actual->obtenerIzquierdo();
        } else {
            actual = actual->obtenerDerecho();
        }

        if (actual->esHoja()) {
            pixeles[fila][columna] = actual->obtenerIntensidad();
            columna++;

            if (columna == xAncho) {
                columna = 0;
                fila++;
                if (fila == xAlto) break;
            }

            actual = raiz;
        }
    }

    return pixeles;
}
