#ifndef TABLERO_HPP
#define TABLERO_HPP
#include "ficha.hpp"

#include <vector>

class Tablero
{
  private:
    std::vector<Ficha> _tablero;
    int _left;
    int _right;

  public:
    Tablero();

    std::string getTablero();

    bool ponerFicha(Ficha &ficha, char lado);

    inline const int left()
    {
        return this->_left;
    }

    inline const int right()
    {
        return this->_right;
    }
};

#endif