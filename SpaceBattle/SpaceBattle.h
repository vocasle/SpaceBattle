// SpaceBattle.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <iostream>
#include "SpaceMap.h"
#include "SpaceShip.h"

enum class Axis
{
	x,y,z
};

enum class Level
{
	one, two, three
};

class SpaceBattle
{
public:
	SpaceBattle();
	void print_round_result() const;
	void print_game_result();
	void run_game_loop();
	bool make_shoot(const Point& p);
private:
	void init_lvl();
	void init_lvl_one();
	Level m_lvl;
	uint32_t m_missiles;
	uint32_t m_ship_size;
	std::vector<SpaceShip> m_ships;
	SpaceMap m_map;
};

std::vector<SpaceShip> generate_ships(uint32_t ship_size, uint32_t num_of_ships);
std::vector<Point> generate_points(uint32_t num_of_points, uint32_t ship_size);
void print_welcome_msg();
Point prompt_for_coordinates();
Axis generate_axis();
uint32_t random_int(uint32_t from, uint32_t to);
std::vector<Point> generate_space_position(uint32_t ship_size, Axis axis);
void print_intro(Level lvl);
