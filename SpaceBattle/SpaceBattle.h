// SpaceBattle.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "SpaceMap.h"

class SpaceBattle
{
public:
	SpaceBattle(uint32_t missiles, uint32_t ship_size);
private:
	uint32_t m_missiles;
	uint32_t m_ship_size;
	SpaceMap m_map;
};

std::vector<SpaceShip> generate_ships(uint32_t ship_size, uint32_t num_of_ships);
std::vector<Point> generate_points(uint32_t num_of_points);
void print_welcome_msg();
