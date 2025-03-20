#include <iostream>
#include <fstream>
#include <string>
#include <vector>
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
    int maxValor;
    archivo >> formato;

    if (formato != "P2") {
        cout << "Formato de imagen incorrecto.\n";
        return;
    }

    archivo >> imagenCargada.ancho >> imagenCargada.alto >> maxValor;
    imagenCargada.nombre_imagen = nombreArchivo;
    imagenCargada.pixeles.resize(imagenCargada.alto, vector<int>(imagenCargada.ancho));

    for (int i = 0; i < imagenCargada.alto; i++) {
        for (int j = 0; j < imagenCargada.ancho; j++) {
            archivo >> imagenCargada.pixeles[i][j];
        }
    }

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
        cout << "Cantidad de imÃ¡genes invÃ¡lida. Debe estar entre 1 y 99.\n";
        return;
    }

    volumenCargado.nombre_volumen = nombreBase;
    volumenCargado.imagenes.clear();

    for (int i = 1; i <= cantidadImagenes; i++) {
        string nombreArchivo = nombreBase + (i < 10 ? "0" : "") + to_string(i) + ".pgm";
        ifstream archivo(nombreArchivo);

        if (!archivo) {
            cout << "Error al cargar el archivo: " << nombreArchivo << ".\n";
            volumenCargado.imagenes.clear();
            return;
        }

        Imagen img;
        string formato;
        int maxValor;

        archivo >> formato;
        if (formato != "P2") {
            cout << "Formato incorrecto en " << nombreArchivo << ".\n";
            return;
        }

        archivo >> img.ancho >> img.alto >> maxValor;
        img.nombre_imagen = nombreArchivo;
        img.pixeles.resize(img.alto, vector<int>(img.ancho));

        for (int y = 0; y < img.alto; y++) {
            for (int x = 0; x < img.ancho; x++) {
                archivo >> img.pixeles[y][x];
            }
        }

        archivo.close();
        volumenCargado.imagenes.push_back(img);
    }

    hayVolumenCargado = true;
    cout << "El volumen " << nombreBase << " ha sido cargado.\n";
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

    int ancho = volumenCargado.obtenerImagenes()[0].obtenerAncho();
    int alto = volumenCargado.obtenerImagenes()[0].obtenerAlto();
    int profundidad = volumenCargado.obtenerImagenes().size();

    vector<vector<int>> resultado;
    if (direccion == "x") {
        resultado.resize(alto, vector<int>(profundidad, 0));
        for (int y = 0; y < alto; y++) {
            for (int z = 0; z < profundidad; z++) {
                vector<int> valores;
                for (int x = 0; x < ancho; x++) {
                    valores.push_back(volumenCargado.obtenerImagenes()[z].obtenerPixeles()[y][x]);
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
                vector<int> valores;
                for (int y = 0; y < alto; y++) {
                    valores.push_back(volumenCargado.obtenerImagenes()[z].obtenerPixeles()[y][x]);

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
                vector<int> valores;
                for (int z = 0; z < profundidad; z++) {
                    valores.push_back(volumenCargado.obtenerImagenes()[z].obtenerPixeles()[y][x]);

                }
                resultado[y][x] = (criterio == "minimo") ? *min_element(valores.begin(), valores.end())
                                   : (criterio == "maximo") ? *max_element(valores.begin(), valores.end())
                                   : (criterio == "promedio") ? accumulate(valores.begin(), valores.end(), 0) / valores.size()
                                   : valores[valores.size() / 2];
            }
        }
    } else {
        cout << "Direccion invÃ¡lida. Use 'x', 'y' o 'z'.\n";
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
// FunciÃ³n para manejar ayuda especÃ­fica por comando
void mostrarAyudaComando(const string& comando) {
    if (comando == "cargar_imagen") {
        cout << "Para cargar una imagen ingrese el comando 'cargar_imagen nombre_imagen.pgm'.\n";
    } else if (comando == "info_imagen") {
        cout << "Ingrese el comando 'info_imagen' para obtener informaciÃ³n de la imagen cargada.\n";
    } else if (comando == "cargar_volumen") {
        cout << "Ingrese el comando 'cargar_volumen nombre_base n_im' para cargar un volumen de imÃ¡genes.\n";
    } else if (comando == "info_volumen") {
        cout << "Ingrese el comando 'info_volumen' para obtener informaciÃ³n del volumen cargado.\n";
    } else if (comando == "proyeccion2D") {
        cout << "Ingrese el comando 'proyeccion2D direcciÃ³n criterio nombre_archivo.pgm' para generar una proyecciÃ³n 2D.\n";
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
        } else if (comando == "proyeccion2D") {7
        	solicitarProyeccion2D();
        } else if (comando == "ayuda") {
            mostrarAyuda();
        } else if (comando == "ayuda cargar_imagen") {
            cout << "Para cargar una imagen ingrese el comando 'cargar_imagen nombre_imagen.pgm'.\n";
        } else if (comando == "ayuda info_imagen") {
            cout << "Ingrese el comando 'info_imagen' para obtener información de la imagen cargada.\n";
        } else if (comando == "ayuda cargar_volumen") {
            cout << "Ingrese el comando 'cargar_volumen nombre_base n_im' para cargar un volumen de imágenes.\n";
        } else if (comando == "ayuda info_volumen") {
            cout << "Ingrese el comando 'info_volumen' para obtener información del volumen cargado.\n";
        } else if (comando == "ayuda proyeccion2D") {
            cout << "Ingrese el comando 'proyeccion2D dirección criterio nombre_archivo.pgm' para generar una proyección 2D.\n";
        } else if (comando == "salir") {
            break;
        } else {
            cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de comandos.\n";
        }
    }
    return 0;
}

