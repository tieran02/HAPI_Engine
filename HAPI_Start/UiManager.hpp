#pragma once
#include "Renderer.hpp"
#include "UiElement.hpp"

class UiManager
{
public:
	static UiManager& Instance()
	{
		static UiManager instance;
		return instance;
	}

	UiManager(UiManager const&) = delete;
	void operator=(UiManager const&) = delete;
private:
	UiManager() {}

public:
	void SetRenderer(Renderer* renderer);
	void SetScreenSize(const Vector2f& size);
	Vector2f PositionToScreenSpace(const Vector2f& position);
	Vector2f ScreenSpaceToPosition(const Vector2f& position);
	Vector2f ScreenSpaceToWorld(const Vector2f& position);

	void AddUIElement(std::shared_ptr<UiElement> element);
private:
	Vector2f m_screenSize;
	Renderer* m_renderer{ nullptr };
	std::shared_ptr<UiElement> m_uiElements;
};

