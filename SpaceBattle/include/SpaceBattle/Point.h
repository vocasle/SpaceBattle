#ifndef POINT_H
#define POINT_H

#include <cstdint>
#include <iostream>
#include <string>

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
std::istream& operator>>(std::istream& is, Point& point);
void error(const std::string& msg);

#endif // !POINT_H
