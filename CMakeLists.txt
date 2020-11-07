cmake_minimum_required(VERSION 2.6)

project("Game_Text" C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99 -g")

add_library(game game.o game_aux.c)

add_executable(game_text game_text.c)

target_link_libraries(game_text game)

#-----------------------------------------------------#
enable_testing()

project("game_test_dummy" C)

#* ********** gduboureau ********** *#
add_executable(game_test_gduboureau game_test_gduboureau.c)
add_test(test_gduboureau_game_fill_grass_row ./game_test_gduboureau test_game_fill_grass_row 7)

#* ********** valoustau ********** *#
add_executable(game_test_valoustau game_test_valoustau.c)
add_test(test_valoustau_dummy ./game_test_valoustau dummy)

#* ********** jephrem ********** *#
add_executable(game_test_jephrem game_test_jephrem.c)
add_test(test_jephrem_dummy ./game_test_jephrem dummy)

#* ********** halmansa ********** *#
add_executable(game_test_halmansa game_test_halmansa.c)
add_test(test_halmansa_dummy ./game_test_halmansa dummy)