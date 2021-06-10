#include <fstream>

#include <SDL.h>

#include "Game.h"
#include "TextureManager.h"
int main(int argc, char** argv)
{

	Renderer::Init();
	
	try
	{
		auto game = std::make_unique<Game>();
		game->MainLoop();
		return 0;
	}
	catch (char* e)
	{
		std::ofstream log("log.txt");
		log << e << std::endl;
		return -1;
	}

	return 0;
}