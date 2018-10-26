#pragma once
#include <HAPI_lib.h>
#include "Vector2.hpp"
#include "Rect.hpp"

class Sprite
{
public:
	Sprite();
	~Sprite();

	//load texture from file
	void Load(const std::string& path);

	const int& GetWidth() const { return m_width; }
	const int& GetHeight() const { return m_height; }


	//blit Texture
	void Blit(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos) const;
	//Blit texture and only render a region of the sprite
	void Blit(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const;

	//Blit texture with alpha blending
	void BlitAlpha(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos) const;
	//Blit texture with alpha blending and only render a region of the sprite
	void BlitAlpha(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const;

	bool HasAlpha() const { return m_hasAlpha; }

private:
	int m_width{ 0 }, m_height{ 0 };
	HAPISPACE::BYTE* m_texture{ nullptr };
	bool m_hasAlpha;

	bool loadTexture(const std::string& path);
	//Go through each pixel of the texture and check if it has an alpha channel
	bool checkAlpha();
};

