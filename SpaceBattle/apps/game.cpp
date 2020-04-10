#include <SpaceBattle/SpaceBattle.h>

int main()
{
	try
	{
		SpaceBattle sb{};
		sb.run_game_loop();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	// prevent cmd to close before user can read the output
	std::cin.get();
}