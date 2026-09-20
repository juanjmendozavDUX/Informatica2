#include <iostream>
#include <cstdlib>
#include <ctime>
#include "logica.h"
using namespace std;

void Interfazjugador(int& filas, int& columnas, unsigned char*& tablero, int& bytes_reservados,
                     int& eliminaciones_usuario, int& fichas_eliminadas, int& combinaciones, int& cascadas_totales, int& puntuacion);

void LimpiarEntrada()
{
    cin.clear();
    cin.ignore(10000, '\n');
}

bool LeerPosicion(int& posicion, int minimo, int maximo)
{
    cin >> posicion;

    if (cin.fail())
    {
        LimpiarEntrada();
        cout << "dato incorrecto" << endl;
        return false;
    }

    if (posicion < minimo || posicion > maximo)
    {
        cout << "posicion no valida" << endl;
        return false;
    }

    return true;
}

void MostrarEstadisticas(int filas, int columnas, int eliminaciones_usuario, int fichas_eliminadas,
                         int combinaciones, int cascadas_ultima_jugada, int cascadas_totales, int puntuacion, int bytes_reservados)
{
    cout << "\n-------------------------------" << endl;
    cout << "           estadisticas" << endl;
    cout << "-------------------------------" << endl;
    cout << "filas actuales: " << filas << endl;
    cout << "columnas actuales: " << columnas << endl;
    cout << "eliminaciones del jugador: " << eliminaciones_usuario << endl;
    cout << "fichas eliminadas: " << fichas_eliminadas << endl;
    cout << "combinaciones detectadas: " << combinaciones << endl;
    cout << "cascadas de la ultima jugada: " << cascadas_ultima_jugada << endl;
    cout << "cascadas totales: " << cascadas_totales << endl;
    cout << "puntuacion: " << puntuacion << endl;
    cout << "bytes reservados: " << bytes_reservados << endl;
    cout << "-------------------------------" << endl;
}

void ResolverDespuesDeCambio(int filas, int columnas,
                             unsigned char* tablero,
                             int& combinaciones, int& fichas_eliminadas,
                             int& puntuacion, int& cascadas, int& cascadas_totales)
{
    // despues de un cambio se reorganiza y se revisan las cascadas
    ReorganizarTablero(filas, columnas, tablero);
    RellenarTablero(filas, columnas, tablero);

    cascadas = 0;
    ResolverCascadas(filas, columnas, tablero,
                     combinaciones, fichas_eliminadas,
                     puntuacion, cascadas);

    cascadas_totales += cascadas;
}

int main()
{
    srand((unsigned)time(0));

    bool salir = false;

    do
    {
        int num = 0;

        cout << "\n";
        cout << " -------------------\n";
        cout << " |  MENU PRINCIPAL  |\n";
        cout << " -------------------\n";
        cout << "1. comenzar juego" << endl;
        cout << "2. salir" << endl;
        cout << "elija una opcion: ";

        cin >> num;

        if (cin.fail())
        {
            LimpiarEntrada();
            cout << "opcion no valida, intente de nuevo." << endl;
            continue;
        }

        if (num == 1)
        {
            int filas;
            int columnas;

            cout << "ingrese cantidad de filas: ";
            cin >> filas;

            if (cin.fail() || filas < 1)
            {
                LimpiarEntrada();
                cout << "cantidad de filas no valida" << endl;
                continue;
            }

            cout << "ingrese cantidad de columnas: ";
            cin >> columnas;

            if (cin.fail() || columnas < 1)
            {
                LimpiarEntrada();
                cout << "cantidad de columnas no valida" << endl;
                continue;
            }

            int bytes_reservados = 0;
            unsigned char* tablero = InicializarTablero(filas, columnas, bytes_reservados);

            int eliminaciones_usuario = 0;
            int fichas_eliminadas = 0;
            int combinaciones = 0;
            int cascadas_totales = 0;
            int puntuacion = 0;

            // se estabiliza el tablero inicial sin contar puntos
            int combinaciones_iniciales = 0;
            int fichas_iniciales = 0;
            int puntuacion_inicial = 0;
            int cascadas_iniciales = 0;

            ResolverCascadas(filas, columnas, tablero,
                             combinaciones_iniciales, fichas_iniciales,
                             puntuacion_inicial, cascadas_iniciales);

            Interfazjugador(filas, columnas, tablero, bytes_reservados,
                            eliminaciones_usuario, fichas_eliminadas,
                            combinaciones, cascadas_totales, puntuacion);

            delete[] tablero;
        }
        else if (num == 2)
        {
            cout << "saliendo del programa..." << endl;
            salir = true;
        }
        else
        {
            cout << "opcion no valida, intente de nuevo." << endl;
        }

    } while (!salir);

    return 0;
}

