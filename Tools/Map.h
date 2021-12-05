#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <tuple>

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
	Map(int mapW, int mapH, int tileW, int tileH, int tilesLength);
public:
	int size_x = 0, size_y = 0;
	int tile_w = 1, tile_h = 1;
	std::string tiles = "";
};

MapFile& operator >> (MapFile& o, Map& m);
std::ostream& operator << (std::ostream& o, const Map& m);
std::ofstream& operator << (std::ofstream& o, const Map& m);

std::ofstream& operator << (std::ofstream& o, const std::map<std::string, std::vector<std::pair<int, int>>>& m);
MapFile& operator >> (MapFile& o, std::map<std::string, std::vector<std::pair<int, int>>>& m);