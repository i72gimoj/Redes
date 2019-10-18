#ifndef FICHA_HPP
#define FICHA_HPP

#include <iostream>
#include <string>

class Ficha
{
  private:
    int _firstNumber;
    int _secondNumber;

  public:
    inline Ficha()
    {
    }

    inline Ficha(int firstNumber, int secondNumber)
    {
        this->_firstNumber = firstNumber;
        this->_secondNumber = secondNumber;
    }

    inline const int getFirst() const
    {
        return this->_firstNumber;
    }

    inline const int getSecond() const
    {
        return this->_secondNumber;
    }

    inline void setFirst(int firstNumber)
    {
        this->_firstNumber = firstNumber;
    }

    inline void setSecond(int secondNumber)
    {
        this->_secondNumber = secondNumber;
    }

    inline const std::string getFicha()
    {
        return "|" + std::to_string(this->_firstNumber) + "|" + std::to_string(this->_secondNumber) + "|";
    }

    inline void invert()
    {
        int aux;
        aux = this->getFirst();
        this->setFirst(this->getSecond());
        this->setSecond(aux);
    }
};

inline bool operator==(Ficha &ficha1, Ficha &ficha2)
{
    if (ficha1.getFirst() == ficha2.getFirst() and ficha1.getSecond() == ficha2.getSecond())
        return true;
    else
    {
        if (ficha1.getSecond() == ficha2.getFirst() and ficha1.getFirst() == ficha2.getSecond())
            return true;
        else
            return false;
    }
}

#endif