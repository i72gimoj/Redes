#ifndef JUGADOR_H
#define JUGADOR_H

#include "ficha.hpp"
#include <vector>

class Jugador
{
  private:
    int _id;
    std::vector<Ficha> _fichas;

  public:
    inline Jugador(int id) : _id(id){}

    inline int getID()
    {
        return this->_id;
    }

    inline std::vector<Ficha> getFichas()
    {
        return this->_fichas;
    }

    inline void pickFicha(const Ficha& ficha)
    {
      this->_fichas.push_back(ficha);
    }

    bool comprobarFicha(int left, int right);

    bool removeFicha(Ficha& ficha);
};

#endif