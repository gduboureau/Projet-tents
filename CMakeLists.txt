cmake_minimum_required(VERSION 2.6)

project("Game_Text" C)

set(CMAKE_VERBOSE_MAKEFILE OFF)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -std=c99 -g --coverage")

## use a CMake3 package to solve SDL2 dependencies
find_package(SDL2 REQUIRED)
set(SDL2_ALL_INC ${SDL2_INCLUDE_DIRS})
set(SDL2_ALL_LIBS ${SDL2_LIBRARIES} -lSDL2_ttf -lSDL2_image) # add extra SDL2 libraries

## print this 
message(STATUS "SDL2 include dir: ${SDL2_ALL_INC}")
message(STATUS "SDL2 all libraries: ${SDL2_ALL_LIBS}")

add_library(game game.c game_aux.c queue.c game_tools.c)
include_directories(${SDL2_ALL_INC})

add_executable(game_text game_text.c)
add_executable(game_solve game_solve.c)
add_executable(game_sdl game_sdl.c model.c)
add_executable(minimal minimal.c)

target_link_libraries(game_sdl ${SDL2_ALL_LIBS} m)
target_link_libraries(game_text game)
target_link_libraries(game_solve game)
target_link_libraries(game_sdl game)

target_link_libraries(minimal ${SDL2_ALL_LIBS} m)

## copy useful data files in build dir
foreach(file "tree.png" "tent.png" "grass.png" "empty.png" "losing.png" "background.png" "Police.ttf" "fin.png" "quit.png" "aide.png")
  file(COPY ${file} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
endforeach(file)

#-----------------------------------------------------#
include(CTest)
enable_testing()

project("game_test" C)

#* ********** gduboureau ********** *#
add_executable(game_test_gduboureau game_test_gduboureau.c)

target_link_libraries(game_test_gduboureau game)

add_test(test_gduboureau_game_restart ./game_test_gduboureau game_restart)
add_test(test_gduboureau_game_fill_grass_row ./game_test_gduboureau game_fill_grass_row)
add_test(test_gduboureau_game_fill_grass_col ./game_test_gduboureau game_fill_grass_col)
add_test(test_gduboureau_game_print ./game_test_gduboureau game_print)
add_test(test_gduboureau_game_default ./game_test_gduboureau game_default)
add_test(test_gduboureau_game_default_solution ./game_test_gduboureau game_default_solution)
add_test(test_gduboureau_game_is_over ./game_test_gduboureau game_is_over)

#* ********** valoustau ********** *#
add_executable(game_test_valoustau game_test_valoustau.c)

target_link_libraries(game_test_valoustau game)

add_test(test_valoustau_game_get_current_nb_tents_row ./game_test_valoustau game_get_current_nb_tents_row)
add_test(test_valoustau_game_get_current_nb_tents_col ./game_test_valoustau game_get_current_nb_tents_col)
add_test(test_valoustau_game_get_current_nb_tents_all ./game_test_valoustau game_get_current_nb_tents_all)
add_test(test_valoustau_game_play_move ./game_test_valoustau game_play_move)
add_test(test_valoustau_game_check_move ./game_test_valoustau game_check_move)

#* ********** jephrem ********** *#
add_executable(game_test_jephrem game_test_jephrem.c)

target_link_libraries(game_test_jephrem game)

add_test(test_jephrem_game_delete ./game_test_jephrem game_delete)
add_test(test_jephrem_game_copy ./game_test_jephrem game_copy)
add_test(test_jephrem_game_equal ./game_test_jephrem game_equal)
add_test(test_jephrem_game_new_empty ./game_test_jephrem game_new_empty)
add_test(test_jephrem_game_new ./game_test_jephrem game_new)
add_test(test_jephrem_game_set_square ./game_test_jephrem game_set_square)
add_test(test_jephrem_game_undo ./game_test_jephrem game_undo)
add_test(test_jephrem_game_redo ./game_test_jephrem game_redo)

#* ********** halmansa ********** *#
add_executable(game_test_halmansa game_test_halmansa.c)

target_link_libraries(game_test_halmansa game)

add_test(test_halmansa_game_get_square ./game_test_halmansa game_get_square)
add_test(test_halmansa_game_get_expected_nb_tents_row ./game_test_halmansa game_get_expected_nb_tents_row)
add_test(test_halmansa_game_get_expected_nb_tents_col ./game_test_halmansa game_get_expected_nb_tents_col)
add_test(test_halmansa_game_set_expected_nb_tents_row ./game_test_halmansa game_set_expected_nb_tents_row)
add_test(test_halmansa_game_set_expected_nb_tents_col ./game_test_halmansa game_set_expected_nb_tents_col)
add_test(test_halmansa_game_get_expected_nb_tents_all ./game_test_halmansa game_get_expected_nb_tents_all)
add_test(test_halmansa_game_play_move ./game_test_halmansa game_play_move)
add_test(test_halmansa_game_new_ext ./game_test_halmansa game_new_ext)
add_test(test_halmansa_game_new_empty_ext ./game_test_halmansa game_new_empty_ext)
add_test(test_halmansa_game_load ./game_test_halmansa game_load)
add_test(test_halmansa_game_save ./game_test_halmansa game_save)