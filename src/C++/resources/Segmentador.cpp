#include "Segmentador.h"
#include <fstream>

// Estructura auxiliar para cola de prioridad simulada
struct EntradaCola {
    int costo;
    int x;
    int y;
    int etiqueta;
};

// Comparador tradicional (reemplazo de lambda)
bool compararEntradas(const EntradaCola& a, const EntradaCola& b) {
    return a.costo > b.costo;
}

// Constructor
Segmentador::Segmentador() {}

// Fijar el grafo
void Segmentador::fijarGrafo(GrafoImagen xGrafo) {
    grafo = xGrafo;
    Posicion dimensiones = grafo.obtenerDimensiones();
    int ancho = dimensiones.x;
    int alto = dimensiones.y;
    etiquetas = vector<vector<int> >(alto, vector<int>(ancho, 0));
}

// Fijar las semillas
void Segmentador::fijarSemillas(const vector<Semilla>& xSemillas) {
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
vector<Semilla> Segmentador::obtenerSemillas() const {
    return semillas;
}

// Obtener etiquetas
vector<vector<int> > Segmentador::obtenerEtiquetas() const {
    return etiquetas;
}

// Segmentar con Dijkstra desde múltiples semillas
void Segmentador::segmentar() {
    const int INFINITO = 999999;

    Posicion dimensiones = grafo.obtenerDimensiones();
    int ancho = dimensiones.x;
    int alto = dimensiones.y;

    etiquetas = vector<vector<int> >(alto, vector<int>(ancho, 0));
    vector<vector<int> > distancia(alto, vector<int>(ancho, INFINITO));

    vector<EntradaCola> cola;

    for (int i = 0; i < semillas.size(); i++) {
        int x = semillas[i].x;
        int y = semillas[i].y;
        int l = semillas[i].etiqueta;

        if (grafo.esValido(x, y)) {
            distancia[y][x] = 0;
            etiquetas[y][x] = l;
            EntradaCola entrada;
            entrada.costo = 0;
            entrada.x = x;
            entrada.y = y;
            entrada.etiqueta = l;
            cola.push_back(entrada);
        }
    }

    while (!cola.empty()) {
        sort(cola.begin(), cola.end(), compararEntradas);

        EntradaCola actual = cola.back();
        cola.pop_back();

        int costo = actual.costo;
        int x = actual.x;
        int y = actual.y;
        int etiqueta = actual.etiqueta;

        vector<Arista> vecinos = grafo.obtenerVecinos(x, y);
        for (int i = 0; i < vecinos.size(); i++) {
            int nx = vecinos[i].destino.x;
            int ny = vecinos[i].destino.y;
            int costoVecino = vecinos[i].costo;

            int nuevoCosto = costo + costoVecino;

            if (nuevoCosto < distancia[ny][nx]) {
                distancia[ny][nx] = nuevoCosto;
                etiquetas[ny][nx] = etiqueta;
                EntradaCola nuevo;
                nuevo.costo = nuevoCosto;
                nuevo.x = nx;
                nuevo.y = ny;
                nuevo.etiqueta = etiqueta;
                cola.push_back(nuevo);
            }
        }
    }
}

// Guardar la imagen segmentada como archivo PGM
void Segmentador::guardarComoPGM(const string& nombreArchivo) {
    Posicion dimensiones = grafo.obtenerDimensiones();
    int ancho = dimensiones.x;
    int alto = dimensiones.y;

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
