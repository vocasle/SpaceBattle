#ifndef SPACE_SHIP_H
#define SPACE_SHIP_H

#include <vector>
#include "Point.h"

class SpaceShip
{
public:
	SpaceShip();
	void place_in_position(const std::vector<Point>& position);
	const std::vector<Point>& get_position() const;
	void uncover_position();
	bool is_discovered() const;
private:
	std::vector<Point> m_position;
};

#endif // !SPACE_SHIP_H
