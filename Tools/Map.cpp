#include <fstream>
#include <vector>

#include "pch.h"
#include "Map.h"

std::ostream& operator << (std::ostream& o, const Map& m)
{
	o << "Size x : " << m.size_x << "; "
		<< "Size y :" << m.size_y << "; "
		<< "Tile x : " << m.tile_w << "; "
		<< "Tile y : " << m.tile_h << ";\n"
		<< "Tiles : " << m.tiles << std::endl;
	return o;
}

std::ofstream& operator << (std::ofstream& o, const Map& m)
{
	try
	{
#if 0
		o.write((char*)&m.size_x, sizeof(m.size_x));
		o.write((char*)&m.size_y, sizeof(m.size_y));
		o.write((char*)&m.tile_x, sizeof(m.tile_x));
		o.write((char*)&m.tile_y, sizeof(m.tile_y));
#else
		auto s = sizeof(m) - sizeof (m.tiles);
		o.write((char*)&m, s);
#endif
		auto l = m.tiles.length();
		o.write((char*)&l, sizeof(l));
		o.write(m.tiles.c_str(), l);

		o.flush();
	}
	catch (const char* e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
		log << e << std::endl;
	}
	return o;
}

MapFile& operator >> (MapFile& o, Map& m)
{
	if (o.is_binary)
	{
#if 0
		o.read((char*)&m.size_x, sizeof(m.size_x));
		o.read((char*)&m.size_y, sizeof(m.size_y));
		o.read((char*)&m.tile_x, sizeof(m.tile_x));
		o.read((char*)&m.tile_y, sizeof(m.tile_y));
#else
		auto s = sizeof(m) - sizeof(m.tiles);
		o.read((char*)&m, s);

#endif
		int l;
		o.read((char*)&l, sizeof(l));

		l += 4;
		//!!!!!!!!!!! *** CHANGER ICI, 4 sort de nul part (mais ça marche parce que les char* commencent '\0' avec la conversion)

		auto tiles = std::make_unique<char[]>(l);
		o.read(tiles.get(), l);
		m.tiles = std::string(tiles.get(), l);
	}
	else
	{
		try
		{
			std::ofstream log("log.txt");
			o >> m.size_x >> m.size_y;

 			unsigned x{ 0 }, y{ 0 };

			while (not o.eof())
			{
				std::string line;
				std::getline(o, line);

				if (not line.empty())
				{
					y++;

					std::stringstream ss{ line };
					x = 0;
					while (not ss.eof())
					{
						char c{ 0 };
						ss >> c;
						
						if (c == 0) continue;
						
						if (c < '1' or c > '9')
						{
							if (c < 'a' or c > 'z')
							{
								log << "Bloc inconnu : " << c << std::endl;
								continue;
							}
						}
						m.tiles += c;
						x++;
					}
				}
			}
			m.tile_h = m.size_y / y;
			m.tile_w = m.size_x / x;
		}
		catch (const char* e)
		{
			std::ofstream log("log.txt", std::ios_base::app);
			log << e << std::endl;
		}
	}
	return o;
}



