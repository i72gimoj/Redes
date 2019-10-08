#include "tablero.hpp"

Tablero::Tablero()
{
    this->extremoIzquierdo = -1;
    this->extremoDerecho = -1;
}

bool Tablero::ponerFicha(Ficha ficha)
{
    if (this->tablero.size() == 0)
    {
        this->tablero.push_back(ficha);
        this->extremoIzquierdo = ficha.getFirst();
        this->extremoDerecho = ficha.getSecond();
        return true;
    }
    else
    {
        if (ficha.getSecond() == this->extremoIzquierdo)
        {
            this->tablero.insert(this->tablero.begin(), ficha);
            this->extremoIzquierdo = ficha.getFirst();
            return true;
        }
        else
        {
            if (ficha.getFirst() == this->extremoDerecho)
            {
                this->tablero.push_back(ficha);
                this->extremoDerecho = ficha.getSecond();
                return true;
            }
            else
            {
                ficha.invertirFicha();
                if (ficha.getSecond() == this->extremoIzquierdo)
                {
                    this->tablero.insert(this->tablero.begin(), ficha);
                    this->extremoIzquierdo = ficha.getFirst();
                    return true;
                }
                else
                {
                    if (ficha.getFirst() == this->extremoDerecho)
                    {
                        this->tablero.push_back(ficha);
                        this->extremoDerecho = ficha.getSecond();
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

void Tablero::printTablero()
{
    if (this->tablero.size() == 0)
    {
        std::cout << "Tablero vacío" << std::endl;
    }

    else
    {
        for (size_t i = 0; i < this->tablero.size(); i++)
        {
            std::cout << this->tablero[i];
        }
        std::cout << std::endl;
    }
}