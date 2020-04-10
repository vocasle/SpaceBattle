// SpaceBattle.cpp : Defines the entry point for the application.
//

#include <SpaceBattle/SpaceBattle.h>
#include <SpaceBattle/MiniGame.h>

#include <boost/locale.hpp>

#include <random>
#include <iostream>
#include <algorithm>

SpaceBattle::SpaceBattle() :
	m_sector{ Sector::f },
	m_charges{ 0 },
	m_map{ SpaceMap{} },
	m_detected{ false },
	m_ships{ 0 }
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
		init_sector_r();
		break;
	}
}

void SpaceBattle::init_sector_f()
{
	print_intro();
	m_charges = 20;
	auto battleships = generate_ships(3, 2);
	auto command_ship = generate_ships(5, 1);
	std::vector<SpaceShip> ships;
	ships.reserve(battleships.size() + command_ship.size());
	ships.insert(ships.end(), battleships.begin(), battleships.end());
	ships.insert(ships.end(), command_ship.begin(), command_ship.end());
	m_ships = ships;
	print_hint();
}

void SpaceBattle::init_sector_k()
{
	m_charges = 50;
	m_map = SpaceMap{};
	m_detected = false;
	print_intro();
	auto battleships = generate_ships(3, 4);
	auto command_ship = generate_ships(5, 2);
	auto scout_ships = generate_ships(2, 6);
	std::vector<SpaceShip> ships;
	ships.reserve(battleships.size() + command_ship.size() + scout_ships.size());
	ships.insert(ships.end(), battleships.begin(), battleships.end());
	ships.insert(ships.end(), command_ship.begin(), command_ship.end());
	ships.insert(ships.end(), scout_ships.begin(), scout_ships.end());
	m_ships = ships;
	print_hint();
}

void SpaceBattle::init_sector_r()
{
	print_intro();
}

void print_z_axis(uint32_t num, uint32_t map_size)
{
	if (num != map_size)
	{
		std::cout << ' ';
	}
	std::cout << num << ' ';
}

void print_y_axis(uint32_t limit)
{
	for (auto i = 0; i < 2; ++i)
	{

		std::cout << ' ' << ' ' << ' ';
		for (size_t j = 0; j < limit; ++j)
		{
			std::cout << j + 1 << ' ';
		}
		std::cout << ' ';
	}
	std::cout << '\n';
}

