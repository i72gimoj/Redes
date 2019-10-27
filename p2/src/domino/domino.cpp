/**
 * @file domino.cpp
 * @author Iván Rodríguez Aguayo (i72roagi@uco.es)
 * @brief Fichero con el código de las funciones no inline de la clase Domino
 * @version 1.0
 * @date 18-10-2019
 * @copyright Copyright (c) 2019
 */

#include "domino.hpp"

Domino::Domino(int idjug1, int idjug2, int &turno)
{
    this->_jugadores.push_back(JugadorD(idjug1));
    this->_jugadores.push_back(JugadorD(idjug2));
    this->generarFichas();
    this->repartirFichas();
    turno = this->decidirTurno();
}

void Domino::generarFichas()
{
    int x = 6, y = 6;
    Ficha ficha;
    for (int i = 6; i >= 0; i--)
    {
        ficha.setFirst(i);
        ficha.setSecond(i);
        this->_drawPool.push_back(ficha);
    }

    for (size_t i = 0; i < 28; i++)
    {
        if (x != y)
        {
            ficha.setFirst(x);
            ficha.setSecond(y);
            this->_drawPool.push_back(ficha);
            if (y == 0)
            {
                y = x;
                x--;
            }
        }
        y--;
    }
    this->_todas = this->_drawPool;
}

void Domino::repartirFichas()
{
    int random = 28;
    int nrandom;
    srand(time(NULL));
    for (size_t i = 0; i < 14; i++)
    {
        nrandom = rand() % random;
        if (i < 7)
            this->_jugadores[0].pickFicha(this->_drawPool[nrandom]);
        else
            this->_jugadores[1].pickFicha(this->_drawPool[nrandom]);
        random--;
        this->_drawPool.erase(this->_drawPool.begin() + nrandom);
    }
}

std::string Domino::getTablero()
{
    return this->_tablero.getTablero();
}

std::string Domino::ponerFicha(Ficha &ficha, std::string lado, int idJug)
{
    if (this->_tablero.ponerFicha(ficha, lado))
    {
        if (idJug == this->_jugadores[0].getID())
        {
            if (this->_jugadores[0].removeFicha(ficha))
                return this->getTablero();
            else
                return "-Err. La ficha no puede ser colocada";
        }
        else
        {
            if (idJug == this->_jugadores[1].getID())
            {
                if (this->_jugadores[1].removeFicha(ficha))
                    return this->getTablero();
                else
                    return "-Err. La ficha no puede ser colocada";
            }
            else
            {
                return "-Err. La ficha no puede ser colocada";
            }
        }
    }
    else
        return "-Err. La ficha no puede ser colocada";
    return "Fallo";
}

std::string Domino::robarFicha(int idjug)
{
    int nrandom;
    std::string ficha = "FICHA ";
    srand(time(NULL));
    nrandom = rand() % this->_drawPool.size();

    if (this->_jugadores[0].getID() == idjug)
    {
        if (this->_jugadores[0].comprobarFicha(this->_tablero.left(), this->_tablero.right()))
        {
            return "+Ok. No es necesario robar ficha.";
        }
        else
        {
            this->_jugadores[0].pickFicha(this->_drawPool[nrandom]);
            ficha += this->_drawPool[nrandom].getFicha();
            this->_drawPool.erase(this->_drawPool.begin() + nrandom);
            return ficha;
        }
    }
    if (this->_jugadores[1].getID() == idjug)
    {
        if (this->_jugadores[1].comprobarFicha(this->_tablero.left(), this->_tablero.right()))
        {
            return "+Ok. No es necesario robar ficha.";
        }
        else
        {
            this->_jugadores[1].pickFicha(this->_drawPool[nrandom]);
            ficha += this->_drawPool[nrandom].getFicha();
            this->_drawPool.erase(this->_drawPool.begin() + nrandom);
            return ficha;
        }
    }
    return "Fallo";
}

std::string Domino::getFichas(int idJug)
{
    std::vector<Ficha> fichasV;
    std::string fichas = "FICHAS ";

    if (idJug == this->_jugadores[0].getID())
    {
        fichasV = this->_jugadores[0].getFichas();
        for (size_t i = 0; i < fichasV.size(); i++)
            fichas += fichasV[i].getFicha();
        return fichas;
    }
    if (idJug == this->_jugadores[1].getID())
    {
        fichasV = this->_jugadores[1].getFichas();
        for (size_t i = 0; i < fichasV.size(); i++)
            fichas += fichasV[i].getFicha();
        return fichas;
    }
    return "Error";
}

bool Domino::siguePartida()
{
    if (this->_jugadores[0].getFichas().size() == 0 or this->_jugadores[1].getFichas().size() == 0)
        return false;
    else
        return true;
}

void insertarFicha(std::vector<Ficha> &fichas, Ficha &ficha)
{
    if (fichas.size() == 0)
        fichas.push_back(ficha);
    else
    {
        if (ficha.esDoble())
        {
            int i = 0;
            while (fichas[i].esDoble() and i < fichas.size())
            {
                if (ficha.getFirst() > fichas[i].getFirst())
                    fichas.insert(fichas.begin() + i, ficha);
                i++;
            }
            fichas.insert(fichas.begin() + i - 1, ficha);
        }
        else
        {
            for (size_t i = 0; i < fichas.size(); i++)
                if (!fichas[i].esDoble() && ficha > fichas[i])
                    fichas.insert(fichas.begin() + i, ficha);
            fichas.push_back(ficha);
        }
    }
}

int Domino::decidirTurno()
{
    for (size_t i = 0; i < this->_todas.size(); i++)
    {
        if (this->_jugadores[0].comprobarFicha(this->_todas[i]))
            return this->_jugadores[0].getID();
        else
        {
            if (this->_jugadores[1].comprobarFicha(this->_todas[i]))
                return this->_jugadores[1].getID();
        }
    }
    return -1;
}