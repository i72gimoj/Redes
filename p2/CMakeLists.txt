cmake_minimum_required(VERSION 2.4)

project(Domino)

add_library(allfuncs src/domino/domino.cpp src/domino/tablero/tablero.cpp)

include_directories(src/domino src/domino/ficha src/domino/tablero)

link_libraries(allfuncs)

add_executable(main src/main.cpp)