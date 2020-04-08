add_executable (SpaceBattle game.cpp)
target_compile_features (SpaceBattle PUBLIC cxx_std_17)
target_link_libraries (SpaceBattle PRIVATE SpaceBattleLib)