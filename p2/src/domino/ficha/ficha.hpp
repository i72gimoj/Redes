/**
 * @file ficha.hpp
 * @author Iván Rodríguez Aguayo (i72roagi@uco.es)
 * @brief Fichero fuente de la clase ficha utilizada para el juego del dominó
 * @version 1.0
 * @date 18-10-2019
 * @copyright Copyright (c) 2019
 */

#ifndef FICHA_HPP
#define FICHA_HPP

#include <iostream>
#include <string>

/**
 * @brief Clase ficha, utilizada en el juego del dominó
 *
 * Esta ficha tiene dos valores, el número de puntos a cada lado de la ficha, de 0 a 6, que define el valor de una ficha
 */
class Ficha
{
  private:
    int _firstNumber;
    int _secondNumber;

  public:

    /**
     * @brief Constructor vacío de la clase Ficha
     */
    inline Ficha() { }

    /**
     * @brief Constructor de la clase Ficha, inicializa los valores de la ficha de dominó
     * @param firstNumber Primer valor de la ficha
     * @param secondNumber Segundo valor de la ficha
     */
    inline Ficha(int firstNumber, int secondNumber)
    {
        this->_firstNumber = firstNumber;
        this->_secondNumber = secondNumber;
    }

    /**
     * @brief Getter del primer valor de la ficha
     * @return const int Devuelve un int constante con el primer valor de la ficha
     */
    inline const int getFirst() const
    {
        return this->_firstNumber;
    }

    /**
     * @brief Getter del segundo valor de la ficha
     * @return const int Devuelve un int constante con el segundo valor de la ficha
     */
    inline const int getSecond() const
    {
        return this->_secondNumber;
    }

    /**
     * @brief Setter del primer valor de la ficha
     * @param firstNumber Nuevo valor del primer elemento
     */
    inline void setFirst(int firstNumber)
    {
        this->_firstNumber = firstNumber;
    }

    /**
     * @brief Comprueba si una ficha es doble, los dos valores iguales
     * @return true Si la ficha es doble
     * @return false Si no lo es
     */
    inline bool esDoble()
    {
        if (this->_firstNumber == this->_secondNumber)
            return true;
        return false;
    }

    /**
     * @brief Setter del segundo valor de la ficha
     * @param firstNumber Nuevo valor del segundo elemento
     */
    inline void setSecond(int secondNumber)
    {
        this->_secondNumber = secondNumber;
    }

    /**
     * @brief Getter de la ficha, formateada como se pide en la práctica 2 de redes
     * @return const std::string String constante con el valor de la ficha
     */
    inline const std::string getFicha()
    {
        return "|" + std::to_string(this->_firstNumber) + "|" + std::to_string(this->_secondNumber) + "|";
    }

    /**
     * @brief Invierte la ficha, cambiando el primer valor por el segundo
     */
    inline void invert()
    {
        int aux;
        aux = this->getFirst();
        this->setFirst(this->getSecond());
        this->setSecond(aux);
    }
};

/**
 * @brief Sobrecarga del operador de igualdar para la clase Ficha
 * @param ficha1 Primera ficha a comparar
 * @param ficha2 Segunda ficha a comparar
 * @return true Si ambas fichas son iguales
 * @return false Si ambas fichas son distintas
 */
inline bool operator==(Ficha &ficha1, Ficha &ficha2)
{
    if ((ficha1.getFirst() == ficha2.getFirst()) and (ficha1.getSecond() == ficha2.getSecond()))
        return true;
    else
    {
        if ((ficha1.getSecond() == ficha2.getFirst()) and (ficha1.getFirst() == ficha2.getSecond()))
            return true;
        else
            return false;
    }
}

inline bool operator>(Ficha &ficha1, Ficha &ficha2)
{
    if ((ficha1.getFirst() + ficha1.getSecond()) > (ficha2.getFirst() + ficha2.getSecond()))
        return true;
    return false;
}

#endif