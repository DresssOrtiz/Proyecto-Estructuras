#include "ArbolHuffman.h"

// Constructor
ArbolHuffman::ArbolHuffman() {
    raiz = 0;
    codigos.resize(256);
    for (int i = 0; i < 256; i++) {
        codigos[i] = "";
    }
}

NodoHuffman* ArbolHuffman::obtenerRaiz() {
    return raiz;
}

vector<string> ArbolHuffman::obtenerCodigos() {
    return codigos;
}

void ArbolHuffman::fijarRaiz(NodoHuffman* xRaiz) {
    raiz = xRaiz;
}

void ArbolHuffman::fijarCodigos(vector<string> xCodigos) {
    codigos = xCodigos;
}

string ArbolHuffman::obtenerCodigo(int xIntensidad) {
    if (xIntensidad >= 0 && xIntensidad < codigos.size()) {
        return codigos[xIntensidad];
    }
    return "";
}

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
            swap(min1, min2);
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

void ArbolHuffman::generarCodigos() {
    if (raiz != 0) {
        generarCodigosAux(raiz, "");
    }
}

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

vector<vector<int>> ArbolHuffman::decodificarBits(string xBits, int xAncho, int xAlto) {
    vector<vector<int>> pixeles(xAlto, vector<int>(xAncho, 0));
    if (raiz == 0) return pixeles;

    NodoHuffman* actual = raiz;
    int fila = 0, columna = 0;
    int totalPixeles = xAncho * xAlto;
    int decodificados = 0;

    for (char bit : xBits) {
        actual = (bit == '0') ? actual->obtenerIzquierdo() : actual->obtenerDerecho();

        if (actual->esHoja()) {
            pixeles[fila][columna] = actual->obtenerIntensidad();
            columna++;
            if (columna == xAncho) {
                columna = 0;
                fila++;
            }
            decodificados++;
            if (decodificados == totalPixeles) break;
            actual = raiz;
        }
    }

    return pixeles;
}
