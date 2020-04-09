// SpaceBattle.cpp : Defines the entry point for the application.
//

#include <SpaceBattle/SpaceBattle.h>
#include <SpaceBattle/MiniGame.h>

#include <boost/locale.hpp>

#include <random>
#include <iostream>
#include <algorithm>

SpaceBattle::SpaceBattle():
	m_sector{Sector::f},
	m_charges{0},
	m_map{SpaceMap{10}},
	m_detected{false},
	m_ships{0}
{ 
	init_localization();
	print_welcome_msg();
	init_sector();
}

void SpaceBattle::init_sector()
{
	switch (m_sector)
	{
	case Sector::f:
		init_sector_f();
		break;
	case Sector::k:
		init_sector_k();
		break;
	case Sector::r:
		break;
	}
}

void SpaceBattle::init_sector_f()
{
	print_intro(m_sector);
	m_detected = true;
	m_charges = 20;
	// generate 2 3-decked ships 
	auto battleships = generate_ships(3, 2, m_map.size());
	// and one 5-decked ship
	auto command_ship = generate_ships(5, 1, m_map.size());
	std::vector<SpaceShip> ships;
	ships.reserve(battleships.size() + command_ship.size());
	ships.insert(ships.end(), battleships.begin(), battleships.end());
	ships.insert(ships.end(), command_ship.begin(), command_ship.end());
	m_ships = ships;
	print_hint();
	m_detected = false;
}

void SpaceBattle::init_sector_k()
{
	m_charges = 30;
	const auto map_size = 15;
	m_map = SpaceMap{ map_size };
	m_detected = true;
	print_intro(m_sector);
	// generate 4 3-decked ships 
	auto battleships = generate_ships(3, 4, map_size);
	// and 2 5-decked ships
	auto command_ship = generate_ships(5, 2, map_size);
	// generate 6 scout ships
	auto scout_ships = generate_ships(1, 6, map_size);
	std::vector<SpaceShip> ships;
	ships.reserve(battleships.size() + command_ship.size() + scout_ships.size());
	ships.insert(ships.end(), battleships.begin(), battleships.end());
	ships.insert(ships.end(), command_ship.begin(), command_ship.end());
	ships.insert(ships.end(), scout_ships.begin(), scout_ships.end());
	m_ships = ships;
	print_hint();
	m_detected = true;
}

void print_z_axis(uint32_t num, uint32_t map_size)
{
	if (num != map_size)
	{
		std::cout << ' ';
	}
	std::cout << num << ' ';
}

void print_y_axis(const std::string & spacer, uint32_t limit)
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

void print_x_axis(const std::string & spacer, uint32_t num, uint32_t map_size)
{
	if (num == map_size || num == map_size / 2)
	{
		std::cout << ' ' << ' ';
	}
	else
	{
		std::cout << spacer;
	}
	std::cout << num << ' ';
}

// prints front and top projections of 3D battle space to stdout
void SpaceBattle::print_round_result() const
{
	if (m_detected)
	{
		auto front_proj = m_map.get_front_projection();
		auto top_proj = m_map.get_top_projection();
		const auto size = front_proj.size();
		bool is_y_axis_printed = false;
		static const std::string spacer{ ' ', ' ', ' ' };
		// print meta information
		std::cout << spacer << ' ' << get_localized_str("charges_left") << ' ' << m_charges << "\n\n";
		// print projections' labels
		std::cout << spacer << ' ' << get_localized_str("front_projection_lbl") <<
			"          " << get_localized_str("top_projection_lbl") << "\n\n";

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

			print_z_axis(size - i, m_map.size());
			// print rows of front projection
			for (const auto& el : front_row)
			{
				std::cout << el << ' ';
			}
			// print Z axis label
			if (i + 1 == size / 2)
			{
				std::cout << "Z";
			}
			print_x_axis(spacer, i + 1, m_map.size());
			// print rows of top projection
			for (const auto& el : top_row)
			{
				std::cout << el << ' ';
			}
			// print X axis label
			if (i + 1 == size / 2)
			{
				std::cout << "X";
			}

			std::cout << '\n';
		}
		// print Y axis labels for both projections
		std::cout << "           Y                        Y\n";
	}
	else
	{
		std::cout << get_localized_str("no_obstacles_found")
			<< '\n' << get_localized_str("charges_left") << ": " << m_charges
			<< '\n';
	}
}

