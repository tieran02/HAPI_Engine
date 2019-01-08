#include "UiTextElement.hpp"

void UiTextElement::Setup()
{
}

void UiTextElement::Update()
{
}

void UiTextElement::Render()
{

}

void UiTextElement::SetText(const std::string& text)
{
	m_text = text;
}

const std::string& UiTextElement::GetText() const
{
	return m_text;
}

void UiTextElement::SetFontSize(float size)
{
	m_fontSize = size;
}

float UiTextElement::GetFontSize() const
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
