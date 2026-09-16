#ifndef LOGICA_H
#define LOGICA_H

// funciones base de manipulación de bits.
char ObtenerFicha(int p, unsigned char* tablero);
void PonerFicha(int p, unsigned char* tablero, int valornuevo);

// funciones de reglas del juego.
void InicializarTablero(unsigned char* tablero, int tamano);
void EliminarFicha(int p, unsigned char* tablero);
void EliminarFila(int fila, unsigned char* tablero, int columnas);

#endif // LOGICA_H
