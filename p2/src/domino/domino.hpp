#ifndef DOMINO_HPP
#define DOMINO_HPP

#include "ficha.hpp"
#include "jugador.hpp"
#include "tablero.hpp"

#include <vector>

#include <stdio.h>
#include <time.h>

class Domino
{
  private:
    std::vector<Ficha> _drawPool;
    std::vector<Jugador> _jugadores;
    Tablero _tablero;
    void generarFichas();
    void repartirFichas();

  public:
    Domino(int idjug1, int idjug2);

    std::string getFichas(int idJug);

    std::string getTablero();

    bool siguePartida();

    std::string ponerFicha(Ficha &ficha, char lado, int idJug);

    std::string robarFicha(int idJug);
};

#endif