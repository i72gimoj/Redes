#ifndef FICHA_HPP
#define FICHA_HPP

#include <iostream>

class Ficha
{
private:
    int firstNumber;
    int secondNumber;

public:
    inline Ficha(int firstNumber, int secondNumber)
    {
        this->firstNumber = firstNumber;
        this->secondNumber = secondNumber;
    }

    inline int getFirst() const
    {
        return this->firstNumber;
    }
    
    inline int getSecond() const
    {
        return this->secondNumber;
    }

    inline void setFirst(int firstNumber)
    {
        this->firstNumber = firstNumber;
    }

    inline void setSecond(int secondNumber)
    {
        this->secondNumber = secondNumber;
    }

    inline void invertirFicha()
    {
        int aux;
        aux = this->getFirst();
        this->setFirst(this->getSecond());
        this->setSecond(aux);
    }
};

inline std::ostream &operator<<(std::ostream &stream, Ficha &ficha)
{
    stream << "[" << ficha.getFirst() << " - " << ficha.getSecond() << "]";
    return stream;
}

#endif