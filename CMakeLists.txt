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

target_link_libraries(game_test_valoustau game)

add_test(test_valoustau_game_get_current_nb_tents_row ./game_test_valoustau game_get_current_nb_tents_row)
add_test(test_valoustau_game_get_current_nb_tents_col ./game_test_valoustau game_get_current_nb_tents_col)
add_test(test_valoustau_game_get_current_nb_tents_all ./game_test_valoustau game_get_current_nb_tents_all)
#add_test(test_valoustau_game_play_move ./game_test_valoustau game_play_move)
#add_test(test_valoustau_game_check_move ./game_test_valoustau game_check_move)

#* ********** jephrem ********** *#
add_executable(game_test_jephrem game_test_jephrem.c)

target_link_libraries(game_test_jephrem game)

add_test(test_jephrem_game_delete ./game_test_jephrem game_delete)
add_test(test_jephrem_game_copy ./game_test_jephrem game_copy)
add_test(test_jephrem_game_equal ./game_test_jephrem game_equal)
add_test(test_jephrem_game_new_empty ./game_test_jephrem game_new_empty)
add_test(test_jephrem_game_new ./game_test_jephrem game_new)
add_test(test_jephrem_game_set_square ./game_test_jephrem game_set_square)

#* ********** halmansa ********** *#
add_executable(game_test_halmansa game_test_halmansa.c)

target_link_libraries(game_test_halmansa game)

add_test(test_halmansa_game_delete ./game_test_halmansa game_get_square)
add_test(test_halmansa_game_get_expected_nb_tents_row ./game_test_halmansa game_get_expected_nb_tents_row)
add_test(test_halmansa_game_get_expected_nb_tents_col ./game_test_halmansa game_get_expected_nb_tents_col)
add_test(test_halmansa_game_set_expected_nb_tents_row ./game_test_halmansa game_set_expected_nb_tents_row)
add_test(test_halmansa_game_set_expected_nb_tents_col ./game_test_halmansa game_set_expected_nb_tents_col)
add_test(test_halmansa_game_get_expected_nb_tents_all ./game_test_halmansa game_get_expected_nb_tents_all)