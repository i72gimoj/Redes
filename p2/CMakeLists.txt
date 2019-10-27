cmake_minimum_required(VERSION 2.4)

project(Domino)

add_library(allfuncs src/domino/domino.cpp src/domino/jugador/jugador.cpp src/domino/tablero/tablero.cpp)

include_directories(src/domino src/domino/jugador src/domino/ficha src/domino/tablero)

link_libraries(allfuncs)

add_executable(main src/main.cpp)

add_executable(cliente src/clienteDomino.cpp)

add_executable(servidor src/servidorDomino.cpp)