void SpaceBattle::next_sector()
{
	static const uint32_t sectors_length = 3;
	uint32_t next_sector_id = int(m_sector) + 1;
	if (next_sector_id == sectors_length)
	{
		// game done
		print_game_result();
		return;
	}
	m_sector = Sector(next_sector_id);
	init_sector();
}

// returns true only if all battleships were found by user
bool SpaceBattle::are_ships_discovered() const
{
	bool are_ships_discovered = true;
	for (const auto& ship : m_ships)
	{
		are_ships_discovered = are_ships_discovered && ship.is_discovered();
	}
	return are_ships_discovered;
}
// main game loop
// executes n times, where n is amount of charges
void SpaceBattle::run_game_loop()
{
	while (true)
	{
		--m_charges;
		// promt player for coordinates of the target point
		Point p = prompt_for_coordinates();
		// scan target point and check if any ships were discovered
		scan_area(p);
		if (are_ships_discovered())
		{
			next_sector();
			continue;
		}
		print_round_result();

		if (m_charges == 0)
		{
			if (wants_to_rush_through())
			{
				MiniGame game{ generate_obstacles(m_sector, m_map.size()) };
				if (game.won())
				{
					std::cout << get_localized_str("rush_through_sector_success");
					// go to the next sector
					next_sector();
					continue;
				}
				else
				{
					std::cout << get_localized_str("rush_through_sector_failure") << '\n';
				}
			}
			else
			{
				std::cout << get_localized_str("sector_failed") << '\n';
			}
			print_game_result();
			return;
		}
	}
}
// checks if any ship were hit by the scanning beam
void SpaceBattle::scan_area(const Point& p)
{
	for (auto& ship : m_ships)
	{
		auto position = ship.get_position();
		auto it = std::find(position.begin(), position.end(), p);
		if (it != position.end())
		{
			it->is_hitted = true;
			m_map.update_projections(position);
			ship.place_in_position(position);
			m_detected = true;
			return;
		}
	}
	m_detected = false;
}
// promts user for answer yes/no
bool SpaceBattle::wants_to_rush_through()
{
	std::cout << get_localized_str("player_wants_to_rush_promt") << ' ';
	std::string answer;
	std::cin >> answer;
	return answer == get_localized_str("affirmative_answer");
}
// generates battleships for Mini guess game
std::vector<SpaceShip> generate_obstacles(Sector s, uint32_t map_size)
{
	uint32_t num_of_scounts = s == Sector::f ? 6 : (s == Sector::k ? 12 : 24);
	uint32_t num_of_battleships = s == Sector::f ? 4 : (s == Sector::k ? 8 : 12);
	uint32_t num_of_command_ships = s == Sector::f ? 2 : (s == Sector::k ? 4 : 6);
	auto scouts = generate_ships(1, num_of_scounts, map_size);
	auto battleships = generate_ships(3, num_of_battleships, map_size);
	auto command_ships = generate_ships(5, num_of_command_ships, map_size);
	std::vector<SpaceShip> ships;
	ships.reserve(scouts.size() + battleships.size() + command_ships.size());
	ships.insert(ships.end(), scouts.begin(), scouts.end());
	ships.insert(ships.end(), battleships.begin(), battleships.end());
	ships.insert(ships.end(), command_ships.begin(), command_ships.end());
	return ships;
}

// prints end game results to stdout and exits the game
void SpaceBattle::print_game_result()
{
	std::cout << get_localized_str("game_over") << '\n';
	for (auto& ship : m_ships)
	{
		ship.uncover_position();
		m_map.update_projections(ship.get_position());
	}
	m_detected = true;
	print_round_result();
}
// outputs success message about lvl completion to stdout
void SpaceBattle::print_lvl_completed() const
{
	std::cout << "\n\n\n"
		<< "************************************************************\n"
		<< get_localized_str("sector_f_completed")
		<< "\n************************************************************\n\n\n";
}
// prints only one coordinate of the battleships if ship size is greater than 1
void SpaceBattle::print_hint()
{
	std::vector<Point> points;
	for (auto& ship : m_ships)
	{
		auto pos = ship.get_position();
		if (pos.size() > 1)
		{
			auto it = pos.begin();
			it->is_hitted = true;
			ship.place_in_position(pos);
			points.push_back(*it);
		}
	}
	m_map.update_projections(points);
	print_round_result();
}
// outputs message about lvl completion failure to stdout
void SpaceBattle::print_lvl_failed() const
{
	std::cout << "\n\n\n"
		<< "************************************************************\n"
		<< get_localized_str("sector_f_failed")
		<< "\n************************************************************\n\n\n";
}

