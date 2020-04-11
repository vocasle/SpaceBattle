#include <SpaceBattle/Point.h>
#include <SpaceBattle/SpaceBattle.h>
#include <stdexcept>
#include <sstream>
#include <regex>

Point::Point(uint32_t x, uint32_t y, uint32_t z) : 
	x{ x }, 
	y{ y }, 
	z{ z }, 
	is_hitted{ false } { }

Point::Point() : 
	x{ 0 }, 
	y{ 0 }, 
	z{ 0 }, 
	is_hitted{ false } { }

bool operator==(const Point& lhs, const Point& rhs)
{
	return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z;
}

bool operator!=(const Point& lhs, const Point& rhs)
{
	return !(lhs == rhs);
}

bool is_valid_coordinates(const std::string& str)
{
	std::regex rx{ R"""(\d{1,2} \d{1,2} \d{1,2})""" };
	auto s{ str };
	trim_str(s);
	return std::regex_match(s, rx);
}

Point str_to_point(const std::string& str)
{
	std::stringstream ss{ str };
	Point p{};
	ss >> p.x >> p.y >> p.z;
	return p;
}

void error(const std::string& msg)
{
	throw std::runtime_error(msg);
}