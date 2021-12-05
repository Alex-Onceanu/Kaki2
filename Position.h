#pragma once

template<typename T>
struct Pos
{
	T x, y;
};

typedef Pos<int> Position;


template<typename T>
Pos<T> operator + (Pos<T> a, Pos<T> b)
{
	return { a.x + b.x, a.y + b.y };
}

template<typename T>
Pos<T> operator - (Pos<T> a, Pos<T> b)
{
	return { a.x - b.x, a.y - b.y };
}

template <typename T>
void operator += (Pos<T> &a, Pos<T> b)
{
	a = a + b;
}

template <typename T>
void operator -= (Pos<T> &a, Pos<T> b)
{
	a = a - b;
}

template <typename T>
Pos<T> operator* (Pos<T> a, double s)
{
	return { static_cast<T>(a.x * s), static_cast<T>(a.y * s) };
}

template <typename T>
Pos<T> operator/ (Pos<T> a, double s)
{
	return { static_cast<T>(a.x / s), static_cast<T>(a.y / s) };
}

template <typename T>
void operator *= (Pos<T>& a, Pos<T> b)
{
	a = a * b;
}
