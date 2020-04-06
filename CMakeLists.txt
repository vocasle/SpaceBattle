# CMakeList.txt : Top-level CMake project file, do global configuration
# and include sub-projects here.
#
cmake_minimum_required (VERSION 3.8)

project ("SpaceBattle" VERSION 1.0
						DESCRIPTION "CLI adaptation of Battleship game"
						LANGUAGES CXX)

# Include sub-projects.
add_subdirectory ("SpaceBattle")