void Interfazjugador(int& filas, int& columnas,
                     unsigned char*& tablero, int& bytes_reservados,
                     int& eliminaciones_usuario, int& fichas_eliminadas,
                     int& combinaciones, int& cascadas_totales, int& puntuacion)
{
    bool volver = false;

    do
    {
        cout << "\n\n";
        cout << "===============================" << endl;
        cout << "             tablero" << endl;
        cout << "===============================" << endl;

        MostrarTablero(filas, columnas, tablero);
        MostrarTableroBinario(filas, columnas, tablero);

        int cascadas_ultima_jugada = 0;

        cout << "\nELIJA LA OPCION PARA JUGAR" << endl;
        cout << "1- ELIMINA UNA FILA" << endl;
        cout << "2- ELIMINA UNA COLUMNA" << endl;
        cout << "3- ELIMINA UNA FICHA" << endl;
        cout << "4- AGREGA UNA FILA" << endl;
        cout << "5- AGREGA UNA COLUMNA" << endl;
        cout << "6- Volver a menu principal" << endl;
        cout << "opcion: ";

        int opciones;
        cin >> opciones;

        if (cin.fail())
        {
            LimpiarEntrada();
            cout << "dato incorrecto, vuelva a intentarlo" << endl;
            continue;
        }

        switch (opciones)
        {
        case 1:
        {
            int posicion;
            cout << "elija la fila que desea eliminar (0 a " << filas - 1 << "): ";

            if (LeerPosicion(posicion, 0, filas - 1) && filas > 1)
            {
                EliminarFila(posicion, filas, columnas,
                             tablero, bytes_reservados);
                eliminaciones_usuario++;

                ResolverDespuesDeCambio(filas, columnas, tablero,
                                        combinaciones, fichas_eliminadas,
                                        puntuacion, cascadas_ultima_jugada, cascadas_totales);
            }
            else if (filas == 1)
            {
                cout << "no puede eliminar la ultima fila" << endl;
            }
            break;
        }

        case 2:
        {
            int posicion;
            cout << "elija la columna que desea eliminar (0 a " << columnas - 1 << "): ";

            if (LeerPosicion(posicion, 0, columnas - 1) && columnas > 1)
            {
                EliminarColumna(posicion, filas, columnas,
                                tablero, bytes_reservados);
                eliminaciones_usuario++;

                ResolverDespuesDeCambio(filas, columnas, tablero,
                                        combinaciones, fichas_eliminadas,
                                        puntuacion, cascadas_ultima_jugada, cascadas_totales);
            }
            else if (columnas == 1)
            {
                cout << "no puede eliminar la ultima columna" << endl;
            }
            break;
        }

        case 3:
        {
            int fila;
            int columna;

            cout << "ingrese fila de la ficha (0 a " << filas - 1 << "): ";
            if (!LeerPosicion(fila, 0, filas - 1))
            {
                break;
            }

            cout << "ingrese columna de la ficha (0 a " << columnas - 1 << "): ";
            if (!LeerPosicion(columna, 0, columnas - 1))
            {
                break;
            }

            int p = fila * columnas + columna;

            if (ObtenerFicha(p, tablero) == VACIO)
            {
                cout << "esa posicion esta vacia" << endl;
                break;
            }

            EliminarFicha(p, tablero);
            eliminaciones_usuario++;

            ResolverDespuesDeCambio(filas, columnas, tablero,
                                    combinaciones, fichas_eliminadas,
                                    puntuacion, cascadas_ultima_jugada, cascadas_totales);
            break;
        }

        case 4:
        {
            int posicion;
            cout << "elija donde agregar la fila (0 a " << filas << "): ";

            if (LeerPosicion(posicion, 0, filas))
            {
                AgregarFila(posicion, filas, columnas,
                            tablero, bytes_reservados);

                ResolverDespuesDeCambio(filas, columnas, tablero,
                                        combinaciones, fichas_eliminadas,
                                        puntuacion, cascadas_ultima_jugada, cascadas_totales);
            }
            break;
        }

        case 5:
        {
            int posicion;
            cout << "elija donde agregar la columna (0 a " << columnas << "): ";

            if (LeerPosicion(posicion, 0, columnas))
            {
                AgregarColumna(posicion, filas, columnas,
                               tablero, bytes_reservados);

                ResolverDespuesDeCambio(filas, columnas, tablero,
                                        combinaciones, fichas_eliminadas,
                                        puntuacion, cascadas_ultima_jugada, cascadas_totales);
            }
            break;
        }

        case 6:
            cout << "VOLVIENDO A MENU PRINCIPAL" << endl;
            volver = true;
            break;

        default:
            cout << "dato incorrecto, vuelva a intentarlo" << endl;
            break;
        }

        if (!volver)
        {
            MostrarEstadisticas(filas, columnas,
                                eliminaciones_usuario, fichas_eliminadas,
                                combinaciones, cascadas_ultima_jugada,
                                cascadas_totales, puntuacion, bytes_reservados);
        }

    } while (!volver);
}
