#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cassert>
#include "logica.h"
using namespace std;

const int CANTIDAD_FICHAS = 6;
const int CODIGOS_FICHAS[CANTIDAD_FICHAS] = {A, B, C, D, E, F};

int CodigoFichaAleatoria()
{
    // se usa rechazo para que las seis fichas tengan la misma posibilidad
    int limite = RAND_MAX - (RAND_MAX % CANTIDAD_FICHAS);
    int numero;

    do
    {
        numero = rand();
    } while (numero >= limite);

    return CODIGOS_FICHAS[numero % CANTIDAD_FICHAS];
}

// acceso a bits
int CalcularBytesNecesarios(int filas, int columnas)
{
    if (filas <= 0 || columnas <= 0)
    {
        return 0;
    }

    int total_bits = filas * columnas * 3;
    int bytes = total_bits / 8;

    if (total_bits % 8 != 0)
    {
        bytes++;
    }

    return bytes;
}

unsigned char ObtenerFicha(int p, unsigned char* tablero, int bytes_reservados)
{
    if (p < 0 || tablero == 0 || bytes_reservados <= 0)
    {
        return VACIO;
    }

    int bitinicio = p * 3;
    int byteinicio = bitinicio / 8;
    int offset = bitinicio % 8;

    if (byteinicio < 0 || byteinicio >= bytes_reservados)
    {
        return VACIO;
    }

    assert(byteinicio < bytes_reservados);

    if (offset <= 5)
    {
        unsigned char valor = tablero[byteinicio] >> offset;
        return valor & 7;
    }

    // la ficha queda repartida entre dos bytes
    if (byteinicio + 1 >= bytes_reservados)
    {
        return VACIO;
    }

    assert(byteinicio + 1 < bytes_reservados);

    int bits_primer_byte = 8 - offset;
    int bits_segundo_byte = 3 - bits_primer_byte;

    unsigned char mascara1 = (1 << bits_primer_byte) - 1;
    unsigned char mascara2 = (1 << bits_segundo_byte) - 1;

    unsigned char parte1 = (tablero[byteinicio] >> offset) & mascara1;
    unsigned char parte2 = tablero[byteinicio + 1] & mascara2;

    unsigned char valor = parte1 | (parte2 << bits_primer_byte);
    return valor & 7;
}

void PonerFicha(int p, unsigned char* tablero, int valornuevo, int bytes_reservados)
{
    if (p < 0 || tablero == 0 || bytes_reservados <= 0)
    {
        return;
    }

    valornuevo = valornuevo & 7;

    int bitinicio = p * 3;
    int byteinicio = bitinicio / 8;
    int offset = bitinicio % 8;

    if (byteinicio < 0 || byteinicio >= bytes_reservados)
    {
        return;
    }

    assert(byteinicio < bytes_reservados);

    if (offset <= 5)
    {
        unsigned char mascara = 7 << offset;
        tablero[byteinicio] = tablero[byteinicio] & (unsigned char)(~mascara);
        tablero[byteinicio] = tablero[byteinicio] |
                               (unsigned char)(valornuevo << offset);
    }
    else
    {
        // la ficha queda repartida entre dos bytes
        if (byteinicio + 1 >= bytes_reservados)
        {
            return;
        }

        assert(byteinicio + 1 < bytes_reservados);

        int bits_primer_byte = 8 - offset;
        int bits_segundo_byte = 3 - bits_primer_byte;

        unsigned char mascara1 = (1 << bits_primer_byte) - 1;
        unsigned char mascara2 = (1 << bits_segundo_byte) - 1;

        tablero[byteinicio] = tablero[byteinicio] &
                              (unsigned char)(~(mascara1 << offset));
        tablero[byteinicio] = tablero[byteinicio] |
                              (unsigned char)((valornuevo & mascara1) << offset);

        tablero[byteinicio + 1] = tablero[byteinicio + 1] &
                                  (unsigned char)(~mascara2);
        tablero[byteinicio + 1] = tablero[byteinicio + 1] |
                                  (unsigned char)((valornuevo >> bits_primer_byte) & mascara2);
    }
}

