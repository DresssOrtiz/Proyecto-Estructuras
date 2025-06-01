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
    aristas = vector<vector< vector< pair< pair<int, int>, int > > > >(alto, vector< vector< pair< pair<int, int>, int > > >(ancho));
}

// Asignar matriz de pixeles
void GrafoImagen::fijarPixeles(const vector<vector<int> >& xPixeles) {
    pixeles = xPixeles;
}

// Asignar estructura de aristas manualmente
void GrafoImagen::fijarAristas(const vector<vector< vector< pair< pair<int, int>, int > > > >& xAristas) {
    aristas = xAristas;
}

// Obtener dimensiones
pair<int, int> GrafoImagen::obtenerDimensiones() const {
    return make_pair(ancho, alto);
}

// Obtener matriz de pixeles
vector<vector<int> > GrafoImagen::obtenerPixeles() const {
    return pixeles;
}

// Obtener estructura de aristas
vector<vector< vector< pair< pair<int, int>, int > > > > GrafoImagen::obtenerAristas() const {
    return aristas;
}

// Verifica si una posición (x, y) es válida
bool GrafoImagen::esValido(int x, int y) const {
    return x >= 0 && x < ancho && y >= 0 && y < alto;
}

// Construye el grafo conectando vecinos arriba, abajo, izquierda y derecha
void GrafoImagen::construirGrafo() {
    aristas = vector<vector< vector< pair< pair<int, int>, int > > > >(alto, vector< vector< pair< pair<int, int>, int > > >(ancho));

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            int intensidadActual = pixeles[y][x];

            // Arriba
            if (esValido(x, y - 1)) {
                int costo = abs(intensidadActual - pixeles[y - 1][x]);
                aristas[y][x].push_back(make_pair(make_pair(x, y - 1), costo));
            }
            // Abajo
            if (esValido(x, y + 1)) {
                int costo = abs(intensidadActual - pixeles[y + 1][x]);
                aristas[y][x].push_back(make_pair(make_pair(x, y + 1), costo));
            }
            // Izquierda
            if (esValido(x - 1, y)) {
                int costo = abs(intensidadActual - pixeles[y][x - 1]);
                aristas[y][x].push_back(make_pair(make_pair(x - 1, y), costo));
            }
            // Derecha
            if (esValido(x + 1, y)) {
                int costo = abs(intensidadActual - pixeles[y][x + 1]);
                aristas[y][x].push_back(make_pair(make_pair(x + 1, y), costo));
            }
        }
    }
}

// Retorna los vecinos de un píxel (x, y) como lista de pares (posición, costo)
vector< pair< pair<int, int>, int > > GrafoImagen::obtenerVecinos(int x, int y) const {
    if (esValido(x, y)) {
        return aristas[y][x];
    } else {
        return vector< pair< pair<int, int>, int > >();
    }
}

// Retorna el costo entre dos píxeles vecinos (u) y (v)
int GrafoImagen::obtenerCosto(pair<int, int> u, pair<int, int> v) const {
    int x1 = u.first;
    int y1 = u.second;
    int x2 = v.first;
    int y2 = v.second;

    if (esValido(x1, y1) && esValido(x2, y2)) {
        return abs(pixeles[y1][x1] - pixeles[y2][x2]);
    } else {
        return -1;
    }
}

