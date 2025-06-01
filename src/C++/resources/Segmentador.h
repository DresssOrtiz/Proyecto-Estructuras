#ifndef SEGMENTADOR_H
#define SEGMENTADOR_H

#include <vector>
#include <tuple>
#include <string>
#include "GrafoImagen.h"

using namespace std;

class Segmentador {
private:
    // Datos mínimos
    GrafoImagen grafo;
    vector<tuple<int, int, int> > semillas;
    vector<vector<int> > etiquetas;

public:
    // Constructor
    Segmentador();

    // Operaciones
    void fijarGrafo(GrafoImagen xGrafo);
    void fijarSemillas(const vector<tuple<int, int, int> >& xSemillas);
    void fijarEtiquetas(const vector<vector<int> >& xEtiquetas);

    GrafoImagen obtenerGrafo() const;
    vector<tuple<int, int, int> > obtenerSemillas() const;
    vector<vector<int> > obtenerEtiquetas() const;

    void segmentar();
    void guardarComoPGM(const string& nombreArchivo);
};

#endif

