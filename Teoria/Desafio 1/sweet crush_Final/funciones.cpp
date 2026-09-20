#include <iostream>
#include <cstdlib>
#include "logica.h"
using namespace std;

// ============================================================
//  ACCESO A BITS
// ============================================================

int CalcularBytesNecesarios(int filas, int columnas) {
    if (filas <= 0 || columnas <= 0) return 0;
    int total_bits = filas * columnas * 3;
    int bytes = total_bits / 8;
    if (total_bits % 8 != 0) bytes++;
    return bytes;
}

unsigned char ObtenerFicha(int p, unsigned char* tablero) {
    int bitinicio = p * 3;
    int byteinicio = bitinicio / 8;
    int offset = bitinicio % 8;

    if (offset == 6 || offset == 7) {
        // la ficha queda repartida entre byteinicio y byteinicio+1
        unsigned char parte1 = tablero[byteinicio] >> offset;
        unsigned char parte2 = tablero[byteinicio + 1];
        unsigned char valor = parte1 | (parte2 << (8 - offset));
        return valor & 7;
    } else {
        unsigned char valor = tablero[byteinicio] >> offset;
        return valor & 7;
    }
}

void PonerFicha(int p, unsigned char* tablero, int valornuevo) {
    int bitinicio = p * 3;
    int byteinicio = bitinicio / 8;
    int offset = bitinicio % 8;
    valornuevo = valornuevo & 7;

    if (offset == 6 || offset == 7) {
        tablero[byteinicio] = tablero[byteinicio] & ~(7 << offset);
        tablero[byteinicio] = tablero[byteinicio] | (valornuevo << offset);

        tablero[byteinicio + 1] = tablero[byteinicio + 1] & ~(7 >> (8 - offset));
        tablero[byteinicio + 1] = tablero[byteinicio + 1] | (valornuevo >> (8 - offset));
    } else {
        tablero[byteinicio] = tablero[byteinicio] & ~(7 << offset);
        tablero[byteinicio] = tablero[byteinicio] | (valornuevo << offset);
    }
}

// ============================================================
//  CREACION / ELIMINACION PUNTUAL
// ============================================================

unsigned char* InicializarTablero(int filas, int columnas, int& bytes_reservados) {
    bytes_reservados = CalcularBytesNecesarios(filas, columnas);
    unsigned char* tablero = new unsigned char[bytes_reservados]();

    for (int p = 0; p < filas * columnas; p++) {
        int ficha_aleatoria = rand() % 6 + 1;
        PonerFicha(p, tablero, ficha_aleatoria);
    }
    return tablero;
}

void EliminarFicha(int p, unsigned char* tablero) {
    PonerFicha(p, tablero, VACIO);
}

// ============================================================
//  VISUALIZACION
// ============================================================

char ConvertirFicha(unsigned char ficha) {
    switch (ficha) {
        case A: return 'A';
        case B: return 'B';
        case C: return 'C';
        case D: return 'D';
        case E: return 'E';
        case F: return 'F';
        default: return '.'; // VACIO u otro valor no usado
    }
}

void MostrarTablero(int filas, int columnas, unsigned char* tablero) {
    cout << "\n    ";
    for (int c = 0; c < columnas; c++) cout << c << " ";
    cout << "\n";

    for (int f = 0; f < filas; f++) {
        cout << " " << f << " | ";
        for (int c = 0; c < columnas; c++) {
            cout << ConvertirFicha(ObtenerFicha(f * columnas + c, tablero)) << " ";
        }
        cout << "\n";
    }
}

void MostrarTableroBinario(int filas, int columnas, unsigned char* tablero) {
    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas);
    cout << "\nmemoria en bits:\n";
    for (int i = 0; i < bytes_necesarios; i++) {
        cout << "byte " << i << ": ";
        for (int bit = 7; bit >= 0; bit--) cout << ((tablero[i] >> bit) & 1);
        cout << "\n";
    }
}

// ============================================================
//  DETECCION Y RESOLUCION DE COMBINACIONES
// ============================================================

int DetectarCombinaciones(int filas, int columnas, unsigned char* tablero, bool* marcadas) {
    for (int p = 0; p < filas * columnas; p++) marcadas[p] = false;

    int combinaciones = 0;

    // horizontales
    for (int f = 0; f < filas; f++) {
        int c = 0;
        while (c < columnas) {
            unsigned char ficha = ObtenerFicha(f * columnas + c, tablero);
            if (ficha == VACIO) { c++; continue; }
            int inicio = c;
            while (c < columnas && ObtenerFicha(f * columnas + c, tablero) == ficha) c++;
            if (c - inicio >= 3) {
                combinaciones++;
                for (int k = inicio; k < c; k++) marcadas[f * columnas + k] = true;
            }
        }
    }

    // verticales
    for (int c = 0; c < columnas; c++) {
        int f = 0;
        while (f < filas) {
            unsigned char ficha = ObtenerFicha(f * columnas + c, tablero);
            if (ficha == VACIO) { f++; continue; }
            int inicio = f;
            while (f < filas && ObtenerFicha(f * columnas + c, tablero) == ficha) f++;
            if (f - inicio >= 3) {
                combinaciones++;
                for (int k = inicio; k < f; k++) marcadas[k * columnas + c] = true;
            }
        }
    }

    return combinaciones;
}

int EliminarMarcadas(int filas, int columnas, unsigned char* tablero, bool* marcadas) {
    int eliminadas = 0;
    for (int p = 0; p < filas * columnas; p++) {
        if (marcadas[p]) {
            EliminarFicha(p, tablero);
            eliminadas++;
        }
    }
    return eliminadas;
}

