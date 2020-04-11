cmake_minimum_required (VERSION 3.15)

project ("SpaceBattle" VERSION 1.0
						DESCRIPTION "CLI adaptation of Battleship game"
						LANGUAGES CXX)

add_subdirectory ("apps")
add_subdirectory ("src")
add_subdirectory("thirdparties/nowide")
