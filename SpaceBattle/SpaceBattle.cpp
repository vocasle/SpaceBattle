﻿// SpaceBattle.cpp : Defines the entry point for the application.
//

#include "SpaceBattle.h"

#include <random>
#include <iostream>
#include <algorithm>

SpaceBattle::SpaceBattle():
	m_lvl{Level::one},
	m_charges{0},
	m_ship_size{0},
	m_map{SpaceMap{10}},
	m_ships{0}
{ 
	print_welcome_msg();
	init_lvl();
}

void SpaceBattle::init_lvl()
{
	switch (m_lvl)
	{
	case Level::one:
		init_lvl_one();
		break;
	case Level::two:
		break;
	case Level::three:
		break;
	}
}

void SpaceBattle::init_lvl_one()
{
	print_intro(m_lvl);
	m_charges = 20;
	// generate 2 3-decked ships 
	auto battleships = generate_ships(3, 2);
	// and one 5-decked ship
	auto command_ship = generate_ships(5, 1);
	std::vector<SpaceShip> ships;
	ships.reserve(battleships.size() + command_ship.size());
	ships.insert(ships.end(), battleships.begin(), battleships.end());
	ships.insert(ships.end(), command_ship.begin(), command_ship.end());
	m_ships = ships;
	print_hint();
}

void print_z_axis(const uint32_t num)
{
	// print z axis
	if (num != 10)
	{
		std::cout << ' ';
	}
	std::cout << num << ' ';
}

void print_y_axis(const std::string& spacer, uint32_t limit)
{
	std::cout << spacer;
	for (size_t j = 0; j < limit; ++j)
	{
		std::cout << j + 1 << ' ';
	}
	std::cout << spacer << ' ';
	for (size_t j = 0; j < limit; ++j)
	{
		std::cout << j + 1 << ' ';
	}
	std::cout << '\n';
}

void print_x_axis(const std::string& spacer, uint32_t num)
{
	// print x axis
	if (num == 10 || num == 5)
	{
		std::cout << ' ' << ' ';
	}
	else
	{
		std::cout << spacer;
	}
	std::cout << num << ' ';
}

// prints front and top projections of 3D battle space to cout
void SpaceBattle::print_round_result() const
{
	auto front_proj = m_map.get_front_projection();
	auto top_proj = m_map.get_top_projection();
	auto size = front_proj.size();
	bool is_y_axis_printed = false;
	static const std::string spacer{ ' ', ' ', ' ' };
	// print map labels
	std::cout << spacer << " FRONT PROJECTION          TOP PROJECTION\n\n";

	for (size_t i = 0; i < size; ++i)
	{
		const auto& front_row = front_proj.at(size - 1 - i);
		const auto& top_row = top_proj.at(i);
		// print y axis
		if (!is_y_axis_printed)
		{
			print_y_axis(spacer, front_row.size());
			is_y_axis_printed = true;
		}

		print_z_axis(size - i);
		// print rows of front projection
		for (const auto& el : front_row)
		{
			std::cout << el << ' ';
		}
		// print Z axis label
		if (i == 4)
		{
			std::cout << "Z";
		}
		print_x_axis(spacer, i + 1);
		// print rows of top projection
		for (const auto& el : top_row)
		{
			std::cout << el << ' ';
		}
		// print X axis label
		if (i == 4)
		{
			std::cout << "X";
		}

		std::cout << '\n';
	}

	std::cout << "           Y                        Y\n";
}

void SpaceBattle::run_game_loop()
{
	while (m_charges != 0)
	{
		// promt user for input
		Point p = prompt_for_coordinates();
		// make a shot and check if any ships were hit
		if (scan_area(p))
		{
			print_round_result();
		}
		// decrement m_charges
		--m_charges;
	}
	print_game_result();
}

bool SpaceBattle::scan_area(const Point& p)
{
	for (auto& ship : m_ships)
	{
		auto position = ship.get_position();
		auto it = std::find(position.begin(), position.end(), p);
		if (it != position.end())
		{
			it->is_hitted = true;
			m_map.update_projections(position);
			return true;
		}
	}
	return false;
}

void SpaceBattle::print_game_result()
{
	for (auto& ship : m_ships)
	{
		ship.uncover_position();
		m_map.update_projections(ship.get_position());
	}
	print_round_result();
}

// --------------------------------------------------------------------------------------
// helper functions
// --------------------------------------------------------------------------------------

uint32_t random_int(uint32_t from, uint32_t to)
{
	std::random_device rd{};
	std::default_random_engine e(rd());
	std::uniform_int_distribution<uint32_t> ud(from, to);
	return ud(e);
}

Point generate_point()
{
	auto x = random_int(1, 10);
	auto y = random_int(1, 10);
	auto z = random_int(1, 10);
	return Point{ x,y,z };
}

Axis generate_axis()
{
	return Axis(random_int(0, 2));
}

// returns all point coordinates that match axis
// for example if axis is Axis::x, then
// all point.x of points will be returned
std::vector<uint32_t> points_on_axis(Axis a, const std::vector<Point>& points)
{
	std::vector<uint32_t> points_on_axis;
	for (const auto& point : points)
	{
		switch (a)
		{
		case Axis::x:
			points_on_axis.push_back(point.x);
			break;
		case Axis::y:
			points_on_axis.push_back(point.y);
			break;
		case Axis::z:
			points_on_axis.push_back(point.z);
			break;
		}
	}
	return points_on_axis;
}