void LimpiarBitsSobrantes(int filas, int columnas, unsigned char* tablero)
{
    if (tablero == 0)
    {
        return;
    }

    int total_bits = filas * columnas * 3;
    int sobrantes = total_bits % 8;

    if (sobrantes != 0)
    {
        int ultimo_byte = total_bits / 8;
        unsigned char mascara = (1 << sobrantes) - 1;
        tablero[ultimo_byte] = tablero[ultimo_byte] & mascara;
    }
}

// creacion y eliminacion puntual
unsigned char* InicializarTablero(int filas, int columnas, int& bytes_reservados)
{
    bytes_reservados = CalcularBytesNecesarios(filas, columnas);

    if (bytes_reservados == 0)
    {
        return 0;
    }

    unsigned char* tablero = new unsigned char[bytes_reservados]();

    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas; c++)
        {
            int p = f * columnas + c;
            PonerFicha(p, tablero, CodigoFichaAleatoria(), bytes_reservados);
        }
    }

    LimpiarBitsSobrantes(filas, columnas, tablero);
    return tablero;
}

void EliminarFicha(int p, unsigned char* tablero, int bytes_reservados)
{
    PonerFicha(p, tablero, VACIO, bytes_reservados);
}

// mostrar tablero
char ConvertirFicha(unsigned char ficha)
{
    if (ficha == A) return 'A';
    if (ficha == B) return 'B';
    if (ficha == C) return 'C';
    if (ficha == D) return 'D';
    if (ficha == E) return 'E';
    if (ficha == F) return 'F';

    return '.';
}

void MostrarTablero(int filas, int columnas, unsigned char* tablero, int bytes_reservados)
{
    cout << "\n     ";

    for (int c = 0; c < columnas; c++)
    {
        cout << c << " ";
    }

    cout << endl;
    cout << "    ";

    for (int c = 0; c < columnas; c++)
    {
        cout << "--";
    }

    cout << endl;

    for (int f = 0; f < filas; f++)
    {
        cout << " " << f << " | ";

        for (int c = 0; c < columnas; c++)
        {
            int p = f * columnas + c;
            cout << ConvertirFicha(ObtenerFicha(p, tablero, bytes_reservados)) << " ";
        }

        cout << endl;
    }
}

void MostrarTableroBinario(int filas, int columnas, unsigned char* tablero)
{
    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas);

    cout << "\nmemoria en bits:" << endl;

    for (int i = 0; i < bytes_necesarios; i++)
    {
        cout << "byte " << i << ": ";

        for (int bit = 7; bit >= 0; bit--)
        {
            unsigned char valor = (tablero[i] >> bit) & 1;
            cout << (int)valor;
        }

        cout << endl;
    }
}

// deteccion y resolucion de combinaciones
int DetectarCombinaciones(int filas, int columnas, unsigned char* tablero, int bytes_reservados, bool* marcadas)
{
    if (marcadas == 0)
    {
        return 0;
    }

    int total_posiciones = filas * columnas;

    for (int p = 0; p < total_posiciones; p++)
    {
        marcadas[p] = false;
    }

    int combinaciones = 0;

    // busca combinaciones horizontales
    for (int f = 0; f < filas; f++)
    {
        int c = 0;

        while (c < columnas)
        {
            int p = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p, tablero, bytes_reservados);

            if (ficha == VACIO)
            {
                c++;
                continue;
            }

            int inicio = c;
            int cantidad = 1;

            while (c + cantidad < columnas)
            {
                int siguiente = f * columnas + c + cantidad;

                if (ObtenerFicha(siguiente, tablero, bytes_reservados) != ficha)
                {
                    break;
                }

                cantidad++;
            }

            if (cantidad >= 3)
            {
                combinaciones++;

                for (int i = 0; i < cantidad; i++)
                {
                    int marcar = f * columnas + inicio + i;
                    marcadas[marcar] = true;
                }
            }

            c += cantidad;
        }
    }

    // busca combinaciones verticales
    for (int c = 0; c < columnas; c++)
    {
        int f = 0;

        while (f < filas)
        {
            int p = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p, tablero, bytes_reservados);

            if (ficha == VACIO)
            {
                f++;
                continue;
            }

            int inicio = f;
            int cantidad = 1;

            while (f + cantidad < filas)
            {
                int siguiente = (f + cantidad) * columnas + c;

                if (ObtenerFicha(siguiente, tablero, bytes_reservados) != ficha)
                {
                    break;
                }

                cantidad++;
            }

            if (cantidad >= 3)
            {
                combinaciones++;

                for (int i = 0; i < cantidad; i++)
                {
                    int marcar = (inicio + i) * columnas + c;
                    marcadas[marcar] = true;
                }
            }

            f += cantidad;
        }
    }

    return combinaciones;
}

