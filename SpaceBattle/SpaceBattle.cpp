// SpaceBattle.cpp : Defines the entry point for the application.
//

#include "SpaceBattle.h"

#include <random>
#include <iostream>
#include <algorithm>

SpaceBattle::SpaceBattle(uint32_t missiles, uint32_t ship_size):
	m_missiles{missiles},
	m_ship_size{ship_size},
	m_map{SpaceMap{10}}
{ 
	print_welcome_msg();
}

// --------------------------------------------------------------------------------------
// helper functions
// --------------------------------------------------------------------------------------

std::vector<Point> generate_points(uint32_t num_of_points)
{
	std::random_device rd{};
	std::default_random_engine e(rd());
	std::uniform_int_distribution<uint32_t> ud(1, 10);
	std::vector<Point> points(num_of_points);
	for (size_t i = 0; i < num_of_points; ++i)
	{
		auto x = ud(e);
		auto y = ud(e);
		auto z = ud(e);
		Point p{ x,y,z };
		while (std::find(points.begin(), points.end(), p) != points.end())
		{
			x = ud(e);
			y = ud(e);
			z = ud(e);
			p = Point{ x,y,z };
		}
		points.at(i) = p;
	}
	return points;
}

std::vector<SpaceShip> generate_ships(uint32_t ship_size, uint32_t num_of_ships)
{
	std::vector<SpaceShip> ships(num_of_ships);
	auto points = generate_points(num_of_ships * ship_size);
	for (size_t i = 0; i < num_of_ships; ++i)
	{
		std::vector<Point> position;
		for (size_t j = 0; j < ship_size; ++j)
		{
			position.push_back(points.at(i + j));
		}
		ships.at(i).place_in_position(position);
	}
	return ships;
}

void print_welcome_msg()
{
	std::cout
		<< "  ########   ########     #######    #######   ########\n"
		<< "  #          #       #   #      #   #          #\n"
		<< "  ########   ########    ########   #          ########\n"
		<< "         #   #           #      #   #          #\n"
		<< "  ########   #           #      #    #######   ########\n\n\n"
		<< "  ########     #######   ########   ########   #          ########\n"
		<< "  #       #   #      #      #          #       #          #\n"
		<< "  ########    ########      #          #       #          ########\n"
		<< "  #       #   #      #      #          #       #          #\n"
		<< "  ########    #      #      #          #       ########   ########\n";
}
