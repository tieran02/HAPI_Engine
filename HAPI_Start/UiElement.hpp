#pragma once
#include "Rect.hpp"

class UiElement
{
public:
	UiElement() {};
	virtual ~UiElement() = default;

	virtual void Update() = 0;
	virtual void Render() = 0;

	Vector2f GetPosition() const { return Vector2f(m_screenSpace.Left, m_screenSpace.Top); }
	void SetPosition(const Vector2f& position) { m_screenSpace.Translate(position); }
	void SetActive(bool active) { m_active = active; }
	bool IsActive()const { return m_active; }
protected:
	int m_id{ -1 };
	Rect m_screenSpace;
	bool m_active{true};
};