// --------------------------------------------------------------------------------------
// helper functions
// --------------------------------------------------------------------------------------

// generates random uint32_t in [from, to] range
uint32_t random_int(uint32_t from, uint32_t to)
{
	std::random_device rd{};
	std::default_random_engine e(rd());
	std::uniform_int_distribution<uint32_t> ud(from, to);
	return ud(e);
}

// generates Point with random coordinates x,y,z from [from, to] range
Point generate_point(uint32_t from, uint32_t to)
{
	auto x = random_int(from, to);
	auto y = random_int(from, to);
	auto z = random_int(from, to);
	return Point{ x,y,z };
}

// generates random cortesian axis
Axis generate_axis()
{
	// there are only 3 axis in 3D space, hence random_int called with 0 and 2
	return Axis(random_int(0, 2));
}

// returns all coordinates that belong to axis
// for example if axis is Axis::x, then
// all point.x of points vector will be returned
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
// returns next or previous value right after or before the values from forbidden_vals vector
// returned value is guaranteed to be in range [min_val, max_val]
uint32_t next_val(
	const std::vector<uint32_t>& forbidden_vals, 
	uint32_t prev_val, 
	uint32_t min_val = 1, 
	uint32_t max_val = 10)
{
	auto [min_el, max_el] = std::minmax_element(forbidden_vals.begin(), forbidden_vals.end());
	if (*max_el + 1 <= max_val)
	{
		return *max_el + 1;
	}
	else if (*min_el - 1 >= min_val)
	{
		return *min_el - 1;
	}
	else
	{
		error("Error in next_val() - Next value will be out of range ["
			+ std::to_string(min_val)
			+ ", "
			+ std::to_string(max_val)
			+ "].");
	}
}
// aligns point p to the reference point according to axis a
// in other words, sets coordinate of p equals to ref
// that match the axis a
void align_point(const Point& ref, Point& p, Axis a)
{
	switch (a)
	{
	case Axis::x:
		p.x = ref.x;
		break;
	case Axis::y:
		p.y = ref.y;
		break;
	case Axis::z:
		p.z = ref.z;
		break;
	}
}

// generates position of the spaceship
std::vector<Point> generate_position(uint32_t ship_size, Axis axis, uint32_t map_size)
{
	auto axis2 = generate_axis();
	while (axis2 == axis)
	{
		axis2 = generate_axis();
	}

	std::vector<Point> position;
	position.push_back(generate_point(1, map_size));
	int32_t cnt = 1;
	while (position.size() != ship_size)
	{
		Point base_point = position.at(position.size() - 1);
		Point p{ 0,0,0 };
		align_point(base_point, p, axis);
		align_point(base_point, p, axis2);

		if (p.x == 0)
		{
			p.x = next_val(points_on_axis(Axis::x, position), base_point.x, 1, map_size);
		}
		else if (p.y == 0)
		{
			p.y = next_val(points_on_axis(Axis::y, position), base_point.y, 1, map_size);
		}
		else
		{
			p.z = next_val(points_on_axis(Axis::z, position), base_point.z, 1, map_size);
		}
		position.push_back(p);
		++cnt;
	}
	return position;
}
// generates enemy battleships
std::vector<SpaceShip> generate_ships(uint32_t ship_size, 
	uint32_t num_of_ships, uint32_t map_size)
{
	std::vector<SpaceShip> ships(num_of_ships);
	for (auto& ship : ships)
	{
		ship.place_in_position(generate_position(ship_size, generate_axis(), map_size));
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
// prompts user for target point coordinates
// throws std::runtime_exception if user input non integral values for X,Y or Z
Point prompt_for_coordinates()
{
	std::cout << get_localized_str("target_point_prompt_msg") << ": ";
	Point p{};
	std::cin >> p;
	return p;
}
// prints story text to stdout for specific lvl of the game
void print_intro(Sector lvl)
{
	// TODO move all text to file
	std::cout << get_localized_str("sector_f_intro")
		<< "\n---------------------------------------------------------------------------------------\n\n";
}

#include <fstream>
#include <sstream>

void init_localization()
{
	// is needed to support UTF-8 output to std::cout
	system("chcp 65001");
	boost::locale::generator gen;
	gen.add_messages_path("..\\assets\\i18n");
	gen.add_messages_domain("game");
	std::locale::global(gen(""));
	std::cout.imbue(std::locale());
}

std::string get_localized_str(const std::string& msg_id)
{
	return boost::locale::translate(msg_id);
}