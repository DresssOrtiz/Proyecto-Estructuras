#ifndef SEGMENTADOR_H
#define SEGMENTADOR_H

#include <vector>
#include <string>
#include "GrafoImagen.h"

using namespace std;

struct Semilla {
    int x;
    int y;
    int etiqueta;
};

class Segmentador {
private:
    // Datos mínimos
    GrafoImagen grafo;
    vector<Semilla> semillas;
    vector<vector<int> > etiquetas;

public:
    // Constructor
    Segmentador();

    // Operaciones
    void fijarGrafo(GrafoImagen xGrafo);
    void fijarSemillas(const vector<Semilla>& xSemillas);
    void fijarEtiquetas(const vector<vector<int> >& xEtiquetas);

    GrafoImagen obtenerGrafo() const;
    vector<Semilla> obtenerSemillas() const;
    vector<vector<int> > obtenerEtiquetas() const;

    void segmentar();
    void guardarComoPGM(const string& nombreArchivo);
};

#endif
