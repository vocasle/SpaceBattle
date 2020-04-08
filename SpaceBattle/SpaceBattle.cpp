// SpaceBattle.cpp : Defines the entry point for the application.
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

uint32_t SpaceBattle::map_size()
{
	static const uint32_t size = 10;
	return size;
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
	if (num != SpaceBattle::map_size())
	{
		std::wcout << ' ';
	}
	std::wcout << num << ' ';
}

void print_y_axis(const std::wstring & spacer, uint32_t limit)
{
	std::wcout << spacer;
	for (size_t j = 0; j < limit; ++j)
	{
		std::wcout << j + 1 << ' ';
	}
	std::wcout << spacer << ' ';
	for (size_t j = 0; j < limit; ++j)
	{
		std::wcout << j + 1 << ' ';
	}
	std::wcout << '\n';
}

void print_x_axis(const std::wstring & spacer, uint32_t num)
{
	if (num == SpaceBattle::map_size() || num == SpaceBattle::map_size() / 2)
	{
		std::wcout << ' ' << ' ';
	}
	else
	{
		std::wcout << spacer;
	}
	std::wcout << num << ' ';
}

// prints front and top projections of 3D battle space to stdout
void SpaceBattle::print_round_result() const
{
	auto front_proj = m_map.get_front_projection();
	auto top_proj = m_map.get_top_projection();
	const auto size = front_proj.size();
	bool is_y_axis_printed = false;
	static const std::wstring spacer{ ' ', ' ', ' ' };
	// print meta information
	std::wcout << spacer << " CHARGES LEFT: " << m_charges << "\n\n";
	// print projections' labels
	std::wcout << spacer << " FRONT PROJECTION          TOP PROJECTION\n\n";

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
			std::wcout << el << ' ';
		}
		// print Z axis label
		if (i + 1 == size / 2)
		{
			std::wcout << "Z";
		}
		print_x_axis(spacer, i + 1);
		// print rows of top projection
		for (const auto& el : top_row)
		{
			std::wcout << el << ' ';
		}
		// print X axis label
		if (i + 1 == size / 2)
		{
			std::wcout << "X";
		}

		std::wcout << '\n';
	}
	// print Y axis labels for both projections
	std::wcout << "           Y                        Y\n";
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
	while (m_charges != 0)
	{
		--m_charges;
		// promt user for coordinates of the target point
		Point p = prompt_for_coordinates();
		// scan target point and check if any ships were discovered
		if (scan_area(p))
		{
			if (are_ships_discovered())
			{
				break;
			}
			print_round_result();
		}
	}
	print_game_result();
}
// checks if any ship were hit by the scanning beam
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
			ship.place_in_position(position);
			return true;
		}
	}
	return false;
}
// prints end game results to stdout
void SpaceBattle::print_game_result()
{
	if (!are_ships_discovered())
	{
		for (auto& ship : m_ships)
		{
			ship.uncover_position();
			m_map.update_projections(ship.get_position());
		}
		// TODO:
		// ask if player wants to try his luck and break through
			// launch miny game to guess three points that does not
			// contain enemy battleships
		// otherwise print position of battleships and ask if he wants
		print_round_result();
		print_lvl_failed();
		// to play again
			// play again
		// otherwise quite
	}
	else
	{
		print_lvl_completed();
	}
}
// outputs success message about lvl completion to stdout
void SpaceBattle::print_lvl_completed() const
{
	std::wcout << "\n\n\n"
		<< "************************************************************"
		<< "\nYOU HAVE DISCOVERED ALL BATTLESHIPS"
		<< "\nAND SUCCESSFULLY FOUND A SAFE ROUTE THROUGH SECTOR F"
		<< "\nTO THE HPS REFUELING POINT."
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
	std::wcout << "\n\n\n"
		<< "************************************************************"
		<< "\nYOU HAVE FAILED TO DISCOVER ALL BATTLESHIPS"
		<< "\nAND REFUSED TO BREAK THROUGHT THE AREA F."
		<< "\nYOUR SPACESHIP WAS CAPTURED AND BROUGHT TO ENEMY BASE."
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
std::vector<Point> generate_position(uint32_t ship_size, Axis axis)
{
	auto axis2 = generate_axis();
	while (axis2 == axis)
	{
		axis2 = generate_axis();
	}

	std::vector<Point> position;
	position.push_back(generate_point(1, SpaceBattle::map_size()));
	int32_t cnt = 1;
	while (position.size() != ship_size)
	{
		Point base_point = position.at(position.size() - 1);
		Point p{ 0,0,0 };
		align_point(base_point, p, axis);
		align_point(base_point, p, axis2);

		if (p.x == 0)
		{
			p.x = next_val(points_on_axis(Axis::x, position), base_point.x, 1, SpaceBattle::map_size());
		}
		else if (p.y == 0)
		{
			p.y = next_val(points_on_axis(Axis::y, position), base_point.y, 1, SpaceBattle::map_size());
		}
		else
		{
			p.z = next_val(points_on_axis(Axis::z, position), base_point.z, 1, SpaceBattle::map_size());
		}
		position.push_back(p);
		++cnt;
	}
	return position;
}
// generates enemy battleships
std::vector<SpaceShip> generate_ships(uint32_t ship_size, uint32_t num_of_ships)
{
	std::vector<SpaceShip> ships(num_of_ships);
	for (auto& ship : ships)
	{
		ship.place_in_position(generate_position(ship_size, generate_axis()));
	}
	return ships;
}

void print_welcome_msg()
{
	std::wcout
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
	std::wcout << "Target point coordinates (X Y Z): ";
	Point p{};
	std::cin >> p;
	return p;
}
// prints story text to stdout for specific lvl of the game
void print_intro(Level lvl)
{
	// TODO move all text to file
	std::wcout << "\tYou are the Chief HPS Engineer on the research spaceship."
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
		<< "\n---------------------------------------------------------------------------------------\n\n";
}