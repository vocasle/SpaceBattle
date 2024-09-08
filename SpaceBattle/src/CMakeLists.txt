add_library (SpaceBattleLib STATIC
	"Point.cpp"
	"MiniGame.cpp"
	"SpaceBattle.cpp"  
	"SpaceMap.cpp" 
	"SpaceShip.cpp")

include(FetchContent)
FetchContent_Declare(
    boost 
    URL https://github.com/boostorg/boost/releases/download/boost-1.86.0/boost-1.86.0-cmake.tar.xz
    OVERRIDE_FIND_PACKAGE)
FetchContent_MakeAvailable(boost)

target_compile_features (SpaceBattleLib PUBLIC cxx_std_17)
target_include_directories(SpaceBattleLib PUBLIC ../include)
target_link_libraries(SpaceBattleLib PRIVATE Boost::locale Boost::nowide)
file(COPY ../assets DESTINATION ${CMAKE_BINARY_DIR})
