set(HEADER_LIST 
	"${SpaceBattle_SOURCE_DIR}\\include\\spacebattle\\Point.h"
	"${SpaceBattle_SOURCE_DIR}\\include\\spacebattle\\SpaceBattle.h"
	"${SpaceBattle_SOURCE_DIR}\\include\\spacebattle\\SpaceMap.h"
	"${SpaceBattle_SOURCE_DIR}\\include\\spacebattle\\SpaceShip.h"
	"${SpaceBattle_SOURCE_DIR}\\include\\spacebattle\\MiniGame.h")
add_library (SpaceBattleLib STATIC
	"Point.cpp"
	"SpaceBattle.cpp"  
	"SpaceMap.cpp" 
	"SpaceShip.cpp"
	"MiniGame.cpp")
target_compile_features (SpaceBattleLib PRIVATE cxx_std_17)
target_include_directories(SpaceBattleLib PUBLIC ..\\include)

find_package(Boost 1.72.0 EXACT REQUIRED 
	COMPONENTS locale
	HINTS "E:\\Libs\\boost_1_72_0\\stage\\lib\\cmake")
target_link_libraries(SpaceBattleLib PRIVATE Boost::locale)
