#include <SpaceBattle/SpaceMap.h>

SpaceMap::SpaceMap(): 
	m_front_proj(grid_size, std::vector<char>(grid_size, '*')),
	m_top_proj(grid_size, std::vector<char>(grid_size, '*'))
{ }

const std::vector<std::vector<char>>& SpaceMap::get_front_projection() const
{
	return m_front_proj;
}

const std::vector<std::vector<char>>& SpaceMap::get_top_projection() const
{
	return m_top_proj;
}

void SpaceMap::update_projections(const std::vector<Point>& points)
{
	for (const auto& point : points)
	{
		if (point.is_hitted)
		{
			plot_point(point, m_front_proj, m_top_proj);
		}
	}
}

void SpaceMap::update_top_projection(const std::vector<std::vector<char>>& proj)
{
	m_top_proj = proj;
}

uint32_t SpaceMap::size()
{
	return grid_size;
}

void plot_point(const Point& point,
	std::vector<std::vector<char>>& front_proj,
	std::vector<std::vector<char>>& top_proj)
{
	front_proj[point.z - 1][point.y - 1] = '#';
	top_proj[point.x - 1][point.y - 1] = '#';
}
