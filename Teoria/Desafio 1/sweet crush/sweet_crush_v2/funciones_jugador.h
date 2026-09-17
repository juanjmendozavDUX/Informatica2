#ifndef FUNCIONES_JUGADOR_H
#define FUNCIONES_JUGADOR_H

void AgregarFila(int posicion_fila, int& filas, int columnas, unsigned char*& tablero);
char ObtenerFicha (int p, unsigned char* tablero);
void PonerFicha(int p, unsigned char* tablero, int valornuevo);
unsigned char* InicializarTablero(int filas, int columnas);
void EliminarFicha(int p, unsigned char* tablero);
void EliminarFila(int fila, unsigned char* tablero, int columnas);
unsigned char* InicializarTablero(int filas, int columnas) ;
#endif // FUNCIONES_JUGADOR_H
