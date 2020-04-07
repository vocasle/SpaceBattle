#include "SpaceShip.h"

SpaceShip::SpaceShip() : m_is_discovered{ false }, m_position{} { }

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