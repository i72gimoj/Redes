/**
 * @file tablero.hpp
 * @author Iván Rodríguez Aguayo (i72roagi@uco.es)
 * @brief Fichero fuente de la clase Tablero
 * @version 1.0
 * @date 18-10-2019
 * @copyright Copyright (c) 2019
 */

#ifndef TABLERO_HPP
#define TABLERO_HPP
#include "ficha.hpp"

#include <vector>

/**
 * @brief Clase Tablero, gestiona el tablero de la partida
 */
class Tablero
{
  private:
    std::vector<Ficha> _tablero;
    int _left;
    int _right;

  public:
    /**
     * @brief Constructor de la clase Tablero
     */
    Tablero();

    /**
     * @brief Getter de las fichas puestas e el tablero
     * @return std::string String con las fichas del tablero
     */
    std::string getTablero();

    /**
     * @brief Coloca una ficha en el tablero
     * @param ficha Ficha a colocar
     * @param lado Lado donde se coloca la ficha
     * @return true Si se puede colocar
     * @return false Si hubo algún error
     */
    bool ponerFicha(Ficha &ficha, std::string lado);

    /**
     * @brief Devuelve el extremo izquierdo del tablero
     * @return const int Valor constante del extremo izquierdo del tablero
     */
    inline const int left()
    {
        return this->_left;
    }

    /**
     * @brief Devuelve el extremo derecho del tablero
     * @return const int Valor constante del extremo derecho del tablero
     */
    inline const int right()
    {
        return this->_right;
    }
};

#endif