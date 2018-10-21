#include "Rect.hpp"
#include <algorithm>


bool Rect::Intersect(const Rect & otherRect)
{
	if (std::max(Left, otherRect.Left) <= std::min(Right, otherRect.Right) && std::max(Top, otherRect.Top) <= std::min(Bottom, otherRect.Bottom))
		return true;

	return false;
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