void print_x_axis(uint32_t num, uint32_t map_size)
{
	if (num == map_size || num == map_size / 2)
	{
		std::cout << ' ' << ' ';
	}
	else
	{
		std::cout << ' ' << ' ' << ' ';
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
		// print meta information
		std::cout << get_localized_str("charges_left") << ": " << m_charges << "\n";
		// print projections' labels
		std::cout << get_localized_str("front_projection_lbl") << std::string(10, ' ') << get_localized_str("top_projection_lbl") << "\n\n";

		for (size_t i = 0; i < size; ++i)
		{
			const auto& front_row = front_proj.at(size - 1 - i);
			const auto& top_row = top_proj.at(i);
			// print y axis
			if (!is_y_axis_printed)
			{
				print_y_axis(front_row.size());
				is_y_axis_printed = true;
			}

			print_z_axis(size - i, SpaceMap::size());
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
			print_x_axis(i + 1, SpaceMap::size());
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

void play_decipher_game()
{
	const std::string cipher{ get_localized_str("cipher") };
	const std::string cipher_lbl{ get_localized_str("cipher_lbl") };
	const std::string dechiper_1{ get_localized_str("guess_without_spaces") };
	const std::string dechiper_2{ get_localized_str("guess_with_spaces") };
	const std::string incorrect_word{ get_localized_str("incorrect_dechiper") };
	static const uint32_t alphabet_length{ 26 };
	static const std::vector<char> alphabet{ 'a', 'b', 'c', 'd', 'e',
		'f', 'g', 'h', 'i', 'j', 
		'k', 'l', 'm', 'n', 'o', 
		'p', 'q', 'r', 's', 't', 
		'u','v', 'w', 'x', 'y', 'z'};
	uint32_t cnt{ 0 };
	std::string guess;
	while (true)
	{
		if (guess == dechiper_1 || guess == dechiper_2)
		{
			break;
		}
		else
		{
			if (cnt > 0)
			{
				if (cnt == 10)
				{
					guess = dechiper_1;
					std::cout << get_localized_str("colleague_dechipered_first") << ' ';
					break;
				}
				std::cout << incorrect_word << ":\n";
				std::cout << cipher << '\n';
				for (auto& ch : guess)
				{
					if (ch == ' ')
					{
						continue;
					}
					auto pos = std::find(alphabet.begin(), alphabet.end(), ch);
					if (pos != alphabet.end())
					{
						std::cout << (pos - alphabet.begin()) + 1 << ' ';
					}
					else
					{
						std::cout << '?' << ' ';
					}
				}
				std::cout << '\n';
			}
		}
		std::cout << cipher_lbl << ":\n";
		std::cout << cipher << '\n';
		std::cout << get_localized_str("prompt_cipher_txt") << ": ";
		std::getline(std::cin, guess);
		++cnt;
	}
	std::cout << get_localized_str("game_over_final_txt");
}

bool SpaceBattle::is_mini_game_won()
{
	MiniGame game{ generate_obstacles(m_sector) };
	if (game.won())
	{
		std::cout <<
			get_localized_str("rush_through_sector_success") << '\n';
		return true;
	}
	else
	{
		std::cout <<
			get_localized_str("rush_through_sector_failure") << '\n';
	}
	return false;
}
// main game loop
// executes n times, where n is amount of charges
void SpaceBattle::run_game_loop()
{
	while (true)
	{
		if (m_sector == Sector::r) // end game
		{
			// play sector R dechiper game
			play_decipher_game();
			break;
		}
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
				if (is_mini_game_won())
				{
					// init sector k
					next_sector();
					continue;
				}
			}
			else
			{
				std::cout << get_localized_str("refused_to_rush") << '\n';
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
	--m_charges;
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
std::vector<SpaceShip> generate_obstacles(Sector s)
{
	uint32_t num_of_scounts = s == Sector::f ? 6 : (s == Sector::k ? 12 : 24);
	uint32_t num_of_battleships = s == Sector::f ? 4 : (s == Sector::k ? 8 : 12);
	uint32_t num_of_command_ships = s == Sector::f ? 2 : (s == Sector::k ? 4 : 6);
	auto scouts = generate_ships(1, num_of_scounts);
	auto battleships = generate_ships(3, num_of_battleships);
	auto command_ships = generate_ships(5, num_of_command_ships);
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
	m_detected = true;
	print_round_result();
	m_detected = false;
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
std::vector<Point> generate_position(uint32_t ship_size, Axis axis)
{
	auto axis2 = generate_axis();
	while (axis2 == axis)
	{
		axis2 = generate_axis();
	}

	std::vector<Point> position;
	position.push_back(generate_point(1, SpaceMap::size()));
	int32_t cnt = 1;
	while (position.size() != ship_size)
	{
		Point base_point = position.at(position.size() - 1);
		Point p{ 0,0,0 };
		align_point(base_point, p, axis);
		align_point(base_point, p, axis2);

		if (p.x == 0)
		{
			p.x = next_val(points_on_axis(Axis::x, position), base_point.x, 1, SpaceMap::size());
		}
		else if (p.y == 0)
		{
			p.y = next_val(points_on_axis(Axis::y, position), base_point.y, 1, SpaceMap::size());
		}
		else
		{
			p.z = next_val(points_on_axis(Axis::z, position), base_point.z, 1, SpaceMap::size());
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
	Point p{0,0,0};
	while (p.x == 0 && p.y == 0 && p.z == 0)
	{
		std::cout << get_localized_str("target_point_prompt_msg") << ": ";
		try
		{
			std::cin >> p;
		}
		catch (const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			p.x = p.y = p.z = 0;
		}
	}
	return p;
}
// prints story text to stdout for specific lvl of the game
void SpaceBattle::print_intro()
{
	std::cout << '\n';
	switch (m_sector)
	{
	case Sector::f:
		std::cout << get_localized_str("sector_f_intro") << "\n\n";
		break;
	case Sector::k:
		std::cout << get_localized_str("sector_k_intro") << "\n\n";
		break;
	case Sector::r:
		// do not print first sentence if player did not found all battleships
		if (m_charges >= 0 && are_ships_discovered())
		{
			std::cout << get_localized_str("bonus_txt") << ' ';
		}
		std::cout << get_localized_str("sector_r_intro") << "\n\n";
		break;
	}
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