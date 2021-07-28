#pragma once

#include "ControllerFactory.h"

#include <map>
#include <string>
#include <fstream>

class InitialData
{
public:
	std::map<std::string, std::map<std::string, std::string>> data;
};

class IniReader
{
public:
	void Read(ControllerFactory* cf, std::ifstream& iniFile, Entity& e);

private:
	void ParseIni(std::ifstream& file, std::map<std::string, std::string>& ini);
};