void ReorganizarTablero(int filas, int columnas, unsigned char* tablero) {
    for (int c = 0; c < columnas; c++) {
        int escritura = filas - 1;
        for (int f = filas - 1; f >= 0; f--) {
            unsigned char ficha = ObtenerFicha(f * columnas + c, tablero);
            if (ficha != VACIO) {
                if (escritura != f) {
                    PonerFicha(escritura * columnas + c, tablero, ficha);
                    PonerFicha(f * columnas + c, tablero, VACIO);
                }
                escritura--;
            }
        }
    }
}

void RellenarTablero(int filas, int columnas, unsigned char* tablero) {
    for (int p = 0; p < filas * columnas; p++) {
        if (ObtenerFicha(p, tablero) == VACIO) {
            int ficha_aleatoria = rand() % 6 + 1;
            PonerFicha(p, tablero, ficha_aleatoria);
        }
    }
}

void ResolverCascadas(int filas, int columnas, unsigned char* tablero,
                      int& combinaciones, int& fichas_eliminadas,
                      int& puntuacion, int& cascadas) {
    bool* marcadas = new bool[filas * columnas];

    while (true) {
        int combinaciones_actuales = DetectarCombinaciones(filas, columnas, tablero, marcadas);
        if (combinaciones_actuales == 0) break;

        cascadas++;
        combinaciones += combinaciones_actuales;

        int eliminadas = EliminarMarcadas(filas, columnas, tablero, marcadas);
        fichas_eliminadas += eliminadas;

        // Criterio de puntuacion (documentar en el informe):
        // 10 puntos por ficha eliminada + 20 por cada combinacion detectada,
        // y un bono de 50 puntos a partir de la segunda cascada de la misma jugada.
        puntuacion += eliminadas * 10;
        puntuacion += combinaciones_actuales * 20;
        if (cascadas > 1) puntuacion += 50;

        ReorganizarTablero(filas, columnas, tablero);
        RellenarTablero(filas, columnas, tablero);
    }

    delete[] marcadas;
}

// ============================================================
//  MODIFICACION DE ESTRUCTURA
//  Regla del profesor: al eliminar, solo se reasigna memoria fisica
//  si la ocupacion cae por debajo del 65% de lo reservado. Mientras
//  tanto, se compacta "in place" dentro del mismo bloque.
// ============================================================

void EliminarFila(int posicion_fila, int& filas, int columnas, unsigned char*& tablero, int& bytes_reservados) {
    if (filas <= 1 || posicion_fila < 0 || posicion_fila >= filas) return;

    int p_destino = 0;
    for (int f = 0; f < filas; f++) {
        if (f == posicion_fila) continue;
        for (int c = 0; c < columnas; c++) {
            int p_origen = f * columnas + c;
            if (p_destino != p_origen) {
                unsigned char ficha = ObtenerFicha(p_origen, tablero);
                PonerFicha(p_destino, tablero, ficha);
            }
            p_destino++;
        }
    }
    filas--;

    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas);
    if (bytes_necesarios * 100 < bytes_reservados * 65) {
        unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();
        for (int i = 0; i < bytes_necesarios; i++) nuevotablero[i] = tablero[i];
        delete[] tablero;
        tablero = nuevotablero;
        bytes_reservados = bytes_necesarios;
    }
}

void EliminarColumna(int posicion_columna, int filas, int& columnas, unsigned char*& tablero, int& bytes_reservados) {
    if (columnas <= 1 || posicion_columna < 0 || posicion_columna >= columnas) return;

    int p_destino = 0;
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            if (c == posicion_columna) continue;
            int p_origen = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero);
            PonerFicha(p_destino, tablero, ficha);
            p_destino++;
        }
    }
    columnas--;

    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas);
    if (bytes_necesarios * 100 < bytes_reservados * 65) {
        unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();
        for (int i = 0; i < bytes_necesarios; i++) nuevotablero[i] = tablero[i];
        delete[] tablero;
        tablero = nuevotablero;
        bytes_reservados = bytes_necesarios;
    }
}

void AgregarFila(int posicion_fila, int& filas, int columnas, unsigned char*& tablero, int& bytes_reservados) {
    if (posicion_fila < 0 || posicion_fila > filas) return;

    int bytes_necesarios = CalcularBytesNecesarios(filas + 1, columnas);
    unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int p_origen = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero);
            int p_destino = (f >= posicion_fila) ? (f + 1) * columnas + c : f * columnas + c;
            PonerFicha(p_destino, nuevotablero, ficha);
        }
    }
    for (int c = 0; c < columnas; c++) {
        int p_destino = posicion_fila * columnas + c;
        PonerFicha(p_destino, nuevotablero, rand() % 6 + 1);
    }

    delete[] tablero;
    tablero = nuevotablero;
    bytes_reservados = bytes_necesarios;
    filas++;
}

void AgregarColumna(int posicion_columna, int filas, int& columnas, unsigned char*& tablero, int& bytes_reservados) {
    if (posicion_columna < 0 || posicion_columna > columnas) return;

    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas + 1);
    unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int p_origen = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero);
            int c_destino = (c < posicion_columna) ? c : c + 1;
            int p_destino = f * (columnas + 1) + c_destino;
            PonerFicha(p_destino, nuevotablero, ficha);
        }
    }
    for (int f = 0; f < filas; f++) {
        int p_destino = f * (columnas + 1) + posicion_columna;
        PonerFicha(p_destino, nuevotablero, rand() % 6 + 1);
    }

    delete[] tablero;
    tablero = nuevotablero;
    bytes_reservados = bytes_necesarios;
    columnas++;
}
