/**
 * @file tablero.cpp
 * @author Iván Rodríguez Aguayo (i72roagi@uco.es)
 * @brief Fichero con el código de las funciones no inline de la clase Tablero
 * @version 1.0
 * @date 18-10-2019
 * @copyright Copyright (c) 2019
 */

#include "tablero.hpp"

Tablero::Tablero()
{
    this->_left = -1;
    this->_right = -1;
}

std::string Tablero::getTablero()
{
    std::string tablero;
    if (this->_tablero.size() == 0)
        tablero = "TABLERO VACIO";

    else
    {
        tablero = "TABLERO ";
        for (size_t i = 0; i < this->_tablero.size(); i++)
            tablero += this->_tablero[i].getFicha();
    }
    return tablero;
}

bool Tablero::ponerFicha(Ficha &ficha, std::string lado)
{
    if (this->_tablero.size() == 0)
    {
        this->_tablero.push_back(ficha);
        this->_left = ficha.getFirst();
        this->_right = ficha.getSecond();
        return true;
    }
    else
    {
        if (lado == "izquierda")
        {
            if (ficha.getSecond() == this->left())
            {
                this->_tablero.insert(this->_tablero.begin(), ficha);
                this->_left = ficha.getFirst();
                return true;
            }
            else
                return false;
        }
        else
        {
            if (lado == "derecha")
            {
                if (ficha.getFirst() == this->right())
                {
                    this->_tablero.push_back(ficha);
                    this->_right = ficha.getSecond();
                    return true;
                }
                else
                    return false;
            }
        }
    }
    return false;
}