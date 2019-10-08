#include "domino.hpp"

int main(int argc, char const *argv[])
{
    Domino partida;
    int opt = 0;

    while (opt != 9)
    {
        std::cout << "\n\n\n";
        std::cout << "1. Mostrar tablero" << std::endl;
        std::cout << "2. Mostrar draw pool" << std::endl;
        std::cout << "3. Mostrar mano jugador 1" << std::endl;
        std::cout << "4. Mostrar mano jugador 2" << std::endl;
        std::cout << "5. Dar ficha jugador 1" << std::endl;
        std::cout << "6. Dar ficha jugador 2" << std::endl;
        std::cout << "7. Poner ficha jugador 1" << std::endl;
        std::cout << "8. Poner ficha jugador 2" << std::endl;
        std::cout << "9. Salir" << std::endl;
        std::cout << "Introduzca la opción" << std::endl;
        std::cin >> opt;
        std::cout << "\n\n\n";

        switch (opt)
        {
        case 1:
            partida.mostrarTablero();
            break;

        case 2:
            partida.mostrarDrawPool();
            break;

        case 3:
            partida.printPlayer(1);
            break;

        case 4:
            partida.printPlayer(2);
            break;

        case 5:
            partida.robarFicha(1);
            break;

        case 6:
            partida.robarFicha(2);
            break;

        case 7:
            partida.ponerFicha(1);
            break;

        case 8:
            partida.ponerFicha(2);
            break;

        case 9:
            std::cout << "Saliendo" << std::endl;
            break;

        default:
            break;
        }
    }

    return 0;
}
