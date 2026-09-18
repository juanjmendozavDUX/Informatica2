#ifndef LOGICA_H
#define LOGICA_H

const int VACIO = 0;
const int ESPECIAL = 2;

const int A = 7;
const int B = 1;
const int C = 3;
const int D = 5;
const int E = 4;
const int F = 6;

// acceso a bits
int CalcularBytesNecesarios(int filas, int columnas);
unsigned char ObtenerFicha(int p, unsigned char* tablero, int bytes_reservados);
void PonerFicha(int p, unsigned char* tablero, int valornuevo, int bytes_reservados);
void LimpiarBitsSobrantes(int filas, int columnas, unsigned char* tablero);

// creacion y eliminacion puntual
unsigned char* InicializarTablero(int filas, int columnas, int& bytes_reservados);
void EliminarFicha(int p, unsigned char* tablero, int bytes_reservados);

// mostrar tablero
char ConvertirFicha(unsigned char ficha);
void MostrarTablero(int filas, int columnas, unsigned char* tablero, int bytes_reservados);
void MostrarTableroBinario(int filas, int columnas, unsigned char* tablero);

// deteccion y resolucion de combinaciones
int DetectarCombinaciones(int filas, int columnas, unsigned char* tablero, int bytes_reservados, bool* marcadas);
int EliminarMarcadas(int filas, int columnas, unsigned char* tablero, bool* marcadas);
void ReorganizarTablero(int filas, int columnas, unsigned char* tablero, int bytes_reservados);
void RellenarTablero(int filas, int columnas, unsigned char* tablero, int bytes_reservados);
void ResolverCascadas(int filas, int columnas, unsigned char* tablero,
                      int bytes_reservados, int& combinaciones, int& fichas_eliminadas,
                      int& puntuacion, int& cascadas);

// modificacion de estructura de filas y columnas
void EliminarFila(int posicion_fila, int& filas, int columnas,
                 unsigned char*& tablero, int& bytes_reservados);
void EliminarColumna(int posicion_columna, int filas, int& columnas,
                     unsigned char*& tablero, int& bytes_reservados);
void AgregarFila(int posicion_fila, int& filas, int columnas,
                 unsigned char*& tablero, int& bytes_reservados);
void AgregarColumna(int posicion_columna, int filas, int& columnas,
                    unsigned char*& tablero, int& bytes_reservados);

#endif // LOGICA_H
