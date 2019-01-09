#include "UiButtonElement.hpp"
#include "HAPI_lib.h"
#include "UiManager.hpp"

UiButtonElement::~UiButtonElement()
{
}

void UiButtonElement::Setup()
{

}

void UiButtonElement::Update()
{
	const auto& mouse_data = HAPI.GetMouseData();
	Vector2f mouse_pos = Vector2f((float)mouse_data.x, (float)mouse_data.y);
	Vector2f screen_mouse = UiManager::Instance().PositionToScreenSpace(mouse_pos);

	int left, right,top, bottom;
	HAPI.GetTextSize(m_text, m_fontSize, 0, left, right, top, bottom);
	Rect textRect((float)left, (float)right, (float)top, (float)bottom);
	textRect.Translate(UiManager::Instance().ScreenSpaceToPosition(GetPosition()));
	//map to screen space
	UiManager::Instance().RectToScreenSpace(textRect);

	if (textRect.Intersect(screen_mouse))
		m_selected = true;
	else
		m_selected = false;
}

void UiButtonElement::Render()
{
	Vector2f pixelPos = UiManager::Instance().ScreenSpaceToPosition(GetPosition());
	if(m_selected)
		HAPI.RenderText((int)pixelPos.x, (int)pixelPos.y, m_highLightColour, m_text, m_fontSize);
	else
		HAPI.RenderText((int)pixelPos.x, (int)pixelPos.y, m_colour, m_text, m_fontSize);
}

bool UiButtonElement::IsSelected() const
{
	if (this == nullptr)
		return false;
	const auto& mouse_data = HAPI.GetMouseData();
	return m_selected && mouse_data.leftButtonDown;
}

void UiButtonElement::SetText(const std::string& text)
{
	m_text = text;
}

const std::string& UiButtonElement::GetText() const
{
	return m_text;
}

void UiButtonElement::SetFontSize(int size)
{
	m_fontSize = size;
}

int UiButtonElement::GetFontSize() const
{
	return m_fontSize;
}

void UiButtonElement::SetColour(const HAPISPACE::HAPI_TColour& colour)
{
	m_colour = colour;
}

const HAPISPACE::HAPI_TColour& UiButtonElement::GetColour() const
{
	return m_colour;
}
