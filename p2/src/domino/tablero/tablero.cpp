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

bool Tablero::ponerFicha(Ficha &ficha, char lado)
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
        if (lado == 'I')
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
            if (lado == 'D')
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
}