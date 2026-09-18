#include <iostream>

using namespace std;

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