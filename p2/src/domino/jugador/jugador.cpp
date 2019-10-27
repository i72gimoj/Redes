/**
 * @file jugador.cpp
 * @author Iván Rodríguez Aguayo (i72roagi@uco.es)
 * @brief Fichero del código de las funciones no inline de la clase Jugador
 * @version 1.0
 * @date 18-10-2019
 * @copyright Copyright (c) 2019
 */

#include "jugador.hpp"

bool JugadorD::removeFicha(Ficha &ficha)
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

bool JugadorD::comprobarFicha(Ficha &ficha)
{
    for (size_t i = 0; i < this->_fichas.size(); i++)
        if (this->_fichas[i] == ficha)
            return true;
    return false;
}

bool JugadorD::comprobarFicha(int left, int right)
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