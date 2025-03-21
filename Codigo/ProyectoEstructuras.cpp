#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include "imagen.h"
#include "volumen.h"
#include "proyeccion.h"

using namespace std;

// Variables globales**********************************************************************************
Imagen imagenCargada;
bool hayImagenCargada = false;

Volumen volumenCargado;
bool hayVolumenCargado = false;

void proyeccion2D(string direccion, string criterio, string nombreArchivo);
void cargarImagen();
void cargarVolumen();
void infoVolumen();
void solicitarProyeccion2D();

//Comandos*****************************************************************
void mostrarAyuda() {
    cout << "Comandos disponibles:\n";
    cout << "  cargar_imagen\n";
    cout << "  cargar_volumen\n";
    cout << "  info_imagen\n";
    cout << "  info_volumen\n";
    cout << "  proyeccion2D\n";
    cout << "  ayuda\n";
    cout << "  salir\n";
}

//Funciones*********************************************************************
void cargarImagen() {
    string nombreArchivo;
    cout << "Ingrese el nombre de la imagen (.pgm): ";
    cin >> nombreArchivo;

    ifstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "La imagen " << nombreArchivo << " no ha podido ser cargada.\n";
        return;
    }

    string formato;
    int ancho, alto, maxValor;

    archivo >> formato;
    if (formato != "P2") {
        cout << "Formato de imagen incorrecto.\n";
        archivo.close();
        return;
    }

    archivo >> ancho >> alto >> maxValor;
    imagenCargada.fijarDimensiones(ancho, alto);
    imagenCargada.fijarNombre_imagen(nombreArchivo);

    // Leer los píxeles de la imagen
    vector<vector<int>> pixeles(alto, vector<int>(ancho));
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            archivo >> pixeles[i][j];
        }
    }
    imagenCargada.fijarPixeles(pixeles);

    archivo.close();
    hayImagenCargada = true;
    cout << "La imagen " << nombreArchivo << " ha sido cargada.\n";
}


void cargarVolumen() {
    string nombreBase;
    int cantidadImagenes;

    cout << "Ingrese el nombre base del volumen: ";
    cin >> nombreBase;
    cout << "Ingrese la cantidad de imagenes en el volumen: ";
    cin >> cantidadImagenes;

    if (cantidadImagenes < 1 || cantidadImagenes > 99) {
        cout << "Cantidad de imagenes invalida. Debe estar entre 1 y 99.\n";
        return;
    }

    volumenCargado.fijarNombre_volumen(nombreBase);
    vector<Imagen> imagenes;

    for (int i = 1; i <= cantidadImagenes; i++) {
        string nombreArchivo = nombreBase + (i < 10 ? "0" : "") + to_string(i) + ".pgm";
        ifstream archivo(nombreArchivo);

        if (!archivo) {
            cout << "Error al cargar el archivo: " << nombreArchivo << ".\n";
            return;
        }

        string formato;
        int ancho, alto, maxValor;

        archivo >> formato;
        if (formato != "P2") {
            cout << "Formato incorrecto en " << nombreArchivo << ".\n";
            archivo.close();
            return;
        }

        archivo >> ancho >> alto >> maxValor;

        Imagen img;
        img.fijarNombre_imagen(nombreArchivo);
        img.fijarDimensiones(ancho, alto);

        vector<vector<int>> pixeles(alto, vector<int>(ancho));
        for (int y = 0; y < alto; y++) {
            for (int x = 0; x < ancho; x++) {
                archivo >> pixeles[y][x];
            }
        }
        img.fijarPixeles(pixeles);

        archivo.close();
        imagenes.push_back(img);
    }

    volumenCargado.fijarImagenes(imagenes);
    hayVolumenCargado = true;
    cout << "El volumen " << nombreBase << " ha sido cargado.\n";
}

void infoImagen() {
    if (!hayImagenCargada) {
        cout << "No hay una imagen cargada en memoria.\n";
        return;
    }

    cout << "Imagen cargada en memoria: " << imagenCargada.obtenerNombre_imagen() 
         << ", ancho: " << imagenCargada.obtenerAncho() 
         << ", alto: " << imagenCargada.obtenerAlto() << ".\n";
}


void infoVolumen() {
    if (!hayVolumenCargado) {
        cout << "No hay un volumen cargado en memoria.\n";
        return;
    }

    cout << "Volumen cargado en memoria: " << volumenCargado.obtenerNombre_volumen() << ", tamano: " << volumenCargado.obtenerImagenes().size()
         << ", ancho: " << volumenCargado.obtenerImagenes()[0].obtenerAncho() << ", alto: " << volumenCargado.obtenerImagenes()[0].obtenerAlto() << ".\n";
}

