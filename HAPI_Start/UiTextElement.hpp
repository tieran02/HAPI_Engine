#pragma once
#include "UiElement.hpp"
#include <HAPI_Types.h>

class UiTextElement : public UiElement
{
public:
	UiTextElement(int ID) : UiElement(ID){}
	void Setup() override;
	void Update() override;
	void Render() override;

	void SetText(const std::string& text);
	const std::string& GetText() const;
	void SetFontSize(float size);
	float GetFontSize() const;
	void SetColour(const HAPISPACE::HAPI_TColour& colour);
	const HAPISPACE::HAPI_TColour& GetColour() const;
private:
	std::string m_text;
	float m_fontSize{10};
	HAPISPACE::HAPI_TColour m_colour { HAPISPACE::HAPI_TColour::WHITE};
};
