#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include "imagen.h"
#include "volumen.h"
#include "proyeccion.h"
#include "ArbolHuffman.h"
#include <bitset> //sirve para manipular y convertir fácilmente secuencias de bits como cadenas binarias en valores numéricos y viceversa.

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
    cout << "  codificar_imagen\n";
    cout << "  decodificar_archivo\n";
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

    int numero = 2;

    for (int i = 0; i < cantidadImagenes; i++) {
        char numeroChar = '0' + numero;
        string numeroStr = "0";
        numeroStr += numeroChar;
        string nombreArchivo = nombreBase + numeroStr + ".pgm";
        numero += 2;

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

    const vector<Imagen>& imagenes = volumenCargado.obtenerImagenes();
    cout << "Volumen cargado en memoria: " << volumenCargado.obtenerNombre_volumen()
         << ", cantidad de imagenes: " << imagenes.size() << ".\n";

    for (int i = 0; i < imagenes.size(); i++) {
        cout << "  Imagen " << i + 1 << ": " << imagenes[i].obtenerNombre_imagen()
             << ", ancho: " << imagenes[i].obtenerAncho()
             << ", alto: " << imagenes[i].obtenerAlto() << ".\n";
    }
}


void proyeccion2D(string direccion, string criterio, string nombreArchivo) {
    if (!hayVolumenCargado) {
        cout << "El volumen aun no ha sido cargado en memoria.\n";
        return;
    }

    const vector<Imagen>& imagenes = volumenCargado.obtenerImagenes();

    int anchoMax = 0, altoMax = 0;
    for (int i = 0; i < imagenes.size(); i++) {
        if (imagenes[i].obtenerAncho() > anchoMax) anchoMax = imagenes[i].obtenerAncho();
        if (imagenes[i].obtenerAlto() > altoMax) altoMax = imagenes[i].obtenerAlto();
    }
    int profundidad = imagenes.size();

    vector<vector<int>> resultado;
    vector<int> valores;

    if (direccion == "x") {
        resultado = vector<vector<int>>(altoMax, vector<int>(profundidad, 0));
        for (int y = 0; y < altoMax; y++) {
            for (int z = 0; z < profundidad; z++) {
                const vector<vector<int>>& pixeles = imagenes[z].obtenerPixeles();
                valores.clear();
                for (int x = 0; x < (int)pixeles[0].size(); x++) {
                    if (y < (int)pixeles.size() && x < (int)pixeles[y].size()) {
                        valores.push_back(pixeles[y][x]);
                    }
                }
                if (!valores.empty()) {
                    if (criterio == "mediana") {
                        sort(valores.begin(), valores.end());
                    }

                    int resultadoValor = 0;

                    if (criterio == "minimo") {
                        resultadoValor = valores[0];
                        for (int i = 1; i < valores.size(); i++) {
                            if (valores[i] < resultadoValor) resultadoValor = valores[i];
                        }
                    } else if (criterio == "maximo") {
                        resultadoValor = valores[0];
                        for (int i = 1; i < valores.size(); i++) {
                            if (valores[i] > resultadoValor) resultadoValor = valores[i];
                        }
                    } else if (criterio == "promedio") {
                        int suma = 0;
                        for (int i = 0; i < valores.size(); i++) {
                            suma += valores[i];
                        }
                        resultadoValor = suma / valores.size();
                    } else {
                        resultadoValor = valores[valores.size() / 2];
                    }

                    resultado[y][z] = resultadoValor;
                }
            }
        }
    } else if (direccion == "y") {
        resultado = vector<vector<int>>(anchoMax, vector<int>(profundidad, 0));
        for (int x = 0; x < anchoMax; x++) {
            for (int z = 0; z < profundidad; z++) {
                const vector<vector<int>>& pixeles = imagenes[z].obtenerPixeles();
                valores.clear();
                for (int y = 0; y < (int)pixeles.size(); y++) {
                    if (x < (int)pixeles[y].size()) {
                        valores.push_back(pixeles[y][x]);
                    }
                }
                if (!valores.empty()) {
                    if (criterio == "mediana") {
                        sort(valores.begin(), valores.end());
                    }

                    int resultadoValor = 0;

                    if (criterio == "minimo") {
                        resultadoValor = valores[0];
                        for (int i = 1; i < valores.size(); i++) {
                            if (valores[i] < resultadoValor) resultadoValor = valores[i];
                        }
                    } else if (criterio == "maximo") {
                        resultadoValor = valores[0];
                        for (int i = 1; i < valores.size(); i++) {
                            if (valores[i] > resultadoValor) resultadoValor = valores[i];
                        }
                    } else if (criterio == "promedio") {
                        int suma = 0;
                        for (int i = 0; i < valores.size(); i++) {
                            suma += valores[i];
                        }
                        resultadoValor = suma / valores.size();
                    } else {
                        resultadoValor = valores[valores.size() / 2];
                    }

                    resultado[x][z] = resultadoValor;
                }
            }
        }
    } else if (direccion == "z") {
        resultado = vector<vector<int>>(altoMax, vector<int>(anchoMax, 0));
        for (int y = 0; y < altoMax; y++) {
            for (int x = 0; x < anchoMax; x++) {
                valores.clear();
                for (int z = 0; z < profundidad; z++) {
                    const vector<vector<int>>& pixeles = imagenes[z].obtenerPixeles();
                    if (y < (int)pixeles.size() && x < (int)pixeles[y].size()) {
                        valores.push_back(pixeles[y][x]);
                    }
                }
                if (!valores.empty()) {
                    if (criterio == "mediana") {
                        sort(valores.begin(), valores.end());
                    }

                    int resultadoValor = 0;

                    if (criterio == "minimo") {
                        resultadoValor = valores[0];
                        for (int i = 1; i < valores.size(); i++) {
                            if (valores[i] < resultadoValor) resultadoValor = valores[i];
                        }
                    } else if (criterio == "maximo") {
                        resultadoValor = valores[0];
                        for (int i = 1; i < valores.size(); i++) {
                            if (valores[i] > resultadoValor) resultadoValor = valores[i];
                        }
                    } else if (criterio == "promedio") {
                        int suma = 0;
                        for (int i = 0; i < valores.size(); i++) {
                            suma += valores[i];
                        }
                        resultadoValor = suma / valores.size();
                    } else {
                        resultadoValor = valores[valores.size() / 2];
                    }

                    resultado[y][x] = resultadoValor;
                }
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
    for (int i = 0; i < resultado.size(); i++) {
        for (int j = 0; j < resultado[i].size(); j++) {
            archivo << resultado[i][j] << " ";
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

void codificarImagen(const string& nombreArchivo) {
    if (!hayImagenCargada) {
        cout << "No hay una imagen cargada en memoria.\n";
        return;
    }

    int ancho = imagenCargada.obtenerAncho();
    int alto = imagenCargada.obtenerAlto();
    int maxVal = 255;
    const vector<vector<int>>& pixeles = imagenCargada.obtenerPixeles();

    // Calcular frecuencias
    unsigned long frecuencias[256] = {0};
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            frecuencias[pixeles[i][j]]++;
        }
    }

    // Construir árbol de Huffman
    ArbolHuffman arbol;
    arbol.construirDesdeFrecuencias(frecuencias, 256);
    arbol.generarCodigos();
    vector<string> codigos = arbol.obtenerCodigos();

    // Generar string binario
    string bits;
    for (int i = 0; i < alto; i++) {
        for (int j = 0; j < ancho; j++) {
            bits += codigos[pixeles[i][j]];
        }
    }

    // Guardar en archivo binario
    ofstream salida(nombreArchivo, ios::binary);
    if (!salida) {
        cout << "No se pudo crear el archivo.\n";
        return;
    }

    unsigned short W = ancho, H = alto;
    unsigned char M = (unsigned char)maxVal;
    salida.write(reinterpret_cast<char*>(&W), sizeof(W));
    salida.write(reinterpret_cast<char*>(&H), sizeof(H));
    salida.write(reinterpret_cast<char*>(&M), sizeof(M));

    for (int i = 0; i <= M; i++) {
        salida.write(reinterpret_cast<char*>(&frecuencias[i]), sizeof(unsigned long));
    }


// Escribir la secuencia de bits directamente (sin empaquetar en bytes)
salida.write(bits.c_str(), bits.size());

    salida.close();
    cout << "La imagen en memoria ha sido codificada exitosamente y almacenada en el archivo " << nombreArchivo << ".\n";
}

void decodificarArchivo(const string& nombreArchivoHuf, const string& nombreImagenPGM) {
    ifstream entrada(nombreArchivoHuf, ios::binary);
    if (!entrada) {
        cout << "El archivo " << nombreArchivoHuf << " no ha podido ser decodificado.\n";
        return;
    }

    unsigned short W, H;
    unsigned char M;
    entrada.read(reinterpret_cast<char*>(&W), sizeof(W));
    entrada.read(reinterpret_cast<char*>(&H), sizeof(H));
    entrada.read(reinterpret_cast<char*>(&M), sizeof(M));

    unsigned long frecuencias[256] = {0};
    for (int i = 0; i <= M; i++) {
        entrada.read(reinterpret_cast<char*>(&frecuencias[i]), sizeof(unsigned long));
    }

    string bits = "";
    char byte;
    while (entrada.read(&byte, sizeof(byte))) {
        bitset<8> b((unsigned char)byte);
        bits += b.to_string();
    }

    entrada.close();

    ArbolHuffman arbol;
    arbol.construirDesdeFrecuencias(frecuencias, 256);
    arbol.generarCodigos();

    vector<vector<int>> pixeles = arbol.decodificarBits(bits, W, H);

    ofstream salida(nombreImagenPGM);
    salida << "P2\n" << W << " " << H << "\n255\n";
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {
            salida << pixeles[i][j] << " ";
        }
        salida << "\n";
    }
    salida.close();

    cout << "El archivo " << nombreArchivoHuf << " ha sido decodificado exitosamente, y la imagen correspondiente se ha almacenado en el archivo " << nombreImagenPGM << ".\n";
}

int main() {
    string linea;
    mostrarAyuda();

    while (true) {
        cout << "$ ";
        getline(cin, linea);
        if (linea.empty()) continue;

        // divide la línea en tokens
        istringstream iss(linea);
        vector<string> tokens;
        string token;
        while (iss >> token)
            tokens.push_back(token);

        if (tokens.empty()) continue;

        string comando = tokens[0];

        // =================== CARGAR ===================
        if (comando == "cargar") {
            if (tokens.size() >= 3 && tokens[1] == "imagen") {
                string nombreArchivo = tokens[2];
                ifstream archivo(nombreArchivo);
                if (!archivo) {
                    cout << "La imagen " << nombreArchivo << " no ha podido ser cargada.\n";
                } else {
                    string formato;
                    int ancho, alto, maxValor;
                    archivo >> formato >> ancho >> alto >> maxValor;

                    if (formato != "P2") {
                        cout << "Formato de imagen incorrecto.\n";
                    } else {
                        vector<vector<int>> pixeles(alto, vector<int>(ancho));
                        for (int i = 0; i < alto; i++)
                            for (int j = 0; j < ancho; j++)
                                archivo >> pixeles[i][j];

                        imagenCargada.fijarNombre_imagen(nombreArchivo);
                        imagenCargada.fijarDimensiones(ancho, alto);
                        imagenCargada.fijarPixeles(pixeles);
                        hayImagenCargada = true;
                        cout << "La imagen " << nombreArchivo << " ha sido cargada.\n";
                    }
                    archivo.close();
                }
            } else if (tokens.size() >= 4 && tokens[1] == "volumen") {
                string nombreBase = tokens[2];
                int cantidadImagenes = stoi(tokens[3]);

                if (cantidadImagenes < 1 || cantidadImagenes > 99) {
                    cout << "Cantidad de imagenes invalida. Debe estar entre 1 y 99.\n";
                } else {
                    volumenCargado.fijarNombre_volumen(nombreBase);
                    vector<Imagen> imagenes;
                    int numero = 2;
                    for (int i = 0; i < cantidadImagenes; i++) {
                        string numeroStr = (numero < 10) ? "0" + to_string(numero) : to_string(numero);
                        string nombreArchivo = nombreBase + numeroStr + ".pgm";
                        numero += 2;

                        ifstream archivo(nombreArchivo);
                        if (!archivo) {
                            cout << "Error al cargar el archivo: " << nombreArchivo << ".\n";
                            break;
                        }

                        string formato;
                        int ancho, alto, maxValor;
                        archivo >> formato >> ancho >> alto >> maxValor;

                        if (formato != "P2") {
                            cout << "Formato incorrecto en " << nombreArchivo << ".\n";
                            break;
                        }

                        Imagen img;
                        img.fijarNombre_imagen(nombreArchivo);
                        img.fijarDimensiones(ancho, alto);
                        vector<vector<int>> pixeles(alto, vector<int>(ancho));
                        for (int y = 0; y < alto; y++)
                            for (int x = 0; x < ancho; x++)
                                archivo >> pixeles[y][x];
                        img.fijarPixeles(pixeles);
                        imagenes.push_back(img);
                        archivo.close();
                    }

                    volumenCargado.fijarImagenes(imagenes);
                    hayVolumenCargado = true;
                    cout << "El volumen " << nombreBase << " ha sido cargado.\n";
                }
            } else {
                cout << "Uso: cargar imagen archivo.pgm | cargar volumen base n_im\n";
            }

        // =================== INFO ===================
        } else if (comando == "info") {
            if (tokens.size() >= 2 && tokens[1] == "imagen") {
                infoImagen();
            } else if (tokens.size() >= 2 && tokens[1] == "volumen") {
                infoVolumen();
            } else {
                cout << "Uso: info imagen | info volumen\n";
            }

        // =================== PROYECCIÓN ===================
        } else if (comando == "proyeccion2D") {
            if (tokens.size() >= 4) {
                proyeccion2D(tokens[1], tokens[2], tokens[3]);
            } else {
                solicitarProyeccion2D();
            }

        // =================== CODIFICAR ===================
        } else if (comando == "codificar") {
            if (tokens.size() >= 2) codificarImagen(tokens[1]);
            else cout << "Uso: codificar archivo.huf\n";

        // =================== DECODIFICAR ===================
        } else if (comando == "decodificar") {
            if (tokens.size() >= 3) decodificarArchivo(tokens[1], tokens[2]);
            else cout << "Uso: decodificar archivo.huf salida.pgm\n";

        // =================== AYUDA ===================
        } else if (comando == "ayuda") {
            if (tokens.size() >= 2)
                mostrarAyudaComando(tokens[1]);
            else
                mostrarAyuda();

        // =================== SALIR ===================
        } else if (comando == "salir") {
            break;

        // =================== ERROR ===================
        } else {
            cout << "Comando no reconocido. Escriba 'ayuda' para ver la lista de comandos.\n";
        }
    }

    return 0;
}
