#ifndef SPACE_MAP_H
#define SPACE_MAP_H

#include <vector>
#include "Point.h"

enum class Projection
{
	front, top
};

class SpaceMap
{
public:
	SpaceMap();
	const std::vector<std::vector<char>>& get_front_projection() const;
	const std::vector<std::vector<char>>& get_top_projection() const;
	void update_projections(const std::vector<Point>& points);
	void update_top_projection(const std::vector<std::vector<char>>& proj);
	static uint32_t size();
private:
	static const uint32_t grid_size = 10;
	std::vector<std::vector<char>> m_front_proj;
	std::vector<std::vector<char>> m_top_proj;
};

void plot_point(const Point& point, 
	std::vector<std::vector<char>>& front_proj,
	std::vector<std::vector<char>>& top_proj);

#endif // !SPACE_MAP_H