void proyeccion2D(string direccion, string criterio, string nombreArchivo) {
    if (!hayVolumenCargado) {
        cout << "El volumen aun no ha sido cargado en memoria.\n";
        return;
    }
    const vector<Imagen>& imagenes = volumenCargado.obtenerImagenes();
    int ancho = imagenes[0].obtenerAncho();
    int alto = imagenes[0].obtenerAlto();
    int profundidad = imagenes.size();

    vector<vector<int>> resultado;
    vector<int> valores;  // Se declara fuera del bucle para reutilizar la memoria

    if (direccion == "x") {
        resultado.resize(alto, vector<int>(profundidad, 0));
        for (int y = 0; y < alto; y++) {
            for (int z = 0; z < profundidad; z++) {
                const vector<vector<int>>& pixeles = imagenes[z].obtenerPixeles();
                valores.clear();  // Se reutiliza en cada iteración
                for (int x = 0; x < ancho; x++) {
                    valores.push_back(pixeles[y][x]);
                }
                resultado[y][z] = (criterio == "minimo") ? *min_element(valores.begin(), valores.end())
                                   : (criterio == "maximo") ? *max_element(valores.begin(), valores.end())
                                   : (criterio == "promedio") ? accumulate(valores.begin(), valores.end(), 0) / valores.size()
                                   : valores[valores.size() / 2];
            }
        }
    } else if (direccion == "y") {
        resultado.resize(ancho, vector<int>(profundidad, 0));
        for (int x = 0; x < ancho; x++) {
            for (int z = 0; z < profundidad; z++) {
                const vector<vector<int>>& pixeles = imagenes[z].obtenerPixeles();
                valores.clear();
                for (int y = 0; y < alto; y++) {
                    valores.push_back(pixeles[y][x]);
                }
                resultado[x][z] = (criterio == "minimo") ? *min_element(valores.begin(), valores.end())
                                   : (criterio == "maximo") ? *max_element(valores.begin(), valores.end())
                                   : (criterio == "promedio") ? accumulate(valores.begin(), valores.end(), 0) / valores.size()
                                   : valores[valores.size() / 2];
            }
        }
    } else if (direccion == "z") {
        resultado.resize(alto, vector<int>(ancho, 0));
        for (int y = 0; y < alto; y++) {
            for (int x = 0; x < ancho; x++) {
                valores.clear();
                for (int z = 0; z < profundidad; z++) {
                    const vector<vector<int>>& pixeles = imagenes[z].obtenerPixeles();
                    valores.push_back(pixeles[y][x]);
                }
                resultado[y][x] = (criterio == "minimo") ? *min_element(valores.begin(), valores.end())
                                   : (criterio == "maximo") ? *max_element(valores.begin(), valores.end())
                                   : (criterio == "promedio") ? accumulate(valores.begin(), valores.end(), 0) / valores.size()
                                   : valores[valores.size() / 2];
            }
        }
    } else {
        cout << "Direccion invalida. Use 'x', 'y' o 'z'.\n";
        return;
    }

    ofstream archivo(nombreArchivo);
    if (!archivo) {
        cout << "La proyeccion 2D del volumen en memoria no ha podido ser generada.\n";
        return;
    }
    archivo << "P2\n" << resultado[0].size() << " " << resultado.size() << "\n255\n";
    for (const auto& fila : resultado) {
        for (int valor : fila) {
            archivo << valor << " ";
        }
        archivo << "\n";
    }
    archivo.close();
    cout << "La proyeccion 2D del volumen en memoria ha sido generada y almacenada en el archivo " << nombreArchivo << ".\n";
}


void solicitarProyeccion2D() {
    if (!hayVolumenCargado) {
        cout << "El volumen aun no ha sido cargado en memoria.\n";
        return;
    }
    
    string direccion, criterio, nombreArchivo;
    cout << "Ingrese la direccion de proyeccion (x, y, z): ";
    cin >> direccion;
    
    cout << "Ingrese el criterio de proyeccion (minimo, maximo, promedio, mediana): ";
    cin >> criterio;
    
    cout << "Ingrese el nombre del archivo de salida (.pgm): ";
    cin >> nombreArchivo;
    
    proyeccion2D(direccion, criterio, nombreArchivo);
}

void mostrarAyudaComando(const string& comando) {
    if (comando == "cargar_imagen") {
        cout << "Para cargar una imagen ingrese el comando 'cargar_imagen nombre_imagen.pgm'.\n";
    } else if (comando == "info_imagen") {
        cout << "Ingrese el comando 'info_imagen' para obtener informacion de la imagen cargada.\n";
    } else if (comando == "cargar_volumen") {
        cout << "Ingrese el comando 'cargar_volumen nombre_base n_im' para cargar un volumen de imagenes.\n";
    } else if (comando == "info_volumen") {
        cout << "Ingrese el comando 'info_volumen' para obtener informacion del volumen cargado.\n";
    } else if (comando == "proyeccion2D") {
        cout << "Ingrese el comando 'proyeccion2D direccion criterio nombre_archivo.pgm' para generar una proyeccion 2D.\n";
    } else {
        cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de comandos.\n";
    }
}

//Main****************************************************************************
int main() {
    string comando;
    mostrarAyuda();

    while (true) {
        cout << "$ ";
        getline(cin, comando);

        if (comando == "cargar_imagen") {
            cargarImagen();
        } else if (comando == "cargar_volumen") {
            cargarVolumen();
        } else if (comando == "info_imagen") {
            infoImagen();
        } else if (comando == "info_volumen") {
            infoVolumen();
        } else if (comando == "proyeccion2D") {
        	solicitarProyeccion2D();
        } else if (comando == "ayuda") {
            mostrarAyuda();
        } else if (comando == "ayuda cargar_imagen") {
            cout << "Para cargar una imagen ingrese el comando 'cargar_imagen nombre_imagen.pgm'.\n";
        } else if (comando == "ayuda info_imagen") {
            cout << "Ingrese el comando 'info_imagen' para obtener información de la imagen cargada.\n";
        } else if (comando == "ayuda cargar_volumen") {
            cout << "Ingrese el comando 'cargar_volumen nombre_base n_im' para cargar un volumen de imagenes.\n";
        } else if (comando == "ayuda info_volumen") {
            cout << "Ingrese el comando 'info_volumen' para obtener informacion del volumen cargado.\n";
        } else if (comando == "ayuda proyeccion2D") {
            cout << "Ingrese el comando 'proyeccion2D dirección criterio nombre_archivo.pgm' para generar una proyeccion 2D.\n";
        } else if (comando == "salir") {
            break;
        } else {
            cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de comandos.\n";
        }
    }
    return 0;
}

