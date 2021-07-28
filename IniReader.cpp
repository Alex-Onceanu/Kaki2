#include "IniReader.h"
#include "Entity.h"
#include "ControllerFactory.h"

#include <fstream>
#include <map>
#include <string>
#include <regex>

constexpr auto ini_line{ "([A-Za-z_]+[A-Za-z0-9_-]*)\\s*=\\s*(\"?'?[A-Za-z0-9]+\\.*[A-Za-z0-9]*'?\"?)" };


void IniReader::Read(ControllerFactory* cf, std::ifstream& iniFile, Entity& e)
{
	std::map<std::string, std::string> ini;
	ParseIni(iniFile, ini);
	//e.LoadInitialData(cf, ini);
}

void IniReader::ParseIni(std::ifstream& file, std::map<std::string, std::string>& ini)
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

#endif