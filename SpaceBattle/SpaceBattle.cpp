// SpaceBattle.cpp : Defines the entry point for the application.
//

#include "SpaceBattle.h"

#include <random>
#include <iostream>
#include <algorithm>

SpaceBattle::SpaceBattle(uint32_t missiles, uint32_t ship_size):
	m_missiles{missiles},
	m_ship_size{ship_size},
	m_map{SpaceMap{10}},
	m_ships{generate_ships(ship_size, missiles / 2)}
{ 
	//print_welcome_msg();
	print_game_result();
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
	if (num == 10)
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
		print_x_axis(spacer, i + 1);
		// print rows of top projection
		for (const auto& el : top_row)
		{
			std::cout << el << ' ';
		}
		std::cout << '\n';
	}
}

void SpaceBattle::run_game_loop()
{
	while (m_missiles != 0)
	{
		// promt user for input
		Point p = prompt_for_coordinates();
		// make a shot and check if any ships were hit
		if (make_shoot(p))
		{
			print_round_result();
		}
		// decrement m_missiles
		--m_missiles;
	}
	print_game_result();
}

bool SpaceBattle::make_shoot(const Point& p)
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

std::vector<Point> generate_space_position(uint32_t ship_size, Axis axis)
{
	auto axis2 = generate_axis();
	while (axis2 == axis)
	{
		axis2 = generate_axis();
	}
	auto base_point = generate_point();
	std::vector<Point> position;
	position.push_back(base_point);
	int32_t cnt = 1;
	while (position.size() != ship_size)
	{
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
			p.x = base_point.x + cnt > 10 ? base_point.x - cnt : base_point.x + cnt;
		}
		else if (p.y == 0)
		{
			p.y = base_point.y + cnt > 10 ? base_point.y - cnt : base_point.y + cnt;
		}
		else
		{
			p.z = base_point.z + cnt > 10 ? base_point.z - cnt : base_point.z + cnt;
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


Point prompt_for_coordinates()
{
	std::cout << "Enter coordinate of the targe: ";
	Point p{};
	std::cin >> p;
	return p;
}
