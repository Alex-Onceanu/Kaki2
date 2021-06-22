#pragma once

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