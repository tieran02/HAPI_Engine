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
	void RectToScreenSpace(Rect& rect);

	void Update();
	void Render();
	void Clear();

	void AddUIElement(const std::string& name, std::shared_ptr<UiElement> element);
	std::shared_ptr<UiElement> GetUIElement(const std::string& name);
private:
	Vector2f m_screenSize;
	Renderer* m_renderer{ nullptr };
	std::unordered_map<std::string, std::shared_ptr<UiElement>> m_uiElements;
};

