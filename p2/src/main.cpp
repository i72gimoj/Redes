#include "domino.hpp"

int main(int argc, char const *argv[])
{
    std::string pet = "", aux;
    Ficha ficha;
    bool robado = false;
    char lado;
    int turno = 1;
    Domino partida(1, 2);

    while (pet != "FIN")
    {
        getline(std::cin, pet);

        aux = pet.substr(0, pet.find_first_of(" "));

        if (aux == "COLOCAR-FICHA")
        {
            aux = pet.substr(pet.find_first_of("|"));
            ficha.setFirst(aux[1] - '0');
            ficha.setSecond(aux[3] - '0');
            lado = aux[5];

            std::cout << lado << std::endl;

            if (turno == 1)
            {
                if ((aux = partida.ponerFicha(ficha, lado, 1)) != "-Err. La ficha no puede ser colocada")
                {
                    if (robado)
                        robado = false;

                    turno = 2;
                }
                std::cout << aux << std::endl;
            }
            else
            {
                if ((aux = partida.ponerFicha(ficha, lado, 2)) != "-Err. La ficha no puede ser colocada")
                {
                    if (robado)
                        robado = false;
                    turno = 1;
                }
                std::cout << aux << std::endl;
            }
        }
        else
        {
            if (aux == "ROBAR-FICHA")
            {
                if (robado)
                    std::cout << "Ya ha robado, si no puede colocar pase turno" << std::endl;
                else
                {
                    if (turno == 1)
                    {
                        std::cout << partida.robarFicha(1) << std::endl;
                        robado = true;
                    }
                    else
                    {
                        std::cout << partida.robarFicha(2) << std::endl;
                        robado = true;
                    }
                }
            }
            else
            {
                if (aux == "PASO-TURNO")
                {
                    if (robado)
                        robado = false;

                    if (turno == 1)
                        turno = 2;
                    else
                        turno = 1;
                }
                else
                {
                    std::cout << "-ERR" << std::endl;
                }
                
            }
        }

        std::cout << "Turno del jugador " << turno << std::endl;

        std::cout << "JUG1 " << partida.getFichas(1) << std::endl;
        std::cout << "JUG2 " << partida.getFichas(2) << std::endl << std::endl;

        if (!partida.siguePartida())
        {
            if (turno == 1)
                turno == 2;
            else
                turno == 1;

            std::cout << "La partida ha finalizado, ha ganado el jugador " << turno << std::endl;
            pet = "FIN";
        }
    }

    return 0;
}
