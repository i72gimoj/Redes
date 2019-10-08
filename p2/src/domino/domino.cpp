#include "domino.hpp"

Domino::Domino()
{
    this->generarFichas();
    this->repartirFichas();
}

void Domino::generarFichas()
{
    int x = 0, y = 0;
    Ficha ficha(x, y);
    for (size_t i = 0; i < 28; i++)
    {
        ficha.setFirst(x);
        ficha.setSecond(y);
        this->drawPool.push_back(ficha);
        if (y == 6)
        {
            y = x;
            x++;
        }
        y++;
    }
}

void Domino::mostrarDrawPool()
{
    for (size_t i = 0; i < this->drawPool.size(); i++)
        std::cout << this->drawPool[i];
    std::cout << std::endl;
}

bool Domino::estanRepartidas()
{
    if (this->drawPool.size() == 28)
        return true;
    return false;
}

void Domino::printPlayer(int player)
{
    if (player == 1)
        for (size_t i = 0; i < this->playerOne.size(); i++)
            std::cout << i + 1 << ". " << this->playerOne[i] << std::endl;
    else
        for (size_t i = 0; i < this->playerTwo.size(); i++)
            std::cout << i + 1 << ". " << this->playerTwo[i] << std::endl;
}

void Domino::repartirFichas()
{
    if (this->estanRepartidas())
    {
        this->drawPool.clear();
        this->playerOne.clear();
        this->playerTwo.clear();
        this->generarFichas();
    }

    int random = 28;
    int nrandom;
    srand(time(NULL));
    for (size_t i = 0; i < 14; i++)
    {
        nrandom = rand() % random;
        if (i < 7)
            this->playerOne.push_back(this->drawPool[nrandom]);
        else
            this->playerTwo.push_back(this->drawPool[nrandom]);
        random--;
        this->drawPool.erase(this->drawPool.begin() + nrandom);
    }
}

void Domino::mostrarTablero()
{
    this->tablero.printTablero();
}

bool Domino::debesRobar(int player)
{
    if (this->tablero.isEmpty())
        return false;
    else
    {
        if (player == 1)
        {
            for (size_t i = 0; i < this->playerOne.size(); i++)
            {
                if ((this->playerOne[i].getFirst() == this->tablero.getExtremoIzquierdo()) or (this->playerOne[i].getFirst() == this->tablero.getExtremoDerecho()))
                    return false;
                else if ((this->playerOne[i].getSecond() == this->tablero.getExtremoIzquierdo()) or (this->playerOne[i].getSecond() == this->tablero.getExtremoDerecho()))
                    return false;
            }
            return true;
        }
        else
        {
            for (size_t i = 0; i < this->playerTwo.size(); i++)
            {
                if ((this->playerTwo[i].getFirst() == this->tablero.getExtremoIzquierdo()) or (this->playerTwo[i].getFirst() == this->tablero.getExtremoDerecho()))
                    return false;
                else if ((this->playerTwo[i].getSecond() == this->tablero.getExtremoIzquierdo()) or (this->playerTwo[i].getSecond() == this->tablero.getExtremoDerecho()))
                    return false;
            }
            return true;
        }
    }
}

void Domino::ponerFicha(int player)
{
    int opt;
    if (player == 1)
    {
        if (!debesRobar(1))
        {
            std::cout << "El tablero está así:" << std::endl;
            this->mostrarTablero();
            std::cout << std::endl;
            this->printPlayer(1);
            std::cout << "Seleccione una opción => ";
            std::cin >> opt;
            if (((opt - 1) > 0) and ((opt - 1) < this->playerOne.size()))
            {
                if (this->tablero.ponerFicha(this->playerOne[opt - 1]))
                {
                    this->playerOne.erase(this->playerOne.begin() + (opt - 1));
                    if (this->playerOne.size() == 0)
                    {
                        std::cout << "Ha ganado el jugador 1" << std::endl;
                        this->repartirFichas();
                    }
                }
                else
                    std::cout << "No puedes poner esa ficha" << std::endl;
            }
            else
                std::cout << "Opción incorrecta" << std::endl;
        }
        else
        {
            std::cout << "No tienes fichas, debes robar" << std::endl;
        }
    }
    else
    {
        if (!debesRobar(2))
        {
            std::cout << "El tablero está así:" << std::endl;
            this->mostrarTablero();
            std::cout << std::endl;
            this->printPlayer(2);
            std::cout << "Seleccione una opción => ";
            std::cin >> opt;
            if (((opt - 1) > 0) and ((opt - 1) < this->playerTwo.size()))
            {
                if (this->tablero.ponerFicha(this->playerTwo[opt - 1]))
                {
                    this->playerTwo.erase(this->playerTwo.begin() + (opt - 1));
                    if (this->playerTwo.size() == 0)
                    {
                        std::cout << "Ha ganado el jugador 2" << std::endl;
                        this->repartirFichas();
                    }
                }
                else
                    std::cout << "No puedes poner esa ficha" << std::endl;
            }
            else
                std::cout << "Opción incorrecta" << std::endl;
        }
        else
        {
            std::cout << "No tienes fichas, debes robar" << std::endl;
        }
    }
}

void Domino::robarFicha(int player)
{
    int nrandom;
    srand(time(NULL));
    if (player == 1)
    {
        nrandom = rand() % this->drawPool.size();
        this->playerOne.push_back(this->drawPool[nrandom]);
        this->drawPool.erase(this->drawPool.begin() + nrandom);
    }
    else
    {
        nrandom = rand() % this->drawPool.size();
        this->playerTwo.push_back(this->drawPool[nrandom]);
        this->drawPool.erase(this->drawPool.begin() + nrandom);
    }
}
