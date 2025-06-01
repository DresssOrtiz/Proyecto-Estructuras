#include "Segmentador.h"
#include <fstream>
#include <queue>
#include <algorithm> // para sort
#include <climits>   // para INT_MAX

// Constructor
Segmentador::Segmentador() {}

// Fijar el grafo
void Segmentador::fijarGrafo(GrafoImagen xGrafo) {
    grafo = xGrafo;
    pair<int, int> dimensiones = grafo.obtenerDimensiones();
    int ancho = dimensiones.first;
    int alto = dimensiones.second;
    etiquetas = vector<vector<int> >(alto, vector<int>(ancho, 0));
}

// Fijar las semillas
void Segmentador::fijarSemillas(const vector<tuple<int, int, int> >& xSemillas) {
    semillas = xSemillas;
}

// Fijar etiquetas manualmente (opcional)
void Segmentador::fijarEtiquetas(const vector<vector<int> >& xEtiquetas) {
    etiquetas = xEtiquetas;
}

// Obtener grafo
GrafoImagen Segmentador::obtenerGrafo() const {
    return grafo;
}

// Obtener semillas
vector<tuple<int, int, int> > Segmentador::obtenerSemillas() const {
    return semillas;
}

// Obtener etiquetas
vector<vector<int> > Segmentador::obtenerEtiquetas() const {
    return etiquetas;
}

// Segmentar con Dijkstra desde múltiples semillas
void Segmentador::segmentar() {
    pair<int, int> dimensiones = grafo.obtenerDimensiones();
    int ancho = dimensiones.first;
    int alto = dimensiones.second;

    // Inicializar etiquetas en 0 y distancias en infinito
    etiquetas = vector<vector<int> >(alto, vector<int>(ancho, 0));
    vector<vector<int> > distancia(alto, vector<int>(ancho, INT_MAX));

    // Cola de prioridad como vector: (costo acumulado, x, y, etiqueta)
    vector<tuple<int, int, int, int> > cola;

    // Insertar semillas en la cola
    for (int i = 0; i < semillas.size(); i++) {
        int x = get<0>(semillas[i]);
        int y = get<1>(semillas[i]);
        int l = get<2>(semillas[i]);

        if (grafo.esValido(x, y)) {
            distancia[y][x] = 0;
            etiquetas[y][x] = l;
            cola.push_back(make_tuple(0, x, y, l));
        }
    }

    // Mientras haya elementos en la cola
    while (!cola.empty()) {
        // Ordenar para que el menor esté al final
        sort(cola.begin(), cola.end(), [](const tuple<int, int, int, int>& a, const tuple<int, int, int, int>& b) {
            return get<0>(a) > get<0>(b);
        });

        // Extraer el nodo con menor costo
        tuple<int, int, int, int> actual = cola.back();
        cola.pop_back();

        int costo = get<0>(actual);
        int x = get<1>(actual);
        int y = get<2>(actual);
        int etiqueta = get<3>(actual);

        // Revisar vecinos
        vector< pair< pair<int, int>, int > > vecinos = grafo.obtenerVecinos(x, y);
        for (int i = 0; i < vecinos.size(); i++) {
            int nx = vecinos[i].first.first;
            int ny = vecinos[i].first.second;
            int costoVecino = vecinos[i].second;

            int nuevoCosto = costo + costoVecino;

            if (nuevoCosto < distancia[ny][nx]) {
                distancia[ny][nx] = nuevoCosto;
                etiquetas[ny][nx] = etiqueta;
                cola.push_back(make_tuple(nuevoCosto, nx, ny, etiqueta));
            }
        }
    }
}

// Guardar la imagen segmentada como archivo PGM
void Segmentador::guardarComoPGM(const string& nombreArchivo) {
    pair<int, int> dimensiones = grafo.obtenerDimensiones();
    int ancho = dimensiones.first;
    int alto = dimensiones.second;

    ofstream archivo(nombreArchivo.c_str());
    if (!archivo.is_open()) return;

    archivo << "P2\n" << ancho << " " << alto << "\n255\n";

    for (int y = 0; y < alto; y++) {
        for (int x = 0; x < ancho; x++) {
            archivo << etiquetas[y][x] << " ";
        }
        archivo << "\n";
    }

    archivo.close();
}

