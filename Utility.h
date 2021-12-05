#pragma once

#include <string>
#include <sstream>

template <typename T>
inline bool Clamp(T& src, const T& min, const T& max)
{
	if (src < min)
	{
		src = min;
		return true;
	}
	if (src > max)
	{
		src = max;
		return true;
	}
	return false;
}

inline int StrToInt(std::string str)
{
	return std::atoi(str.c_str());
}

inline std::string IntToStr(int num)
{
	std::stringstream ss;
	ss << char(num);
	return ss.str();
}

inline void RemoveZerosStr(std::string& str)
{
	str = str.erase(0, str.find_first_not_of('\0'));
}

inline bool Collision(const Position& p_a, const Rect& r_a, const Position& p_b, const Rect& r_b)
{
	return((p_a.x + r_a.w >= p_b.x) and (p_a.x <= p_b.x + r_b.w) and (p_a.y + r_a.h >= p_b.y) and (p_a.y <= p_b.y + r_b.h));
}
