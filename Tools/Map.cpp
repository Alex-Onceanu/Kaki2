#include "Tools/pch.h"
#include "Map.h"

Map::Map(int mapW, int mapH, int tileW, int tileH, int tilesLength)
	: size_x(mapW)
	, size_y(mapH)
	, tile_w(tileW)
	, tile_h(tileH)
{
	tiles.append(tilesLength, '.');
}

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

std::ofstream& operator << (std::ofstream& file, const std::map<std::string, std::vector<std::pair<int, int>>>& resultat)
{
	try
	{
		for (auto it = resultat.begin(); it != resultat.end(); it++)
		{
			int len = 1 + static_cast<int>(it->first.length());
			file.write((char*)&len, sizeof(len));
			file.write(it->first.c_str(), len);

			int s = static_cast<int>(it->second.size() * sizeof(std::pair<int,int>));
			file.write((char*)&s, sizeof(s));
			file.write(reinterpret_cast<const char*>(&(it->second[0])), s);

			file.flush();
		}
	}
	catch (std::exception& e)
	{
		std::ofstream log("log.txt", std::ios_base::app);
		log << e.what() << std::endl;
	}
	return file;
}

MapFile& operator >> (MapFile& file, std::map<std::string, std::vector<std::pair<int, int>>>& resultat)
{
	if (file.is_binary)
	{
		try
		{
			while (not file.eof())
			{
				int len;
				file.read((char*)&len, sizeof(len));
				
				auto string_lu = std::make_unique<char[]>(static_cast<unsigned __int64>(len) + 1);
				file.read(string_lu.get(), len);
				string_lu.get()[len] = '\0';

				auto first = std::string(string_lu.get());


				int size_vector;
				file.read((char*)&size_vector, sizeof(size_vector));

				std::vector<std::pair<int, int>> second(size_vector / sizeof(std::pair<int, int>));
				file.read(reinterpret_cast<char*>(&second[0]), size_vector);
				
				resultat.insert(std::make_pair(first, second));
			}
		}
		catch (...) { return file; } 
		//Ce try-catch se declenche si le fichier se termine par un string (entite definie mais sans positions)
	}
	return file;
}