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

Vector2f Rect::GetIntersectionDepth(const Rect& rectB)
{
	// Calculate half sizes.
	float halfWidthA = Width() / 2.0f;
	float halfHeightA = Height() / 2.0f;
	float halfWidthB = rectB.Width() / 2.0f;
	float halfHeightB = rectB.Height() / 2.0f;

	// Calculate centers.
	Vector2 centerA(Left + halfWidthA, Top + halfHeightA);
	Vector2 centerB(rectB.Left + halfWidthB, rectB.Top + halfHeightB);

	// Calculate current and minimum-non-intersecting distances between centers.
	float distanceX = centerA.x - centerB.x;
	float distanceY = centerA.y - centerB.y;
	float minDistanceX = halfWidthA + halfWidthB;
	float minDistanceY = halfHeightA + halfHeightB;

	// If we are not intersecting at all, return (0, 0).
	if (abs(distanceX) >= minDistanceX || abs(distanceY) >= minDistanceY)
		return Vector2f(0,0);

	// Calculate and return intersection depths.
	float depthX = distanceX > 0 ? minDistanceX - distanceX : -minDistanceX - distanceX;
	float depthY = distanceY > 0 ? minDistanceY - distanceY : -minDistanceY - distanceY;
	return Vector2f(depthX, depthY);
}

Vector2i Rect::Center() const
{
	int x = Left + (Width() / 2);
	int y = Top + (Height() / 2);
	return Vector2i(x, y);
}

std::vector<Vector2f> Rect::RotatedRectangle(float radians) const
{
	std::vector<Vector2f> points;
	points.resize(4);

	const float cosine{ cos(radians) };
	const float sine{ sin(radians) };

	float p1x = (Left * cosine + Top * sine);
	float p1y = (Top * cosine - Left * sine);
	points[0] = Vector2f { p1x,p1y };

	float p2x = (Right * cosine + Top * sine);
	float p2y = (Top * cosine - Right * sine);
	points[1] = Vector2f{ p2x,p2y };

	float p3x = (Right * cosine + Bottom * sine);
	float p3y = (Bottom * cosine - Right * sine);
	points[2] = Vector2f{ p3x,p3y };

	float p4x = (Left * cosine + Bottom * sine);
	float p4y = (Bottom * cosine - Left * sine);
	points[3] = Vector2f{ p4x,p4y };

	return points;
}

void Rect::OBRBoundingRect(std::vector<Vector2f> points)
{
	int minX = std::numeric_limits<int>::max();
	int minY = std::numeric_limits<int>::max();
	int maxX = std::numeric_limits<int>::min();
	int maxY = std::numeric_limits<int>::min();

	for (size_t i = 0; i < 4; i++)
	{
		if (points[i].x < minX)
			minX = points[i].x;
		if (points[i].y < minY)
			minY = points[i].y;
		if (points[i].x > maxX)
			maxX = points[i].x;
		if (points[i].y > maxY)
			maxY = points[i].y;
	}

	Left = minX;
	Right = maxX;
	Top = minY;
	Bottom = maxY;
}

