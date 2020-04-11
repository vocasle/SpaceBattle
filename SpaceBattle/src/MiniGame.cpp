#include <SpaceBattle/MiniGame.h>
#include <SpaceBattle/SpaceBattle.h>

#include <boost/nowide/iostream.hpp>

#include <algorithm>

MiniGame::MiniGame(const std::vector<SpaceShip>& ships): 
	m_points{get_points(ships)}
{ }

bool MiniGame::won()
{
	// promt user for input
	Point p = prompt();
	// check if point is unique
	return std::find(m_points.begin(), m_points.end(), p) == m_points.end();
}

Point MiniGame::prompt()
{
	while (true)
	{
		write_console(get_localized_str("jump_coordinates") + ": ");
		auto input = read_console();
		if (!is_valid_coordinates(input))
		{
			write_console("'" + input + "' " + get_localized_str("not_valid_target_point") + "\n");
		}
		return str_to_point(input);
	}
}

std::vector<Point> get_points(const std::vector<SpaceShip>& ships)
{
	std::vector<Point> points;
	for (auto& ship : ships)
	{
		auto pos = ship.get_position();
		points.insert(points.end(), pos.begin(), pos.end());
	}
	return points;
}