int EliminarMarcadas(int filas, int columnas, unsigned char* tablero, bool* marcadas)
{
    int eliminadas = 0;

    for (int p = 0; p < filas * columnas; p++)
    {
        if (marcadas[p])
        {
            EliminarFicha(p, tablero, CalcularBytesNecesarios(filas, columnas));
            eliminadas++;
        }
    }

    return eliminadas;
}

void ReorganizarTablero(int filas, int columnas, unsigned char* tablero, int bytes_reservados)
{
    // las fichas bajan dentro de cada columna
    for (int c = 0; c < columnas; c++)
    {
        int destino = filas - 1;

        for (int f = filas - 1; f >= 0; f--)
        {
            int p_origen = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero, bytes_reservados);

            if (ficha != VACIO)
            {
                int p_destino = destino * columnas + c;
                PonerFicha(p_destino, tablero, ficha, bytes_reservados);
                destino--;
            }
        }

        for (int f = destino; f >= 0; f--)
        {
            int p_vacio = f * columnas + c;
            PonerFicha(p_vacio, tablero, VACIO, bytes_reservados);
        }
    }

    LimpiarBitsSobrantes(filas, columnas, tablero);
}

void RellenarTablero(int filas, int columnas, unsigned char* tablero, int bytes_reservados)
{
    // los espacios vacios se llenan desde arriba
    for (int c = 0; c < columnas; c++)
    {
        for (int f = 0; f < filas; f++)
        {
            int p = f * columnas + c;

            if (ObtenerFicha(p, tablero, bytes_reservados) == VACIO)
            {
                PonerFicha(p, tablero, CodigoFichaAleatoria(), bytes_reservados);
            }
        }
    }

    LimpiarBitsSobrantes(filas, columnas, tablero);
}

void ResolverCascadas(int filas, int columnas, unsigned char* tablero,
                      int bytes_reservados, int& combinaciones, int& fichas_eliminadas,
                      int& puntuacion, int& cascadas)
{
    int total_posiciones = filas * columnas;
    bool* marcadas = new bool[total_posiciones];

    bool hay_combinacion = true;

    while (hay_combinacion)
    {
        int combinaciones_actuales = DetectarCombinaciones(filas, columnas,
                                                            tablero, bytes_reservados, marcadas);

        if (combinaciones_actuales == 0)
        {
            hay_combinacion = false;
        }
        else
        {
            cascadas++;
            combinaciones += combinaciones_actuales;

            int eliminadas = EliminarMarcadas(filas, columnas, tablero, marcadas);
            fichas_eliminadas += eliminadas;

            // cada ficha vale 10 puntos y cada combinacion vale 20
            puntuacion += eliminadas * 10;
            puntuacion += combinaciones_actuales * 20;

            // cada cascada despues de la primera agrega 50 puntos
            if (cascadas > 1)
            {
                puntuacion += 50;
            }

            ReorganizarTablero(filas, columnas, tablero, bytes_reservados);
            RellenarTablero(filas, columnas, tablero, bytes_reservados);
        }
    }

    delete[] marcadas;
}

// modificacion de estructura de filas y columnas
void EliminarFila(int posicion_fila, int& filas, int columnas,
                  unsigned char*& tablero, int& bytes_reservados)
{
    if (filas <= 0 || columnas <= 0 || tablero == 0 || bytes_reservados <= 0)
    {
        return;
    }


    if (posicion_fila < 0 || posicion_fila >= filas || filas <= 1)
    {
        return;
    }

    int p_destino = 0;

    for (int f = 0; f < filas; f++)
    {
        if (f == posicion_fila)
        {
            continue;
        }

        for (int c = 0; c < columnas; c++)
        {
            int p_origen = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero, bytes_reservados);

            if (p_destino != p_origen)
            {
                PonerFicha(p_destino, tablero, ficha, bytes_reservados);
            }

            p_destino++;
        }
    }

    filas--;

    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas);

    if (bytes_necesarios * 100 < bytes_reservados * 65)
    {
        unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();

        for (int i = 0; i < bytes_necesarios; i++)
        {
            nuevotablero[i] = tablero[i];
        }

        delete[] tablero;
        tablero = nuevotablero;
        bytes_reservados = bytes_necesarios;
    }

    LimpiarBitsSobrantes(filas, columnas, tablero);
}

