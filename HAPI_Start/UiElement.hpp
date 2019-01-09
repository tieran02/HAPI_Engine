#pragma once
#include "Rect.hpp"

class UiElement
{
public:
	UiElement() {};
	virtual ~UiElement() = default;

	virtual void Setup() = 0;
	virtual void Update() = 0;
	virtual void Render() = 0;

	Vector2f GetPosition() const { return Vector2f(m_screenSpace.Left, m_screenSpace.Top); }
	void SetPosition(const Vector2f& position) { m_screenSpace.Translate(position); }
protected:
	int m_id{ -1 };
	Rect m_screenSpace;
	int m_parentID;
	std::vector<int> m_childrenID;
};
