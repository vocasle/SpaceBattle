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

enum class Sector
{
	f, k, r
};

class SpaceBattle
{
public:
	SpaceBattle();
	void run_game_loop();
private:
	void scan_area(const Point& p);
	void print_round_result() const;
	bool are_ships_discovered() const;
	void print_game_result();
	void init_sector();
	void init_sector_f();
	void init_sector_k();
	void init_sector_r();
	void print_hint();
	void print_intro();
	bool wants_to_rush_through();
	bool is_mini_game_won();
	void next_sector();
	Sector m_sector;
	uint32_t m_charges;
	std::vector<SpaceShip> m_ships;
	SpaceMap m_map;
	bool m_detected;
};

std::vector<SpaceShip> generate_ships(uint32_t ship_size, uint32_t num_of_ships);
std::vector<Point> generate_points(uint32_t num_of_points, uint32_t ship_size);
void print_welcome_msg();
Point prompt_for_coordinates();
Axis generate_axis();
uint32_t random_int(uint32_t from, uint32_t to);
std::vector<Point> generate_position(uint32_t ship_size, Axis axis);
void init_localization();
std::string get_localized_str(const std::string& msg_id);
std::vector<SpaceShip> generate_obstacles(Sector s);

void write_console(const std::string& str);
std::string read_console(unsigned long chars_to_read);