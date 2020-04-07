#include "SpaceShip.h"

SpaceShip::SpaceShip(uint32_t health) : m_health{health}, m_is_sunk{false}, m_position{} { }
SpaceShip::SpaceShip() : m_health{ 0 }, m_is_sunk{ false }, m_position{} { }

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