#include "SpaceShip.h"

SpaceShip::SpaceShip() : m_position{} { }

void SpaceShip::place_in_position(const std::vector<Point>& position)
{
	m_position = position;
}

const std::vector<Point>& SpaceShip::get_position() const
{
	return m_position;
}

void SpaceShip::uncover_position()
{
	for (auto& p : m_position)
	{
		p.is_hitted = true;
	}
}

bool SpaceShip::is_discovered() const
{
	bool is_discovered = true;
	for (const auto& p : m_position)
	{
		is_discovered = is_discovered && p.is_hitted;
	}
	return is_discovered;
}