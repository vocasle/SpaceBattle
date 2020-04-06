#ifndef POINT_H
#define POINT_H

#include <cstdint>

struct Point
{
	uint32_t x;
	uint32_t y;
	uint32_t z;
	bool is_hitted;
	Point(uint32_t x, uint32_t y, uint32_t z);
	Point();
};

bool operator==(const Point& lhs, const Point& rhs);
bool operator!=(const Point& lhs, const Point& rhs);
#endif // !POINT_H
