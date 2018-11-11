#pragma once
#include <string>
#include <HAPI_lib.h>
#include "Vector2.hpp"
#include "Rect.hpp"

class Texture
{
public:
	Texture(){}
	~Texture();

	//load texture from file
	void Load(const std::string& path);
	//Load texture as a sub texture
	void Load(Texture* mainTexture, int offset);

	int GetWidth() const { return m_width; }
	int GetHeight() const { return m_height; }

	bool HasAlpha() const { return m_hasAlpha; }

	HAPISPACE::BYTE* Data() const { return m_texture; }

	//DrawAnimation texture without alpha and only render a region of the sprite
	void BlitFast(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const;

	//BlitFast texture with alpha blending and only render a region of the sprite
	void BlitAlpha(HAPISPACE::BYTE* screen, Vector2i screenSize, const Vector2i& pos, const Rect& area) const;

private:
	int m_width{ 0 }, m_height{ 0 };
	HAPISPACE::BYTE* m_texture{ nullptr };
	bool m_hasAlpha;
	bool m_isSubTexture{ false };

	bool loadTexture(const std::string& path);
	//Go through each pixel of the texture and check if it has an alpha channel
	bool checkAlpha();
};

