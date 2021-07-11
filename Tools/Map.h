#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

class MapFile : public std::ifstream {
public:
	MapFile(const char* path, std::ios_base::openmode mode = 1)
		:std::ifstream(path, mode)
	{
		is_binary = (mode == std::ios::binary);
	}

	bool is_binary = false;
};

class Map
{
public:
	int size_x = 0, size_y = 0;
	int tile_w = 1, tile_h = 1;
	std::string tiles;
};

MapFile& operator >> (MapFile& o, Map& m);
std::ostream& operator << (std::ostream& o, const Map& m);
std::ofstream& operator << (std::ofstream& o, const Map& m);