void EliminarColumna(int posicion_columna, int filas, int& columnas,
                     unsigned char*& tablero, int& bytes_reservados)
{
    if (filas <= 0 || columnas <= 0 || tablero == 0 || bytes_reservados <= 0)
    {
        return;
    }


    if (posicion_columna < 0 || posicion_columna >= columnas || columnas <= 1)
    {
        return;
    }

    int columnas_viejas = columnas;
    int p_destino = 0;

    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas_viejas; c++)
        {
            if (c == posicion_columna)
            {
                continue;
            }

            int p_origen = f * columnas_viejas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero, bytes_reservados);
            PonerFicha(p_destino, tablero, ficha, bytes_reservados);
            p_destino++;
        }
    }

    columnas--;

    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas);

    if (bytes_necesarios * 100 < bytes_reservados * 65)
    {
        unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();

        for (int i = 0; i < bytes_necesarios; i++)
        {
            nuevotablero[i] = tablero[i];
        }

        delete[] tablero;
        tablero = nuevotablero;
        bytes_reservados = bytes_necesarios;
    }

    LimpiarBitsSobrantes(filas, columnas, tablero);
}

void AgregarFila(int posicion_fila, int& filas, int columnas,
                 unsigned char*& tablero, int& bytes_reservados)
{
    if (filas <= 0 || columnas <= 0 || tablero == 0 || bytes_reservados <= 0)
    {
        return;
    }


    if (posicion_fila < 0 || posicion_fila > filas)
    {
        return;
    }

    int bytes_necesarios = CalcularBytesNecesarios(filas + 1, columnas);
    unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();

    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas; c++)
        {
            int p_origen = f * columnas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero, bytes_reservados);
            int fila_destino;

            if (f >= posicion_fila)
            {
                fila_destino = f + 1;
            }
            else
            {
                fila_destino = f;
            }

            int p_destino = fila_destino * columnas + c;
            PonerFicha(p_destino, nuevotablero, ficha, bytes_necesarios);
        }
    }

    for (int c = 0; c < columnas; c++)
    {
        int p_destino = posicion_fila * columnas + c;
        PonerFicha(p_destino, nuevotablero, CodigoFichaAleatoria(), bytes_necesarios);
    }

    delete[] tablero;
    tablero = nuevotablero;
    bytes_reservados = bytes_necesarios;
    filas++;

    LimpiarBitsSobrantes(filas, columnas, tablero);
}

void AgregarColumna(int posicion_columna, int filas, int& columnas,
                    unsigned char*& tablero, int& bytes_reservados)
{
    if (filas <= 0 || columnas <= 0 || tablero == 0 || bytes_reservados <= 0)
    {
        return;
    }


    if (posicion_columna < 0 || posicion_columna > columnas)
    {
        return;
    }

    int columnas_viejas = columnas;
    int bytes_necesarios = CalcularBytesNecesarios(filas, columnas_viejas + 1);
    unsigned char* nuevotablero = new unsigned char[bytes_necesarios]();

    for (int f = 0; f < filas; f++)
    {
        for (int c = 0; c < columnas_viejas; c++)
        {
            int p_origen = f * columnas_viejas + c;
            unsigned char ficha = ObtenerFicha(p_origen, tablero, bytes_reservados);
            int columna_destino;

            if (c < posicion_columna)
            {
                columna_destino = c;
            }
            else
            {
                columna_destino = c + 1;
            }

            int p_destino = f * (columnas_viejas + 1) + columna_destino;
            PonerFicha(p_destino, nuevotablero, ficha, bytes_necesarios);
        }
    }

    for (int f = 0; f < filas; f++)
    {
        int p_destino = f * (columnas_viejas + 1) + posicion_columna;
        PonerFicha(p_destino, nuevotablero, CodigoFichaAleatoria(), bytes_necesarios);
    }

    delete[] tablero;
    tablero = nuevotablero;
    bytes_reservados = bytes_necesarios;
    columnas++;

    LimpiarBitsSobrantes(filas, columnas, tablero);
}
