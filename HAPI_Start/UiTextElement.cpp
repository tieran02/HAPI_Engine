#include "UiTextElement.hpp"
#include "HAPI_lib.h"
#include "UiManager.hpp"

void UiTextElement::Setup()
{

}

void UiTextElement::Update()
{
}

void UiTextElement::Render()
{
	Vector2f pixelPos = UiManager::Instance().ScreenSpaceToPosition(GetPosition());
	HAPI.RenderText((int)pixelPos.x, (int)pixelPos.y, m_colour, m_text, m_fontSize);
}

void UiTextElement::SetText(const std::string& text)
{
	m_text = text;
}

const std::string& UiTextElement::GetText() const
{
	return m_text;
}

void UiTextElement::SetFontSize(int size)
{
	m_fontSize = size;
}

int UiTextElement::GetFontSize() const
{
	return m_fontSize;
}

void UiTextElement::SetColour(const HAPISPACE::HAPI_TColour& colour)
{
	m_colour = colour;
}

const HAPISPACE::HAPI_TColour& UiTextElement::GetColour() const
{
	return m_colour;
}
