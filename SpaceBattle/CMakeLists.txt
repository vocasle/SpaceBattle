cmake_minimum_required (VERSION 3.15)

project ("SpaceBattle" VERSION 1.0
						DESCRIPTION "CLI adaptation of Battleship game"
						LANGUAGES CXX)

add_subdirectory ("apps")
add_subdirectory ("src")
add_subdirectory("thirdparties/nowide")

install(TARGETS SpaceBattle
	RUNTIME
	DESTINATION bin)

install(TARGETS SpaceBattleLib
	ARCHIVE
	DESTINATION lib)

install(DIRECTORY include
	DESTINATION .)

install(FILES assets/i18n/en/LC_MESSAGES/game.mo
	DESTINATION assets/i18n/ru/LC_MESSAGES)

install(FILES assets/i18n/en/LC_MESSAGES/game.mo
	DESTINATION assets/i18n/en/LC_MESSAGES)
