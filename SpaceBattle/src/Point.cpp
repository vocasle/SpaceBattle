#include <SpaceBattle/Point.h>
#include <stdexcept>
#include <SpaceBattle/SpaceBattle.h>

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

std::istream& operator>>(std::istream& is, Point& point)
{
	is >> point.x >> point.y >> point.z;
	if (!is)
	{
		is.clear();
		is.unget();
		std::string line;
		std::getline(is, line);
		error( "'" + line + "' " + get_localized_str("not_valid_target_point"));
	}
	return is;
}

void error(const std::string& msg)
{
	throw std::runtime_error(msg);
}