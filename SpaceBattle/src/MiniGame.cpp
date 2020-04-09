#include <SpaceBattle/MiniGame.h>

#include <algorithm>

MiniGame::MiniGame(const std::vector<SpaceShip>& ships): 
	m_points{get_points(ships)}
{ }

bool MiniGame::play()
{
	// promt user for input
	Point p = prompt();
	// check if point is unique
	return std::find(m_points.begin(), m_points.end(), p) == m_points.end();
}

Point MiniGame::prompt()
{
	std::wcout << "Enter coordinates for the jump (X Y Z): ";
	Point p{};
	std::cin >> p;
	return p;
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
