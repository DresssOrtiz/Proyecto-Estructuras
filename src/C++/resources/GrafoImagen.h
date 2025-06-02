#ifndef GRAFOIMAGEN_H
#define GRAFOIMAGEN_H

#include <vector>
using namespace std;

struct Posicion {
    int x;
    int y;
};

struct Arista {
    Posicion destino;
    int costo;
};

class GrafoImagen {
private:
    // Datos mínimos
    int alto;
    int ancho;
    vector<vector<int> > pixeles;
    vector<vector< vector<Arista> > > aristas;

public:
    // Constructor
    GrafoImagen();

    // Operaciones
    void fijarDimensiones(int xAncho, int xAlto);
    void fijarPixeles(const vector<vector<int> >& xPixeles);
    void fijarAristas(const vector<vector< vector<Arista> > >& xAristas);

    Posicion obtenerDimensiones() const;
    vector<vector<int> > obtenerPixeles() const;
    vector<vector< vector<Arista> > > obtenerAristas() const;

    void construirGrafo();
    vector<Arista> obtenerVecinos(int x, int y) const;
    int obtenerCosto(Posicion u, Posicion v) const;
    bool esValido(int x, int y) const;
};

#endif

