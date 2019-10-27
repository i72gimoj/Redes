/**
 * @file domino.hpp
 * @author Iván Rodríguez Aguayo (i72roagi@uco.es)
 * @brief Fichero fuente de la clase Domino
 * @version 1.0
 * @date 18-10-2019
 * @copyright Copyright (c) 2019
 */

#ifndef DOMINO_HPP
#define DOMINO_HPP

#include "ficha.hpp"
#include "jugador.hpp"
#include "tablero.hpp"

#include <vector>

#include <stdio.h>
#include <time.h>

/**
 * @brief Clase Domino que gestiona el juego en sí
 */
class Domino
{
  private:
    std::vector<Ficha> _drawPool;
    std::vector<Ficha> _todas;
    std::vector<JugadorD> _jugadores;
    Tablero _tablero;
    void generarFichas();
    int decidirTurno();
    void repartirFichas();

  public:

    /**
     * @brief Constructor vacío
     */
    Domino() {}

    /**
     * @brief Constructor de la clase Domino
     * @param idjug1 ID del jugador 1
     * @param idjug2 ID del jugador 2
     * @param turno ID del jugador que empieza la partida
     *
     * Gestiona la partida entre dos jugadores
     */
    Domino(int idjug1, int idjug2, int &turno);

    /**
     * @brief Getter de las fichas de un jugador
     * @param idJug ID del jugador
     * @return std::string String con las fichas del jugador
     */
    std::string getFichas(int idJug);

    /**
     * @brief Getter de las fichas del tablero
     * @return std::string String con las fichas del tablero
     */
    std::string getTablero();

    /**
     * @brief Comprueba si la partida puede continuar
     * @return true Si puede continuar la partida
     * @return false Si por algún problema la partida no puede seguir
     */
    bool siguePartida();

    /**
     * @brief Pone la ficha en el tablero y la borra de la mano de un jugador
     * @param ficha Ficha a colocar
     * @param lado Lado donde colocar la ficha
     * @param idJug ID del jugador que pone la ficha
     * @return std::string Estado del tablero tras poner la ficha
     */
    std::string ponerFicha(Ficha &ficha, std::string lado, int idJug);

    /**
     * @brief Roba una ficha y se la da al jugador
     * @param idJug ID del jugador
     * @return std::string Ficha robada
     */
    std::string robarFicha(int idJug);
};

#endif

// TODO: Quien empieza partida