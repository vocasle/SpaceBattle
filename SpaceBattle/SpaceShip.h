#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <vector>
#include "Point.h"

class SpaceShip
{
public:
	SpaceShip(uint32_t health);
	SpaceShip();
	void place_in_position(const std::vector<Point>& position);
	const std::vector<Point>& get_position() const;
	void uncover_position();
private:
	std::vector<Point> m_position;
	bool m_is_sunk;
	uint32_t m_health;
};

#endif // !SPACE_SHIP_H
