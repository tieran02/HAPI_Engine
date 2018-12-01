#include "Rect.hpp"
#include <algorithm>


void Rect::Translate(const Vector2i& pos)
{
	Left += pos.x;
	Right += pos.x;
	Top += pos.y;
	Bottom += pos.y;
}

void Rect::Clamp(const Rect& clampRect)
{
	Left = std::clamp(Left, clampRect.Left, clampRect.Right);
	Right = std::clamp(Right, clampRect.Left, clampRect.Right);
	Top = std::clamp(Top, clampRect.Top, clampRect.Bottom);
	Bottom = std::clamp(Bottom, clampRect.Top, clampRect.Bottom);
}

bool Rect::Intersect(const Rect & otherRect)
{
	if (std::max(Left, otherRect.Left) <= std::min(Right, otherRect.Right) && std::max(Top, otherRect.Top) <= std::min(Bottom, otherRect.Bottom))
		return true;

	return false;
}

bool Rect::Intersect(const Vector2i& point)
{
	//get bottom right
	Vector2i min = Vector2i(Left, Top);
	Vector2i max = Vector2i(Right, Bottom);

	return (point.x < max.x && point.x > min.x &&
		point.y < max.y && point.y > min.y);
}

bool Rect::Intersect(const Vector2f& point)
{
	//get bottom right
	Vector2i min = Vector2i(Left, Top);
	Vector2i max = Vector2i(Right, Bottom);

	return (point.x < max.x && point.x > min.x &&
		point.y < max.y && point.y > min.y);
}

bool Rect::Contains(const Rect & otherRect)
{
	if (otherRect.Right <= Right && otherRect.Left >= Left && otherRect.Top >= Top && otherRect.Bottom <= Bottom)
		return true;
	return false;
}

bool Rect::Outside(const Rect& otherRect)
{
	return !Intersect(otherRect);
}

void Rect::ClipTo(const Rect& otherRect)
{
	Left = std::max(Left, otherRect.Left);
	Right = std::min(Right, otherRect.Right);
	Top = std::max(Top, otherRect.Top);
	Bottom = std::min(Bottom, otherRect.Bottom);
}

Vector2i Rect::Center()
{
	int x = Left + (Width() / 2);
	int y = Top + (Height() / 2);
	return Vector2i(x, y);
}
