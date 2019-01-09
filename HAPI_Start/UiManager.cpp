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
	Vector2f worldPosition = mousePosition - m_renderer->GetOffset();
	return worldPosition;
}

void UiManager::RectToScreenSpace(Rect& rect)
{
	rect.Left /= m_screenSize.x;
	rect.Right /= m_screenSize.x;
	rect.Top /= m_screenSize.y;
	rect.Bottom /= m_screenSize.y;
}

void UiManager::Update()
{
	for (const auto& ui_element : m_uiElements)
	{
		ui_element.second->Update();
	}
}

void UiManager::Render()
{
	for (const auto& ui_element : m_uiElements)
	{
		ui_element.second->Render();
	}
}

void UiManager::Clear()
{
	m_uiElements.clear();
}

void UiManager::AddUIElement(const std::string& name, std::shared_ptr<UiElement> element)
{
	m_uiElements[name] =  element;
}

std::shared_ptr<UiElement> UiManager::GetUIElement(const std::string & name)
{
	if(m_uiElements.find(name) == m_uiElements.end())
		return nullptr;

	return m_uiElements.at(name);
}
