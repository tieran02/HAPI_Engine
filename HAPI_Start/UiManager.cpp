#include "UiManager.hpp"

void UiManager::SetRenderer(Renderer* renderer)
{
	m_renderer = renderer;
}

void UiManager::SetScreenSize(const Vector2f& size)
{
	m_screenSize = size;
}

Vector2f UiManager::PositionToScreenSpace(const Vector2f& position)
{
	 return Vector2f(position.x / m_screenSize.x, position.y / m_screenSize.y);
}

Vector2f UiManager::ScreenSpaceToPosition(const Vector2f& position)
{
	return Vector2f(position.x * m_screenSize.x, position.y * m_screenSize.y);
}

Vector2f UiManager::ScreenSpaceToWorld(const Vector2f& position)
{
	Vector2f mousePosition = ScreenSpaceToPosition(position);
	Vector2f worldPosition = mousePosition + m_renderer->GetOffset();
	return worldPosition;
}

void UiManager::AddUIElement(std::shared_ptr<UiElement> element)
{
}
