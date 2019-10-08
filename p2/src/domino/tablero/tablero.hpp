#ifndef TABLERO_HPP
#define TABLERO_HPP
#include "ficha.hpp"

#include <vector>

class Tablero
{
private:
    std::vector<Ficha> tablero;
    int extremoIzquierdo;
    int extremoDerecho;

public:
    Tablero();
    bool ponerFicha(Ficha ficha);
    void printTablero();
    inline int getExtremoDerecho() const
    {
        return this->extremoDerecho;
    }

    inline bool isEmpty() const
    {
        if (this->tablero.size() == 0)
            return true;
        else
            return false;
    }

    inline int getExtremoIzquierdo() const
    {
        return this->extremoIzquierdo;
    }
};

#endif