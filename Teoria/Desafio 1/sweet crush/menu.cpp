#include <iostream>

using namespace std;

void Interfazjugador();

int main()
{
    int const A = 0b111;
    int const B = 0b001;
    int const C = 0b011;
    int const D = 0b101;
    int const E = 0b100;
    int const F = 0b110;
    int const libre = 0b000;
    int const especial = 0b010;

    bool salir = false;

    do {
        int num = 0;

        cout << libre << endl;
        cout << " -------------------\n";
        cout << " |  MENU PRINCIPAL |  \n";
        cout << " -------------------\n";
        cout << "1. Comenzar Juego" << endl;
        cout << "2. Salir" << endl;
        cout << "Elija una opcion: " << endl;

        cin >> num;

        if (num == 1) {

            // Entramos al menu del jugador
            Interfazjugador();

        }
        else if (num == 2) {

            cout << "Saliendo del programa..." << endl;
            salir = true;

        }
        else {

            cout << "Opcion no valida, intente de nuevo." << endl;
        }

    } while (!salir);

    return 0;
}


void Interfazjugador()
{
    bool volver = false;

    do {

        int opciones;

        cout << "\nELIJA LA OPCION PARA JUGAR" << endl;
        cout << "1- ELIMINA UNA FILA" << endl;
        cout << "2- ELIMINA UNA COLUMNA" << endl;
        cout << "3- ELIMINA UNA FICHA" << endl;
        cout << "4- AGREGA UNA FILA" << endl;
        cout << "5- AGREGA UNA COLUMNA" << endl;
        cout << "6- Volver a menu principal" << endl;

        cin >> opciones;

        switch (opciones) {

        case 1:
            cout << "Elimina una fila" << endl;
            break;

        case 2:
            cout << "Elimina una columna" << endl;
            break;

        case 3:
            cout << "Elimina una ficha" << endl;
            break;

        case 4:
            cout << "Agrega una fila" << endl;
            break;

        case 5:
            cout << "Agrega una columna" << endl;
            break;

        case 6:
            cout << "VOLVIENDO A MENU PRINCIPAL" << endl;
            volver = true;
            break;

        default:
            cout << "Dato incorrecto, vuelva a intentarlo" << endl;
            break;
        }

    } while (!volver);
}