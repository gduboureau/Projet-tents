cmake_minimum_required(VERSION 2.6)

project(Game_Text C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_library(libgame game.o game_aux.o)

add_executable(game_text game_text.o libgame.a)

target_link_libraries(game_text libgame)