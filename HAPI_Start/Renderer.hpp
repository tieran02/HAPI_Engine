#pragma once
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Sprite.hpp"
#include "Rect.hpp"
#include <unordered_map>

#include <HAPI_Types.h>


class Game;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void Intialise(const Vector2i& screenSize, const std::string& windowName);

	const Vector2i& GetSize() const { return m_screenSize; };

	//Clear the screen buffer to a a colour value (4 bytes)
	void ClearScreen(const HAPISPACE::HAPI_TColour& colour);
	//Clear the screen buffer to a gray scale value (Between 0-255)
	void ClearScreen(HAPISPACE::BYTE grayScale);

	//Set a specific pixel on the screen to a certain colour (Pass in position Vector2i)
	void SetPixel(const Vector2i& vec, const HAPISPACE::HAPI_TColour& colour);
	//Set a specific pixel on the screen to a certain colour (Pass in position as int)
	void SetPixel(int x, int y, const HAPISPACE::HAPI_TColour& colour);

	//blit Texture
	void Blit(const Sprite& sprite ,const Vector2i& pos);
	//Blit texture and only render a region of the sprite
	void Blit(const Sprite& sprite, const Vector2i& pos, const Rect& area);

	//Blit texture using perspective scaling (SLOW, NO ALPHA)
	void Blit3D(const Sprite& sprite, const Vector3f& pos);

	//Blit texture with alpha blending
	void BlitAlpha(const Sprite& sprite, const Vector2i& pos);
	//Blit texture with alpha blending and only render a region of the sprite
	void BlitAlpha(const Sprite& sprite, const Vector2i& pos, const Rect& area);

	//Draw a sprite and clip it to the screen (Vector2i as pos)
	void Draw(const Sprite& sprite, const Vector2i& pos);
	//Draw a sprite and clip it to the screen (Vector2f as pos)
	void Draw(const Sprite& sprite, const Vector2f& pos);
	//Draw a sprite and clip it to the screen (Vector3f as pos)
	void Draw(const Sprite& sprite, const Vector3f& pos);

	//Load a sprite into the sprite map
	Sprite* LoadSprite(std::string name, const std::string& path);

	void Cleanup();

private:
	Vector2i m_screenSize;
	HAPISPACE::BYTE* m_screen;
	float m_eyeDistance{ 100.0f };

	//sprite map
	std::unordered_map<std::string, Sprite*> m_sprites;

	//Project a 3D world space to the Screen space
	Vector2i projectPosition(const Vector3f& sourcePos);

};

