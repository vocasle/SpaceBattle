#ifndef MINI_GAME_H
#define MINI_GAME_H

#include <SpaceBattle/SpaceShip.h>
#include <SpaceBattle/Point.h>

#include <vector>


class MiniGame
{
public:
	MiniGame(const std::vector<SpaceShip>& ships);
	bool won();
private:
	Point prompt();
	std::vector<Point> m_points;
};

std::vector<Point> get_points(const std::vector<SpaceShip>& ships);

#endif // !MINI_GAME_H
