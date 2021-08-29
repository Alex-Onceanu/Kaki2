#include "IniReader.h"
#include "Entity.h"
#include "ControllerFactory.h"

#include <fstream>
#include <map>
#include <string>
#include <regex>
//line regex n'est pas bon
constexpr auto ini_line{ "([A-Za-z_]+[A-Za-z0-9_-]*)\\s*=\\s*\"?'?([A-Za-z0-9/]+\\.*[A-Za-z0-9]*)'?\"?" };
constexpr auto group_line{ "\\[([A-Za-z_]+)\\]" };


void IniReader::Read(ControllerFactory* cf, std::ifstream& iniFile, Entity& e)
{
	std::map<std::string, std::map<std::string, std::string>> ini;
	ParseIni(iniFile, ini);

	InitialData id;
	id.data = ini;
	e.LoadInitialData(cf, id);
}

void IniReader::ParseIni(std::ifstream& file, std::map<std::string, std::map<std::string, std::string>>& ini)
{
	std::string line;

	while (!file.eof())
	{
		std::getline(file, line);

		std::regex groupRegex(group_line);
		std::smatch groupMatch;

		std::regex lineRegex(ini_line);
		std::smatch lineMatch;

		bool enters = true;
		if (std::regex_match(line, groupMatch, groupRegex))
		{
			auto gm = groupMatch[1].str();
			std::getline(file, line);

			while (std::regex_match(line, lineMatch, lineRegex))
			{
				enters = false;
				ini[gm][lineMatch[1].str()] = lineMatch[2].str();
				std::getline(file, line);
			}
			if (enters)
			{
				ini[gm];
			}
		}
	}
}

#if 0
std::ifstream& operator>>(std::ifstream& iniFile, Entity e)
{
	std::map<std::string, std::map<std::string, std::string>> meta;

	std::regex groupe("\\[([a-zA-Z_]+)\\]");
	std::regex valeurs("([a-zA-Z_]+)\\s*=\\s*([\\w\"']+)");

	std::string curseur;
	while (std::getline(iniFile, curseur)) //Tant qu'il reste des lignes a lire
	{
		std::smatch groupeMatch;
		if (std::regex_match(curseur, groupeMatch, groupe)) //Si c'est un groupe
		{
			if (strToCreator.find(groupeMatch[0]))

		}
	}
	return iniFile;
}

std::ifstream& operator>>(std::ifstream& iniFile, Entity e)
{
	std::map<std::string, std::map<std::string, std::string>> meta;

	std::regex groupe("\\[([a-zA-Z_]+)\\]");
	std::regex valeurs("([a-zA-Z_]+)\\s*=\\s*([\\w\"']+)");

	std::string curseur;
	while (std::getline(iniFile, curseur)) //Tant qu'il reste des lignes a lire
	{
		std::smatch groupeMatch;
		if (std::regex_match(curseur, groupeMatch, groupe)) //Si c'est un groupe
		{
			std::map<std::string, std::string> secon;
			while (std::getline(iniFile, curseur) and (not std::regex_search(curseur, groupe))) //Tant qu'on est dans le groupe
			{
				std::smatch valeurMatch;
				if (std::regex_match(curseur, valeurMatch, valeurs)) //Si c'est une valeur
				{
					secon.emplace(valeurMatch[0].str(), valeurMatch[1].str());
				}
			}
			meta.emplace(groupeMatch.str(), secon);
		}
	}
	return iniFile;
}

void IniReader::ParseIni(std::ifstream& file, std::map<std::string, std::map<std::string, std::string>>& ini)
{
	std::string line;

	while (!file.eof())
	{
		std::getline(file, line);

		std::regex r(ini_line);
		std::smatch m;

		if (std::regex_match(line, m, r))
		{
			ini[m[1].str()] = m[2].str();
		}
	}
}

void IniReader::ParseIni(std::ifstream& file, std::map<std::string, std::map<std::string, std::string>>& ini)
{
	std::string line;

	std::smatch currentGroup;

	while (!file.eof())
	{
		std::getline(file, line);

		std::regex gr(group_line);
		std::smatch gm;

		std::regex r(ini_line);
		std::smatch m;

		if (std::regex_match(line, gm, gr))
		{
			currentGroup = gm;
			//std::getline(file, line);
		}
		else if (std::regex_match(line, m, r))
		{
			ini[currentGroup[1].str()][m[1].str()] = m[2].str();
		}
	}
}

#endif