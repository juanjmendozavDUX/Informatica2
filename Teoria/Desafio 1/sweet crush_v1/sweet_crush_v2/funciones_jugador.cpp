#include "funciones_jugador.h"
#include <cstdlib>

void AgregarFila(int posicion_fila, int& filas, int columnas, unsigned char*& tablero) {
    int nuevo_tamano = (filas + 1) * columnas;
    unsigned char* nuevotablero = new unsigned char[nuevo_tamano]();

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int p_origen = (f * columnas) + c;
            char ficha_actual = ObtenerFicha(p_origen, tablero);

            int p_destino;
            if (f >= posicion_fila) {
                p_destino = ((f + 1) * columnas) + c;
            } else {
                p_destino = (f * columnas) + c;
            }

            PonerFicha(p_destino, nuevotablero, ficha_actual);
        }
    }

    for (int c = 0; c < columnas; c++) {
        int p = (posicion_fila * columnas) + c;
        int ficha_aleatoria = rand() % 6;
        PonerFicha(p, nuevotablero, ficha_aleatoria);
    }

    delete[] tablero;

    tablero = nuevotablero;

    filas++;
}



unsigned char* InicializarTablero(int filas, int columnas) {
    int totalBits = filas * columnas * 3;
    int bytes = totalBits / 8;
    if (totalBits % 8 != 0) {
        bytes++;
    }

    unsigned char* tablero = new unsigned char[bytes]();

    for (int p = 0; p < filas * columnas; p++) {
        int ficha_aleatoria = rand() % 6;
        PonerFicha(p, tablero, ficha_aleatoria);
    }

    return tablero;
}

void EliminarFicha(int p, unsigned char* tablero) {
    PonerFicha(p, tablero, 0b000);
}

void EliminarFila(int fila, unsigned char* tablero, int columnas) {
    for (int c = 0; c < columnas; c++) {
        int p = (fila * columnas) + c;
        EliminarFicha(p, tablero);
    }
}

void EliminarColumna(int colobjetivo, unsigned char* tablero, int filasTotales, int columnasTotales) {
    for (int f = 0; f < filasTotales; f++) {
        int p = f * columnasTotales + colobjetivo;
        EliminarFicha(p, tablero);
    }
}



char ObtenerFicha (int p, unsigned char* tablero) {
    int bitinicio = p * 3;
    int byteinicio = bitinicio / 8;
    int offset = bitinicio % 8;
    if (offset == 6 || offset == 7) {
        char parte1 = tablero[byteinicio] >> offset;
        char parte2 = tablero[byteinicio + 1];
        char valor = parte1 | (parte2<<(8 - offset));
        valor = valor & 7;
        return valor;

    }
    else {
        char valor = tablero[byteinicio] >> offset;
        valor = valor & 7;
        return valor;
    }
}

void PonerFicha(int p, unsigned char* tablero, int valornuevo) {
    int bitinicio = p * 3;
    int byteinicio = bitinicio / 8;
    int offset = bitinicio % 8;
    if (offset == 6 || offset == 7) {
        tablero[byteinicio] = tablero[byteinicio] & ~(7 << offset);
        tablero[byteinicio] = tablero[byteinicio] | (valornuevo << offset);

        tablero[byteinicio + 1] = tablero[byteinicio + 1] & ~(7 >> (8 - offset));
        tablero[byteinicio + 1] = tablero[byteinicio + 1] | (valornuevo >> (8 - offset));
    }
    else {
        tablero[byteinicio] = tablero[byteinicio] & ~(7 << offset);
        tablero[byteinicio] = tablero[byteinicio] | (valornuevo << offset);
    }
}
