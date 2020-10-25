cmake_minimum_required(VERSION 2.6)

#* ********** valoustau ********** *#
project("game_test_dummy" C)

enable_testing()

add_test(test_valoustau_dummy ./game_test_valoustau dummy)

#* ********** gduboureau ********** *#

add_test(test_gduboureau_dummy ./game_test_gduboureau dummy)

#* ********** ... ********** *#