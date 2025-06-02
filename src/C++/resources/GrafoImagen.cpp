#include "GrafoImagen.h"

// Constructor
GrafoImagen::GrafoImagen() {
    alto = 0;
    ancho = 0;
}

// Asignar dimensiones
void GrafoImagen::fijarDimensiones(int xAncho, int xAlto) {
    ancho = xAncho;
    alto = xAlto;
    pixeles = vector<vector<int> >(alto, vector<int>(ancho, 0));
    aristas = vector<vector< vector<Arista> > >(alto, vector< vector<Arista> >(ancho));
}

// Asignar matriz de pixeles
void GrafoImagen::fijarPixeles(const vector<vector<int> >& xPixeles) {
    pixeles = xPixeles;
}

// Asignar estructura de aristas manualmente
void GrafoImagen::fijarAristas(const vector<vector< vector<Arista> > >& xAristas) {
    aristas = xAristas;
}

// Obtener dimensiones
Posicion GrafoImagen::obtenerDimensiones() const {
    Posicion dim;
    dim.x = ancho;
    dim.y = alto;
    return dim;
}

// Obtener matriz de pixeles
vector<vector<int> > GrafoImagen::obtenerPixeles() const {
    return pixeles;
}

// Obtener estructura de aristas
vector<vector< vector<Arista> > > GrafoImagen::obtenerAristas() const {
    return aristas;
}

// Verifica si una posición (x, y) es válida
bool GrafoImagen::esValido(int x, int y) const {
    return x >= 0 && x < ancho && y >= 0 && y < alto;
}

// Construye el grafo conectando vecinos arriba, abajo, izquierda y derecha
void GrafoImagen::construirGrafo() {
    aristas = vector<vector< vector<Arista> > >(alto, vector< vector<Arista> >(ancho));

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int intensidadActual = pixeles[y][x];

            // Arriba
            if (esValido(x, y - 1)) {
                int vecinoInt = pixeles[y - 1][x];
                int costo = intensidadActual - vecinoInt;
                if (costo < 0) costo = -costo;

                Posicion destino;
                destino.x = x;
                destino.y = y - 1;

                Arista a;
                a.destino = destino;
                a.costo = costo;
                aristas[y][x].push_back(a);
            }

            // Abajo
            if (esValido(x, y + 1)) {
                int vecinoInt = pixeles[y + 1][x];
                int costo = intensidadActual - vecinoInt;
                if (costo < 0) costo = -costo;

                Posicion destino;
                destino.x = x;
                destino.y = y + 1;

                Arista a;
                a.destino = destino;
                a.costo = costo;
                aristas[y][x].push_back(a);
            }

            // Izquierda
            if (esValido(x - 1, y)) {
                int vecinoInt = pixeles[y][x - 1];
                int costo = intensidadActual - vecinoInt;
                if (costo < 0) costo = -costo;

                Posicion destino;
                destino.x = x - 1;
                destino.y = y;

                Arista a;
                a.destino = destino;
                a.costo = costo;
                aristas[y][x].push_back(a);
            }

            // Derecha
            if (esValido(x + 1, y)) {
                int vecinoInt = pixeles[y][x + 1];
                int costo = intensidadActual - vecinoInt;
                if (costo < 0) costo = -costo;

                Posicion destino;
                destino.x = x + 1;
                destino.y = y;

                Arista a;
                a.destino = destino;
                a.costo = costo;
                aristas[y][x].push_back(a);
            }
        }
    }
}

// Retorna los vecinos de un píxel (x, y)
vector<Arista> GrafoImagen::obtenerVecinos(int x, int y) const {
    if (esValido(x, y)) {
        return aristas[y][x];
    } else {
        return vector<Arista>();
    }
}

// Retorna el costo entre dos píxeles vecinos
int GrafoImagen::obtenerCosto(Posicion u, Posicion v) const {
    int x1 = u.x;
    int y1 = u.y;
    int x2 = v.x;
    int y2 = v.y;

    if (esValido(x1, y1) && esValido(x2, y2)) {
        int costo = pixeles[y1][x1] - pixeles[y2][x2];
        if (costo < 0) costo = -costo;
        return costo;
    } else {
        return -1;
    }
}
