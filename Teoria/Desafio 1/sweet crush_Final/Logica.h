#ifndef LOGICA_H
#define LOGICA_H

// Cada ficha ocupa exactamente 3 bits (valores posibles: 0 a 7).
// 0 = posicion vacia. 1 a 6 = los seis tipos de ficha. El 7 queda libre.
const int VACIO = 0;
const int A = 1;
const int B = 2;
const int C = 3;
const int D = 4;
const int E = 5;
const int F = 6;

// ---- Acceso a bits ----
int CalcularBytesNecesarios(int filas, int columnas);
unsigned char ObtenerFicha(int p, unsigned char* tablero);
void PonerFicha(int p, unsigned char* tablero, int valornuevo);

// ---- Creacion / eliminacion puntual ----
unsigned char* InicializarTablero(int filas, int columnas, int& bytes_reservados);
void EliminarFicha(int p, unsigned char* tablero);

// ---- Visualizacion ----
char ConvertirFicha(unsigned char ficha);
void MostrarTablero(int filas, int columnas, unsigned char* tablero);
void MostrarTableroBinario(int filas, int columnas, unsigned char* tablero);

// ---- Deteccion y resolucion de combinaciones ----
int DetectarCombinaciones(int filas, int columnas, unsigned char* tablero, bool* marcadas);
int EliminarMarcadas(int filas, int columnas, unsigned char* tablero, bool* marcadas);
void ReorganizarTablero(int filas, int columnas, unsigned char* tablero);
void RellenarTablero(int filas, int columnas, unsigned char* tablero);
void ResolverCascadas(int filas, int columnas, unsigned char* tablero,
                      int& combinaciones, int& fichas_eliminadas,
                      int& puntuacion, int& cascadas);

// ---- Modificacion de estructura (filas / columnas) ----
void EliminarFila(int posicion_fila, int& filas, int columnas, unsigned char*& tablero, int& bytes_reservados);
void EliminarColumna(int posicion_columna, int filas, int& columnas, unsigned char*& tablero, int& bytes_reservados);
void AgregarFila(int posicion_fila, int& filas, int columnas, unsigned char*& tablero, int& bytes_reservados);
void AgregarColumna(int posicion_columna, int filas, int& columnas, unsigned char*& tablero, int& bytes_reservados);

#endif // LOGICA_H
