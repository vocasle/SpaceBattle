set(HEADER_LIST 
	"${SpaceBattle_SOURCE_DIR}/include/spacebattle/Point.h"
	"${SpaceBattle_SOURCE_DIR}/include/spacebattle/SpaceBattle.h"
	"${SpaceBattle_SOURCE_DIR}/include/spacebattle/SpaceMap.h"
	"${SpaceBattle_SOURCE_DIR}/include/spacebattle/SpaceShip.h")
add_library (SpaceBattleLib STATIC
	"Point.cpp"
	"SpaceBattle.cpp"  
	"SpaceMap.cpp" 
	"SpaceShip.cpp" )
target_compile_features (SpaceBattleLib PRIVATE cxx_std_17)
target_include_directories(SpaceBattleLib PUBLIC ../include)
