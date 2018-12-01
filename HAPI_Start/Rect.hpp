#pragma once
#include "Vector2.hpp"

struct Rect
{
	Rect() : Left(0), Right(0), Top(0), Bottom(0) {}
	Rect(int left, int right, int top, int bottom) : Left(left), Right(right), Top(top), Bottom(bottom) {}

	int Left, Right, Top, Bottom;

	int Width() { return (Right - Left); }
	int Height() { return (Bottom - Top); }

	//Translate rect by a vector position
	void Translate(const Vector2i& pos);
	//Clamp rect values
	void Clamp(const Rect& clampRect);
	//One or more points intersect
	bool Intersect(const Rect& otherRect);
	//A single point intersects
	bool Intersect(const Vector2i& point);
	//A single point intersects
	bool Intersect(const Vector2f& point);
	//Rectangle COMPLETELY contains OTHER_Rect 
	bool Contains(const Rect& otherRect);
	//Check of THIS rect is outide OTHER_Rect
	bool Outside(const Rect& otherRect);
	void ClipTo(const Rect& otherRect);

	Vector2i Center();

	//Add a vector to the current vector
	Rect& operator+=(const Rect& rhs)
	{
		Left += rhs.Left;
		Right += rhs.Right;
		Top += rhs.Top;
		Bottom += rhs.Bottom;
		return *this;
	}


};

