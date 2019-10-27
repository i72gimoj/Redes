/**
 * @file Jugador.hpp
 * @author Iván Rodríguez Aguayo (i72roagi@uco.es)
 * @brief fichero fuente de la clase JugadorD
 * @version 1.0
 * @date 18-10-2019
 * @copyright Copyright (c) 2019
 */

#ifndef JUGADOR_H
#define JUGADOR_H

#include "ficha.hpp"
#include <vector>

/**
 * @brief Clase JugadorD, mantiene las fichas y el id de cada jugador
 */
class JugadorD
{
  private:
    int _id;
    std::vector<Ficha> _fichas;

  public:
    /**
     * @brief Constructor de la clase JugadorD
     * @param id Id del jugador
     *
     * Inicializa el valor de la id del jugador
     */
    inline JugadorD(int id)
    {
        this->_id = id;
    }

    /**
     * @brief Getter del id del jugador
     * @return int ID del jugador
     */
    inline int getID()
    {
        return this->_id;
    }

    /**
     * @brief Getter de la mano del jugador
     * @return std::vector<Ficha> Vector que almacena la mano de fichas del jugador
     */
    inline std::vector<Ficha> getFichas()
    {
        return this->_fichas;
    }

    /**
     * @brief Añade una ficha a la mano del jugador
     * @param ficha Ficha a añadir
     */
    inline void pickFicha(const Ficha &ficha)
    {
        this->_fichas.push_back(ficha);
    }

    /**
     * @brief Comprueba si el jugador tiene ficha para poner en el tablero
     * @param left Valor izquierdo del tablero
     * @param right Valor derecho del tablero
     * @return true Si tiene para jugar
     * @return false Si no tiene ficha para jugar
     */
    bool comprobarFicha(int left, int right);

    /**
     * @brief Comprueba si el jugador tiene una ficha en mano
     * @param ficha Ficha a comprobar
     * @return true Si tiene la ficha
     * @return false Si no tiene la ficha
     */
    bool comprobarFicha(Ficha &ficha);

    /**
     * @brief Borra una ficha de la mano del jugador
     * @param ficha Ficha a borrar
     * @return true Si se ha borrado correctamente
     * @return false Si ha habido algún error
     */
    bool removeFicha(Ficha &ficha);
};

#endif