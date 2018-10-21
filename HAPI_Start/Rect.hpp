#pragma once

struct Rect
{
	Rect() : Left(0), Right(0), Top(0), Bottom(0) {}
	Rect(int left, int right, int top, int bottom) : Left(left), Right(right), Top(top), Bottom(bottom) {}

	int Left, Right, Top, Bottom;

	int Width() { return (Right - Left); }
	int Height() { return (Bottom - Top); }

	//One or more points intersect
	bool Intersect(const Rect& otherRect);
	//Rectangle COMPLETELY contains OTHER_Rect 
	bool Contains(const Rect& otherRect);
	//Check of THIS rect is outide OTHER_Rect
	bool Outside(const Rect& otherRect);
	void ClipTo(const Rect& otherRect);
};

