#ifndef GRAFOIMAGEN_H
#define GRAFOIMAGEN_H

#include <vector>
#include <utility> // para pair

using namespace std;

class GrafoImagen {
private:
    // Datos mínimos
    int alto;
    int ancho;
    vector<vector<int> > pixeles;
    vector<vector< vector< pair< pair<int, int>, int > > > > aristas;

public:
    // Constructor
    GrafoImagen();

    // Operaciones
    void fijarDimensiones(int xAncho, int xAlto);
    void fijarPixeles(const vector<vector<int> >& xPixeles);
    void fijarAristas(const vector<vector< vector< pair< pair<int, int>, int > > > >& xAristas);

    pair<int, int> obtenerDimensiones() const;
    vector<vector<int> > obtenerPixeles() const;
    vector<vector< vector< pair< pair<int, int>, int > > > > obtenerAristas() const;

    void construirGrafo();
    vector< pair< pair<int, int>, int > > obtenerVecinos(int x, int y) const;
    int obtenerCosto(pair<int, int> u, pair<int, int> v) const;
    bool esValido(int x, int y) const;
};

#endif

