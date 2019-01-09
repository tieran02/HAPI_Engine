#pragma once
#include "UiElement.hpp"
#include "HAPI_Types.h"
#include "Scene.hpp"

class UiButtonElement : public UiElement
{
public:
	~UiButtonElement();
	void Setup() override;
	void Update() override;
	void Render() override;

	void SetText(const std::string& text);
	const std::string& GetText() const;
	void SetFontSize(int size);
	int GetFontSize() const;
	void SetColour(const HAPISPACE::HAPI_TColour& colour);
	const HAPISPACE::HAPI_TColour& GetColour() const;

	bool IsSelected() const;
private:
	std::string m_text;
	int m_fontSize{ 10 };
	HAPISPACE::HAPI_TColour m_colour{ HAPISPACE::HAPI_TColour::WHITE };
	HAPISPACE::HAPI_TColour m_highLightColour{ HAPISPACE::HAPI_TColour::BLUE };
	bool m_selected{false};
};

