#pragma once

#include <string>
#include <sstream>

template <typename T>
inline void Clamp(T& src, const T& min, const T& max)
{
	if (src < min)
	{
		src = min;
		return;
	}
	if (src > max)
	{
		src = max;
	}
}

inline int StrToInt(std::string str)
{
	std::stringstream ss;
	int num;
	ss << str;
	ss >> num;
	return num;
}

inline void RemoveZerosStr(std::string& str)
{
	str = str.erase(0, str.find_first_not_of('\0'));
}