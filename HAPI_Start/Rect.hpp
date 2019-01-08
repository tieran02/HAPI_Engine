#pragma once
#include "Vector2.hpp"
#include <vector>

struct Rect
{
	Rect() : Left(0), Right(0), Top(0), Bottom(0) {}
	Rect(float left, float right, float top, float bottom) : Left(left), Right(right), Top(top), Bottom(bottom) {}

	float Left, Right, Top, Bottom;

	float Width() const { return (Right - Left); }
	float Height() const { return (Bottom - Top); }

	//Translate rect by a vector position
	void Translate(const Vector2f& pos);
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

	Vector2f GetIntersectionDepth(const Rect& rectB);

	//Return the center point of the rectangle
	Vector2f Center() const;

	std::vector<Vector2f> RotatedRectangle(float radians) const;

	void OBRBoundingRect(std::vector<Vector2f> points);

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

