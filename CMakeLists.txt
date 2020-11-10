cmake_minimum_required(VERSION 2.6)

project("Game_Text" C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99 -g")

add_library(game game.o game_aux.c)

add_executable(game_text game_text.c)

target_link_libraries(game_text game)

#-----------------------------------------------------#
include(CTest)
enable_testing()

project("game_test" C)

#* ********** gduboureau ********** *#
add_executable(game_test_gduboureau game_test_gduboureau.c)

target_link_libraries(game_test_gduboureau game)

add_test(test_gduboureau_game_restart ./game_test_gduboureau game_restart)

#* ********** valoustau ********** *#
add_executable(game_test_valoustau game_test_valoustau.c)
add_test(test_valoustau_dummy ./game_test_valoustau dummy)

#* ********** jephrem ********** *#
add_executable(game_test_jephrem game_test_jephrem.c)

target_link_libraries(game_test_jephrem game)

add_test(test_jephrem_game_delete ./game_test_jephrem game_delete)
add_test(test_jephrem_game_copy ./game_test_jephrem game_copy)
add_test(test_jephrem_game_equal ./game_test_jephrem game_equal)
add_test(test_jephrem_game_new_empty ./game_test_jephrem game_new_empty)

#* ********** halmansa ********** *#
add_executable(game_test_halmansa game_test_halmansa.c)
add_test(test_halmansa_dummy ./game_test_halmansa dummy)