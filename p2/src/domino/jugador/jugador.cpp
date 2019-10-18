#include "jugador.hpp"

bool Jugador::removeFicha(Ficha &ficha)
{
    for (size_t i = 0; i < this->_fichas.size(); i++)
    {
        if (ficha == this->_fichas[i])
        {
            this->_fichas.erase(this->_fichas.begin() + i);
            return true;
        }
    }
    return false;
}

bool Jugador::comprobarFicha(int left, int right)
{
    for (size_t i = 0; i < this->_fichas.size(); i++)
    {
        if (this->_fichas[i].getFirst() == left or this->_fichas[i].getSecond() == left)
            return true;
        if (this->_fichas[i].getFirst() == right or this->_fichas[i].getSecond() == right)
            return true;
    }
    return false;
}