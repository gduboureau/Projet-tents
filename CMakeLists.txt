enable_testing()

project(Game_Text C)

set(CMAKE_VERBOSE_MAKEFILE ON)

set(CMAKE_C_FLAGS "-std=c99 -g -Wall")

add_library(game game.o game_aux.c)

add_executable(game_text game_text.c)

target_link_libraries(game_text game)

add_test(test_gduboureau_dummy ./game_test_gduboureau dummy)