uint32_t next_val(const std::vector<uint32_t>& forbidden_vals, uint32_t prev_val, uint32_t min_val = 1, uint32_t max_val = 10)
{
	auto [min_el, max_el] = std::minmax_element(forbidden_vals.begin(), forbidden_vals.end());
	if (*max_el + 1 <= max_val)
		return *max_el + 1;
	else if (*min_el - 1 >= min_val)
		return *min_el - 1;
	else
	{
		error("Error in next_val() - Next value will be out of range ["
			+ std::to_string(min_val)
			+ ", "
			+ std::to_string(max_val)
			+ "].");
	}
}

std::vector<Point> generate_space_position(uint32_t ship_size, Axis axis)
{
	auto axis2 = generate_axis();
	while (axis2 == axis)
	{
		axis2 = generate_axis();
	}

	std::vector<Point> position;
	position.push_back(generate_point());
	int32_t cnt = 1;
	const uint32_t map_size = 10;
	while (position.size() != ship_size)
	{
		Point base_point = position.at(position.size() - 1);
		Point p{ 0,0,0 };
		switch (axis)
		{
		case Axis::x:
			p.x = base_point.x;
			break;
		case Axis::y:
			p.y = base_point.y;
			break;
		case Axis::z:
			p.z = base_point.z;
			break;
		}
		switch (axis2)
		{
		case Axis::x:
			p.x = base_point.x;
			break;
		case Axis::y:
			p.y = base_point.y;
			break;
		case Axis::z:
			p.z = base_point.z;
			break;
		}

		if (p.x == 0)
		{
			p.x = next_val(points_on_axis(Axis::x, position), base_point.x, 1, 10);
		}
		else if (p.y == 0)
		{
			p.y = next_val(points_on_axis(Axis::y, position), base_point.y, 1, 10);
		}
		else
		{
			p.z = next_val(points_on_axis(Axis::z, position), base_point.z, 1, 10);
		}
		position.push_back(p);
		++cnt;
	}
	return position;
}

std::vector<Point> generate_points(uint32_t num_of_points, uint32_t ship_size)
{
	std::vector<Point> points(num_of_points);
	for (size_t i = 0; i < num_of_points; i += ship_size)
	{
		auto position = generate_space_position(ship_size, generate_axis());
		for (size_t j = 0; j < position.size(); ++j)
		{
			points.at(i + j) = position.at(j);
		}
	}
	return points;
}

std::vector<SpaceShip> generate_ships(uint32_t ship_size, uint32_t num_of_ships)
{
	std::vector<SpaceShip> ships(num_of_ships);
	auto points = generate_points(num_of_ships * ship_size, ship_size);
	uint32_t ships_processed = 0;
	std::vector<Point> position;
	for (size_t i = 0; i < points.size(); ++i)
	{
		position.push_back(points[i]);
		if (position.size() == ship_size)
		{
			ships.at(ships_processed).place_in_position(position);
			position = {};
			++ships_processed;
		}
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
		<< "  ########    #      #      #          #       ########   ########\n\n\n";
}


Point prompt_for_coordinates()
{
	std::cout << "Target point coordinates (X Y Z): ";
	Point p{};
	std::cin >> p;
	return p;
}

void print_intro(Level lvl)
{
	// TODO move all text to file
	std::cout << "\tYou are the Chief HPS Engineer on the research spaceship."
		<< "\nYour ship is on course to HPS refueling point."
		<< "\n\n\tSpacehip's main mission is detection of singularities"
		<< "\nthat prevent good communication with Space Flight Control Center."
		<< "\nThe tool that makes that detection possible is high precision scanner (HPS)."
		<< "\n\n\tIn 0.5 parsecs from HPS refueling point you were called on the"
		<< "\nbridge due to emergency. Spacehip's sensors detected some abnormal"
		<< "\nradiation in sector F."
		<< "\n\n\tThe Captain ordered to perform low energy scanning on the sector F."
		<< "\nScanners detected 3 battleships nearby. Two 3-masted battleships and one"
		<< "\n5-masted command ship. But due to the fact that scanning was performed in"
		<< "\nlow energy mode you were not able to detect precise location of battleships."
		<< "\n\n\tYou were ordered to detect precise location of the battleships to find"
		<< "\na safe route through sector F using HPS."
		<< "\n\n\tDue to the fact that you almost used all charges of the HPS in sector"
		<< "\nE you have only 20 charges left."
		<< "\n\n\tHPS emits a beam that allows to gen precise information"
		<< "\nof the area nearby the target point."
		<< "\nCoordinates of the target point must be entered precisely in following order: X Y Z."
		<< "\nThe coordinates must be separated by space and after entering the coordinates"
		<< "\nthey must be commited with commit button (Enter)."
		<< "\n\n\tHere is the picture that low energy scanning was able to get. All scanners on"
		<< "\nthe spaceship capture 3D image using right handed Cartesian coordinate system and"
		<< "\nreport it using two projections of the 3D image: top and front projections."
		<< "\n\n";
}



void SpaceBattle::print_hint()
{
	std::vector<Point> points;
	for (const auto& ship : m_ships)
	{
		auto pos = ship.get_position();
		if (pos.size() > 1)
		{
			auto it = pos.begin();
			it->is_hitted = true;
			points.push_back(*it);
		}
	}
	m_map.update_projections(points);
	print_round_result();
	hide_ships();
}

void SpaceBattle::hide_ships()
{
	std::vector<Point> points;
	for (const auto& ship : m_ships)
	{
		auto pos = ship.get_position();
		for (auto it = pos.begin(); it != pos.end(); ++it)
		{
			it->is_hitted = false;
			points.push_back(*it);
		}
	}
	m_map.update_projections(points);
}