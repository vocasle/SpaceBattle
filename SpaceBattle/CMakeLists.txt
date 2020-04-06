# CMakeList.txt : CMake project for SpaceBattle, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.8)

# Add source to this project's executable.
add_executable (SpaceBattle "game.cpp" "Point.cpp" "Point.h" "SpaceBattle.cpp" "SpaceBattle.h" "SpaceMap.cpp" "SpaceMap.h" "SpaceShip.cpp" "SpaceShip.h")
target_compile_features (SpaceBattle PUBLIC cxx_std_17)

# TODO: Add tests and install targets if needed.
