#ifndef DOMINO_HPP
#define DOMINO_HPP

#include "ficha.hpp"
#include "tablero.hpp"

#include <vector>

#include <time.h>
#include <stdio.h>

class Domino
{
private:
    std::vector<Ficha> drawPool;
    std::vector<Ficha> playerOne;
    std::vector<Ficha> playerTwo;
    Tablero tablero;
    void generarFichas();

public:
    Domino();
    bool debesRobar(int player);
    void mostrarTablero();
    void ponerFicha(int player);
    void printPlayer(int player);
    void mostrarDrawPool();
    bool estanRepartidas();
    void repartirFichas();
    void robarFicha(int player);
};

#endif