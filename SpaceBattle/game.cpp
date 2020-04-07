#include "SpaceBattle.h"

int main()
{
	try
	{
		SpaceBattle sb{ 10,5 };
		sb.run_game_loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}