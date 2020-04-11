find_package(Boost 1.72.0 EXACT REQUIRED 
	COMPONENTS locale
	HINTS "E:\\Libs\\boost_1_72_0\\stage\\lib\\cmake")

add_library (SpaceBattleLib STATIC
	"Point.cpp"
	"MiniGame.cpp"
	"SpaceBattle.cpp"  
	"SpaceMap.cpp" 
	"SpaceShip.cpp")

target_compile_features (SpaceBattleLib PUBLIC cxx_std_17)
target_include_directories(SpaceBattleLib PUBLIC ../include)
target_link_libraries(SpaceBattleLib PRIVATE Boost::locale Boost::nowide)
file(COPY ../assets DESTINATION ${CMAKE_BINARY_DIR})
