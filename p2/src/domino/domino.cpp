#include "domino.hpp"

Domino::Domino(int idjug1, int idjug2)
{
    this->_jugadores.push_back(Jugador(idjug1));
    this->_jugadores.push_back(Jugador(idjug2));
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
        this->_drawPool.push_back(ficha);
        if (y == 6)
        {
            y = x;
            x++;
        }
        y++;
    }
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

std::string Domino::ponerFicha(Ficha &ficha, char lado, int idJug)
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
